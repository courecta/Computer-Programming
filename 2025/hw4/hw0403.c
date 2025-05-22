/*
 * hw0403.c -- list dependencies and files in a .deb using liblzma
 * Usage: ./hw0403 pkg.deb
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lzma.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdint.h>
#include <errno.h>

#define AR_MAGIC "!<arch>\n"
#define AR_HDR_SIZE 60
#define TAR_BLOCK 512
#define NAME_SIZE 100
#define BUF_OUT (1 << 16)
#define MAX_DEPTH 32

// AR archive header
typedef struct {
    char name[16];
    char timestamp[12];
    char owner[6];
    char group[6];
    char mode[8];
    char size[10];
    char fmag[2];
} ar_header;

// Tree node structure
typedef struct Entry {
    char *name;
    int is_dir;
    char *link_target;
    int is_last;             // for ASCII tree
    struct Entry *children;
    struct Entry *next;
} Entry;

static Entry *root = NULL;

static void die(const char *msg) {
    perror(msg);
    exit(1);
}

static void read_exact(int fd, void *buf, size_t n) {
    size_t off = 0;
    while (off < n) {
        ssize_t r = read(fd, (char*)buf + off, n - off);
        if (r <= 0) die("read");
        off += r;
    }
}

static size_t parse_decimal(const char *s) {
    return strtoul(s, NULL, 10);
}

static void xz_decompress(const uint8_t *in_mem, size_t in_size,
                          uint8_t **out_mem, size_t *out_size) {
    lzma_stream strm = LZMA_STREAM_INIT;
    if (lzma_auto_decoder(&strm, UINT64_MAX, 0) != LZMA_OK)
        die("lzma_auto_decoder");

    strm.next_in = in_mem;
    strm.avail_in = in_size;

    size_t cap = BUF_OUT, used = 0;
    *out_mem = malloc(cap);
    if (!*out_mem) die("malloc");

    while (1) {
        if (used + BUF_OUT > cap) {
            cap *= 2;
            *out_mem = realloc(*out_mem, cap);
            if (!*out_mem) die("realloc");
        }
        strm.next_out = *out_mem + used;
        strm.avail_out = BUF_OUT;
        lzma_ret ret = lzma_code(&strm, LZMA_RUN);
        if (ret == LZMA_STREAM_END) {
            used += BUF_OUT - strm.avail_out;
            break;
        }
        if (ret != LZMA_OK) die("lzma_code");
        used += BUF_OUT - strm.avail_out;
    }
    *out_size = used;
    lzma_end(&strm);
}

static uint8_t *read_member_data(int fd, size_t size) {
    uint8_t *buf = malloc(size);
    if (!buf) die("malloc");
    read_exact(fd, buf, size);
    if (size & 1) lseek(fd, 1, SEEK_CUR);
    return buf;
}

// Find or insert sorted in siblings
static Entry *find_or_create(Entry **siblings, const char *name) {
    Entry **prev = siblings;
    for (Entry *e = *siblings; e; prev = &e->next, e = e->next) {
        int cmp = strcmp(e->name, name);
        if (cmp == 0) return e;
        if (cmp > 0) break;
    }
    Entry *n = calloc(1, sizeof(Entry));
    if (!n) die("calloc");
    n->name = strdup(name);
    if (!n->name) die("strdup");
    n->is_dir = 1;
    n->next = *prev;
    *prev = n;
    return n;
}

// same as above but not sorted by alphabet
//static Entry *find_or_create(Entry **siblings, const char *name) {
//    for (Entry *e = *siblings; e; e = e->next)
//        if (strcmp(e->name, name) == 0)
//            return e;
//    // not found → append to end
//    Entry *n = calloc(1, sizeof(Entry));
//    n->name = strdup(name);
//    n->is_dir = 1;
//    n->next = NULL;
//    if (!*siblings)
//        *siblings = n;
//    else {
//        Entry *tail = *siblings;
//        while (tail->next) tail = tail->next;
//        tail->next = n;
//    }
//    return n;
//}

static void add_path(const char *path, int is_dir, const char *link_target) {
    if (strncmp(path, "./", 2) == 0) path += 2;
    char *copy = strdup(path);
    if (!copy) die("strdup");
    char *ctx = NULL;
    char *comp = strtok_r(copy, "/", &ctx);
    Entry **siblings = &root;
    while (comp) {
        Entry *node = find_or_create(siblings, comp);
        char *next = strtok_r(NULL, "/", &ctx);
        if (next) {
            siblings = &node->children;
            comp = next;
        } else {
            node->is_dir = is_dir;
            if (link_target) {
                node->link_target = strdup(link_target);
                if (!node->link_target) die("strdup");
            }
            break;
        }
    }
    free(copy);
}

// Mark last siblings for ASCII art
static void assign_sibling_flags(Entry *list) {
    for (Entry *e = list; e; e = e->next) {
        e->is_last = (e->next == NULL);
        if (e->children) assign_sibling_flags(e->children);
    }
}

static void print_tree(Entry *list, int depth, int verticals[]) {
    for (Entry *e = list; e; e = e->next) {
        for (int i = 0; i < depth; i++) {
            printf(verticals[i] ? "|   " : "    ");
        }
        printf("+---- %s", e->name);
        if (e->link_target) printf(" -> %s", e->link_target);
        printf("\n");
        verticals[depth] = !e->is_last;
        if (e->children) print_tree(e->children, depth+1, verticals);
    }
}

static void parse_control_tar(const uint8_t *tar, size_t sz) {
    printf("Dependencies:\n");
    size_t off = 0;
    while (off + TAR_BLOCK <= sz) {
        const char *raw = (const char*)(tar + off);
        if (!raw[0]) break;
        char name[NAME_SIZE+1] = {0};
        memcpy(name, raw, strnlen(raw, NAME_SIZE));
        char *fname = name;
        if (strncmp(fname, "./", 2) == 0) fname += 2;
        size_t entry_size = strtoul((char*)(tar + off + 124), NULL, 8);
        if (strcmp(fname, "control") == 0) {
            const char *data = (char*)tar + off + TAR_BLOCK;
            char *ctrl = strndup(data, entry_size);
            if (!ctrl) die("strndup");
            char *line = strtok(ctrl, "\n");
            int idx = 1;
            while (line) {
                if (strncmp(line, "Depends:", 8) == 0) {
                    char *dep = line + 8;
                    char *tok = strtok(dep, ",");
                    while (tok) {
                        while (*tok==' '||*tok=='\t') tok++;
                        printf("%d. %s\n", idx++, tok);
                        tok = strtok(NULL, ",");
                    }
                }
                line = strtok(NULL, "\n");
            }
            free(ctrl);
            return;
        }
        size_t blocks = ((entry_size + TAR_BLOCK - 1) / TAR_BLOCK) + 1;
        off += blocks * TAR_BLOCK;
    }
}

static void parse_data_tar(const uint8_t *tar, size_t sz) {
    printf("Files:\n");
    size_t off = 0;
    int zero_blocks = 0;
    while (off + TAR_BLOCK <= sz) {
        const char *raw = (const char *)(tar + off);
        if (raw[0] == '\0') {
            zero_blocks++;
            if (zero_blocks == 2) break;       // real end-of-archive
            off += TAR_BLOCK;
            continue;                          // skip padding or PAX header
        }
        zero_blocks = 0;                       // reset if it wasn’t an empty block

        size_t entry_size = strtoul((char*)(tar + off + 124), NULL, 8);
        char type = *(char*)(tar + off + 156);
        char linkname[NAME_SIZE+1] = {0};
        if (type == '2') memcpy(linkname, tar + off + 157, NAME_SIZE);
        int is_dir = (type == '5');
        add_path(raw, is_dir, linkname[0] ? linkname : NULL);

        size_t blocks = ((entry_size + TAR_BLOCK - 1) / TAR_BLOCK) + 1;
        off += blocks * TAR_BLOCK;
    }

    assign_sibling_flags(root);
    int verticals[MAX_DEPTH] = {0};
    print_tree(root, 0, verticals);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s pkg.deb\n", argv[0]);
        return 1;
    }
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) die("open");

    char magic[8];
    read_exact(fd, magic, 8);
    if (memcmp(magic, AR_MAGIC, 8)) {
        fprintf(stderr, "Not a .deb file\n");
        return 1;
    }

    uint8_t *ctrl_xz = NULL, *data_xz = NULL;
    size_t ctrl_sz = 0, data_sz = 0;
    ar_header hdr;

    while (read(fd, &hdr, AR_HDR_SIZE) == AR_HDR_SIZE) {
        size_t mem_sz = parse_decimal(hdr.size);
        char name[17] = {0};
        memcpy(name, hdr.name, 16);
        for (int i = 15; i >= 0; i--) {
            if (name[i]==' '||name[i]=='/') name[i]='\0'; else break;
        }
        if (!strcmp(name, "control.tar.xz")) {
            ctrl_xz = read_member_data(fd, mem_sz);
            ctrl_sz = mem_sz;
        } else if (!strcmp(name, "data.tar.xz")) {
            data_xz = read_member_data(fd, mem_sz);
            data_sz = mem_sz;
        } else {
            lseek(fd, mem_sz + (mem_sz & 1), SEEK_CUR);
        }
    }
    close(fd);

    if (!ctrl_xz || !data_xz) {
        fprintf(stderr, "Missing control or data\n");
        return 1;
    }

    uint8_t *ctrl_tar, *data_tar;
    size_t ctrl_tar_sz, data_tar_sz;
    xz_decompress(ctrl_xz, ctrl_sz, &ctrl_tar, &ctrl_tar_sz);
    xz_decompress(data_xz, data_sz, &data_tar, &data_tar_sz);

    parse_control_tar(ctrl_tar, ctrl_tar_sz);
    parse_data_tar(data_tar, data_tar_sz);

    free(ctrl_xz); free(data_xz);
    free(ctrl_tar); free(data_tar);
    return 0;
}
