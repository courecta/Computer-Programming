#define _DEFAULT_SOURCE   // for feature macros
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <getopt.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

typedef struct {
    double interval;      // -t (decimal seconds)
    long count;          // -c
    int pid_only;        // -p
    char sort_by;        // 'p'=pid, 'l'=cpu, 'm'=mem
} options_t;

typedef struct {
    int    pid;
    char   name[64];
    char   state;
    unsigned long utime, stime;
    unsigned long vsize;
    double cpu_pct;
} procinfo_t;

static void usage(const char *p) {
    fprintf(stderr,
        "Usage: %s [options]\n"
        "  -t, --time-interval=[SEC] update every SEC seconds (decimal, default 5)\n"
        "  -c, --count=[NUM]         update NUM times (default infinite)\n"
        "  -p, --pid=[PID]           only show that PID\n"
        "  -l, --loading             sort by CPU%%\n"
        "  -m, --memory              sort by memory usage\n"
        "  -h, --help                this help\n",
        p);
    exit(p?0:1);
}

static options_t parse_args(int argc, char **argv) {
    options_t opts = {5.0, -1, -1, 'p'};
    static struct option longopts[] = {
        {"time-interval", required_argument, NULL, 't'},
        {"count",         required_argument, NULL, 'c'},
        {"pid",           required_argument, NULL, 'p'},
        {"loading",       no_argument,       NULL, 'l'},
        {"memory",        no_argument,       NULL, 'm'},
        {"help",          no_argument,       NULL, 'h'},
        {0,0,0,0}
    };
    int c, idx;
    while ((c = getopt_long(argc, argv, "t:c:p:lmh", longopts, &idx)) != -1) {
        switch (c) {
            case 't': opts.interval = atof(optarg); break;
            case 'c': opts.count    = atol(optarg); break;
            case 'p': opts.pid_only = atoi(optarg); break;
            case 'l': opts.sort_by  = 'l'; break;
            case 'm': opts.sort_by  = 'm'; break;
            case 'h': usage(argv[0]); break;
            default:  usage(NULL);
        }
    }
    return opts;
}

// Read total jiffies from /proc/stat
static unsigned long read_total_jiffies(void) {
    FILE *f = fopen("/proc/stat","r");
    if (!f) return 0;
    unsigned long user,nice,sys,idle,iowait,irq,softirq,steal;
    fscanf(f,"cpu  %lu %lu %lu %lu %lu %lu %lu %lu",
           &user,&nice,&sys,&idle,&iowait,&irq,&softirq,&steal);
    fclose(f);
    return user+nice+sys+idle+iowait+irq+softirq+steal;
}

// Parse /proc/[pid]/stat for utime, stime, vsize, name, state
static int read_proc_stat(int pid, unsigned long *ut, unsigned long *st, unsigned long *vs, char *name, char *state) {
    char path[64], buf[512];
    snprintf(path,sizeof(path),"/proc/%d/stat",pid);
    FILE *f = fopen(path,"r");
    if (!f) {
        if (errno==ENOENT) fprintf(stderr, "hw0401: /proc/%d does not exist\n", pid);
        return -1;
    }
    if (!fgets(buf,sizeof(buf),f)) { fclose(f); return -1; }
    fclose(f);
    char comm[64], stch;
    unsigned long utime, stime, vsize;
    int ret = sscanf(buf,
    "%*d (%63[^)]) %c "                  /* pid, comm, state */
    "%*d %*d %*d %*d %*d "                /* ppid … tpgrp */
    "%*u %*u %*u %*u %*u "                /* flags … cmajflt */
    "%lu %lu "                            /* utime (14), stime (15) */
    "%*d %*d %*d %*d %*u %*u "            /* cutime … num_threads */
    "%*d %*d "                            /* itrealvalue, starttime (22) */
    "%lu",                                /* **vsize** (23) */
    comm, &stch, &utime, &stime, &vsize);
    if (ret < 5) return -1;
    strncpy(name, comm, 63); name[63]=0;
    *state = stch;
    *ut = utime; *st = stime; *vs = vsize;
    return 0;
}

static int cmp_pid(const void *a, const void *b) {
    return ((procinfo_t*)a)->pid - ((procinfo_t*)b)->pid;
}
static int cmp_cpu(const void *a, const void *b) {
    double d = ((procinfo_t*)b)->cpu_pct - ((procinfo_t*)a)->cpu_pct;
    return d>0?1:(d<0?-1:0);
}
static int cmp_mem(const void *a, const void *b) {
    procinfo_t *p_a = (procinfo_t*)a;
    procinfo_t *p_b = (procinfo_t*)b;
    if (p_b->vsize > p_a->vsize) return 1;
    if (p_a->vsize > p_b->vsize) return -1;
    return 0;
}

int main(int argc, char **argv) {
    options_t opts = parse_args(argc, argv);

    // If specific PID requested, verify /proc/PID exists
    if (opts.pid_only>0) {
        char pdir[64]; snprintf(pdir,sizeof(pdir),"/proc/%d",opts.pid_only);
        struct stat sb;
        if (stat(pdir,&sb)==-1) {
            fprintf(stderr,"hw0401: /proc/%d does not exist\n",opts.pid_only);
            return 1;
        }
    }

    unsigned long prev_tot = read_total_jiffies();
    unsigned long prev_proc[32768] = {0};
    long iterations = 0;
    do {
        unsigned long tot = read_total_jiffies();
        DIR *d = opendir("/proc");
        if (!d) { perror("opendir"); exit(1); }

        procinfo_t *plist = malloc(sizeof(procinfo_t)*1024);
        int count = 0; struct dirent *de;
        while ((de = readdir(d))) {
            if (!isdigit(de->d_name[0])) continue;
            int pid = atoi(de->d_name);
            if (opts.pid_only>0 && pid!=opts.pid_only) continue;

            procinfo_t pi = {0}; unsigned long ut, st, vs;
            if (read_proc_stat(pid,&ut,&st,&vs,pi.name,&pi.state)<0) continue;
            pi.pid = pid; pi.utime = ut; pi.stime = st; pi.vsize = vs;
            unsigned long curj = ut+st;
            unsigned long dp = curj - prev_proc[pid];
            unsigned long dt = tot - prev_tot;
            pi.cpu_pct = dt ? (100.0 * dp / dt) : 0.0;
            prev_proc[pid] = curj;
            plist[count++] = pi;
            if (count>=1024) break;
        }
        closedir(d);
        prev_tot = tot;

        // choose sort comparator
        int (*cmp)(const void*,const void*) = cmp_pid;
        if (opts.sort_by=='l') cmp = cmp_cpu;
        else if (opts.sort_by=='m') cmp = cmp_mem;
        qsort(plist, count, sizeof(procinfo_t), cmp);

        // header
        printf(" PID   NAME           S     CPU     MEM\n");
        for (int i=0; i< count && i<10; i++) {
            procinfo_t *p = &plist[i];
            printf("%5d %-15s %-1c %6.2f%% %8.2f\n",
                p->pid, p->name, p->state, p->cpu_pct,
                p->vsize / 1024.0 / 1024.0);
        }
        free(plist);

        if (opts.count>0 && ++iterations>=opts.count) break;
        struct timespec ts;
        ts.tv_sec  = (time_t)opts.interval;
        ts.tv_nsec = (opts.interval - ts.tv_sec) * 1e9;
        nanosleep(&ts, NULL);
    } while (1);

    return 0;
}