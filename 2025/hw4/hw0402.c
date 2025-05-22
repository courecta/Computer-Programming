/*
 * hw0402.c
 * Draws a BTC price chart (TWD) as a BMP, with gradient fill and colored line.
 * Usage: see --help
 */
#define _XOPEN_SOURCE 700
#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <getopt.h>
#include <curl/curl.h>
#include <math.h>

#define DATE_STRLEN 11  // "YYYY-MM-DD\0"

typedef struct {
    char *data;
    size_t size;
} Memory;

static size_t write_cb(void *ptr, size_t size, size_t nmemb, void *userp) {
    size_t total = size * nmemb;
    Memory *mem = (Memory*)userp;
    char *p = realloc(mem->data, mem->size + total + 1);
    if (!p) return 0;
    mem->data = p;
    memcpy(mem->data + mem->size, ptr, total);
    mem->size += total;
    mem->data[mem->size] = '\0';
    return total;
}

int parse_price(const char *json, double *out) {
    const char *p = strstr(json, "\"twd\"");
    if (!p) return -1;
    p = strchr(p, ':');
    if (!p) return -1;
    if (sscanf(p+1, "%lf", out) != 1) return -1;
    return 0;
}

void next_day(char *date) {
    struct tm tm = {0};
    strptime(date, "%Y-%m-%d", &tm);
    tm.tm_mday += 1;
    mktime(&tm);
    strftime(date, DATE_STRLEN, "%Y-%m-%d", &tm);
}

void days_ago(char *out, int days) {
    time_t t = time(NULL) - (time_t)days * 86400;
    struct tm tm = *gmtime(&t);
    strftime(out, DATE_STRLEN, "%Y-%m-%d", &tm);
}

#pragma pack(push,1)
typedef struct {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
} BITMAPFILEHEADER;

typedef struct {
    uint32_t biSize;
    int32_t  biWidth;
    int32_t  biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t  biXPelsPerMeter;
    int32_t  biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} BITMAPINFOHEADER;
#pragma pack(pop)

int main(int argc, char **argv) {
    char *outfn = "btc.bmp";
    int width = 800, height = 400;
    char start[DATE_STRLEN] = {0}, end[DATE_STRLEN] = {0};
    double minp = NAN, maxp = NAN;
    int have_min = 0, have_max = 0;
    double data_min = INFINITY, data_max = -INFINITY;

    struct option longopts[] = {
        {"output",     required_argument, 0, 'o'},
        {"width",      required_argument, 0, 'w'},
        {"height",     required_argument, 0, 'h'},
        {"start-time", required_argument, 0, 's'},
        {"end-time",   required_argument, 0, 'e'},
        {"min-price",  required_argument, 0, 'm'},
        {"max-price",  required_argument, 0, 'M'},
        {"help",       no_argument,       0, 'H'},
        {0,0,0,0}
    };

    int c, idx;
    while ((c = getopt_long(argc, argv, "o:w:h:s:e:m:M:H", longopts, &idx)) != -1) {
        if (optarg && optarg[0]=='=') {
            fprintf(stderr, "Invalid syntax for %s\n", longopts[idx].name);
            return 1;
        }
        switch(c) {
        case 'o': outfn = optarg; break;
        case 'w': width = atoi(optarg); break;
        case 'h': height = atoi(optarg); break;
        case 's': if (strlen(optarg)!=10) { fprintf(stderr,"Bad date\n"); return 1;} strcpy(start,optarg); break;
        case 'e': if (strlen(optarg)!=10) { fprintf(stderr,"Bad date\n"); return 1;} strcpy(end,optarg); break;
        case 'm': minp = atof(optarg); have_min = 1; break;
        case 'M': maxp = atof(optarg); have_max = 1; break;
        case 'H':
            printf("Usage: hw0402 [options]\n"
                   "\t-o, --output=filename   Set output (default: btc.bmp)\n"
                   "\t-w, --width=width       Set width (default: 800)\n"
                   "\t-h, --height=height     Set height (default: 400)\n"
                   "\t--start-time=YYYY-MM-DD   Set start date (default: 1 month ago)\n"
                   "\t--end-time=YYYY-MM-DD     Set end date (default: today)\n"
                   "\t--min-price=price      Default=min price in record\n"
                   "\t--max-price=price      Default=max price in record\n"
                   "\t-H, --help             Show help\n");
            return 0;
        default: return 1;
        }
    }

    if (!start[0]) days_ago(start,30);
    if (!end[0]) { time_t t=time(NULL); struct tm tm=*gmtime(&t); strftime(end,DATE_STRLEN,"%Y-%m-%d",&tm);}    
    if (strcmp(end,start)<0) { fprintf(stderr,"end < start\n"); return 1; }

    curl_global_init(CURL_GLOBAL_ALL);
    CURL *curl = curl_easy_init();
    if (!curl) { fprintf(stderr,"curl init failure\n"); return 1; }

    size_t cap=128, cnt=0;
    char (*dates)[DATE_STRLEN] = malloc(cap*DATE_STRLEN);
    double *prices = malloc(cap*sizeof(*prices));

    char cur[DATE_STRLEN]; strcpy(cur,start);
    while (strcmp(cur,end)<=0) {
        char url[256];
        snprintf(url,sizeof(url),
                 "https://cdn.jsdelivr.net/npm/@fawazahmed0/"
                 "currency-api@%s/v1/currencies/btc.json",cur);
        Memory m={.data=NULL,.size=0};
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &m);
        CURLcode res = curl_easy_perform(curl);
        long rc=0; curl_easy_getinfo(curl,CURLINFO_RESPONSE_CODE,&rc);
        if (res==CURLE_OK && rc==200 && m.data) {
            double p;
            if (parse_price(m.data,&p)==0) {
                if (cnt>=cap) {
                    cap*=2;
                    dates = realloc(dates, cap*DATE_STRLEN);
                    prices = realloc(prices, cap*sizeof(*prices));
                }
                strcpy(dates[cnt],cur);
                prices[cnt]=p;
                if (p<data_min) data_min=p;
                if (p>data_max) data_max=p;
                cnt++;
            }
        }
        free(m.data);
        next_day(cur);
    }
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    if (cnt==0) { fprintf(stderr,"No data fetched\n"); return 1; }
    if (!have_min) minp=data_min;
    if (!have_max) maxp=data_max;

    int do_draw = (cnt > 1 && minp < maxp);

    // Precompute pixel Y only if drawing
    int *iy = NULL;
    if (do_draw) {
        iy = malloc(cnt*sizeof(int));
        for (size_t i=0;i<cnt;i++) {
            double frac = (prices[i]-minp)/(maxp-minp);
            frac = fmin(fmax(frac,0.0),1.0);
            int y = (int)round(frac*(height-1));
            iy[i] = height-1-y;
        }
    }

    // Allocate white background
    int rowbytes = ((3*width+3)/4)*4;
    size_t imgsize = rowbytes*height;
    uint8_t *pix = malloc(imgsize);
    memset(pix, 0xFF, imgsize);

    if (do_draw) {
        // Smooth gradient fill
        uint8_t topR=0xC0, topG=0xE8, topB=0xE0;
        uint8_t botR=0xF8, botG=0xFC, botB=0xFC;
        for (int x=0;x<width;x++) {
            double xf = (double)x*(cnt-1)/(width-1);
            size_t i0 = (size_t)floor(xf);
            size_t i1 = (i0+1<cnt? i0+1 : i0);
            double w = xf - i0;
            double yf = iy[i0]*(1-w) + iy[i1]*w;
            int yc = (int)round(yf);
            for (int y=yc;y<height;y++) {
                double t = (double)(y-yc)/(height-1-yc);
                uint8_t R = topR + (botR-topR)*t;
                uint8_t G = topG + (botG-topG)*t;
                uint8_t B = topB + (botB-topB)*t;
                size_t r = (size_t)y*rowbytes + x*3;
                pix[r+0]=B; pix[r+1]=G; pix[r+2]=R;
            }
        }
        // Thick green line
        uint8_t lR=0x00, lG=0x80, lB=0x60;
        for (size_t i=1;i<cnt;i++) {
            int x0=(int)round((i-1)*(double)(width-1)/(cnt-1));
            int y0=iy[i-1];
            int x1=(int)round(i*(double)(width-1)/(cnt-1));
            int y1=iy[i];
            int dx=abs(x1-x0), sx=x0<x1?1:-1;
            int dy=-abs(y1-y0), sy=y0<y1?1:-1;
            int err=dx+dy;
            int x=x0, y=y0;
            while (1) {
                for (int dx2=-1;dx2<=1;dx2++) for (int dy2=-1;dy2<=1;dy2++) {
                    int xx=x+dx2, yy=y+dy2;
                    if (xx>=0&&xx<width&&yy>=0&&yy<height) {
                        size_t r=(size_t)yy*rowbytes + xx*3;
                        pix[r+0]=lB; pix[r+1]=lG; pix[r+2]=lR;
                    }
                }
                if (x==x1 && y==y1) break;
                int e2=2*err;
                if (e2>=dy) { err+=dy; x+=sx; }
                if (e2<=dx) { err+=dx; y+=sy; }
            }
        }
    }

    // Write BMP
    BITMAPFILEHEADER bfh = {0x4D42, sizeof(bfh)+sizeof(BITMAPINFOHEADER)+imgsize,0,0,
                             sizeof(bfh)+sizeof(BITMAPINFOHEADER)};
    BITMAPINFOHEADER bih = {sizeof(bih),width,height,1,24,0,(uint32_t)imgsize,
                             3780,3780,0,0};
    FILE *f = fopen(outfn,"wb");
    fwrite(&bfh,sizeof(bfh),1,f);
    fwrite(&bih,sizeof(bih),1,f);
    for (int y=0;y<height;y++) fwrite(pix + (height-1-y)*rowbytes,1,rowbytes,f);
    fclose(f);

    free(dates); free(prices); if (iy) free(iy); free(pix);
    return 0;
}
