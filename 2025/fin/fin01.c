/*
 * fin01.c
 *
 * Pipeline:
 * 1) Download PDF using libcurl
 * 2) Convert PDF to text using pdftotext (requires Poppler utils)
 * 3) Parse the text output to extract a table of state data
 * 4) Offer menu to sort and display
 *
 * Build:
 *   gcc -o fin01 fin01.c -lcurl
 *
 * Requirements:
 *   - libcurl-dev
 *   - poppler-utils (for pdftotext)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

typedef struct {
    char state[64];
    int start_salary;
    int avg_salary;
    int min_wage;
    int per_student_spending;
    int he_earn;
    int he_fac_salary;
} StateData;

#define MAX_STATES 60
static StateData states[MAX_STATES];
static int num_states = 0;

// libcurl write callback
static size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    return fwrite(ptr, size, nmemb, stream);
}

// Download the PDF
void download_pdf(const char *url, const char *outfile) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Failed to init curl\n");
        exit(1);
    }
    FILE *fp = fopen(outfile, "wb");
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    fclose(fp);
    if (res != CURLE_OK) {
        fprintf(stderr, "Error downloading PDF: %s\n", curl_easy_strerror(res));
        exit(1);
    }
    printf("PDF downloaded to %s\n", outfile);
}

// Convert PDF to text
void pdf_to_text(const char *pdf, const char *txt) {
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "pdftotext -layout %s %s", pdf, txt);
    printf("Running: %s\n", cmd);
    int rc = system(cmd);
    if (rc != 0) {
        fprintf(stderr, "pdftotext conversion failed\n");
        exit(1);
    }
}

// Parse the text file output
void parse_text(const char *txtfile) {
    FILE *fp = fopen(txtfile, "r");
    if (!fp) { perror("fopen"); exit(1); }
    char line[256];
    int in_table = 0;
    num_states = 0;
    while (fgets(line, sizeof(line), fp)) {
        // Trim newline
        line[strcspn(line, "\r\n")] = 0;
        if (!in_table) {
            // detect table header line containing "State" and "Starting"
            if (strstr(line, "State") && strstr(line, "Starting")) {
                in_table = 1;
                // skip header underline
                fgets(line, sizeof(line), fp);
            }
        } else {
            if (strlen(line) < 10) break; // end of table
            // Expect fixed-width columns, parse by substrings
            char state_name[64];
            char buf[16];
            // columns: State [0-20], StartSalary [20-30], AvgSalary [30-40], MinWage [40-50], PerStudent [50-60], HEearn [60-70], HEfac [70-80]
            strncpy(state_name, line, 20); state_name[20] = 0;
            // cleanup
            char *s = state_name;
            while (*s == ' ') s++;
            strcpy(states[num_states].state, s);
            // parse ints
            strncpy(buf, line+20, 10); buf[10]=0; states[num_states].start_salary = atoi(buf);
            strncpy(buf, line+30, 10); buf[10]=0; states[num_states].avg_salary   = atoi(buf);
            strncpy(buf, line+40, 10); buf[10]=0; states[num_states].min_wage      = atoi(buf);
            strncpy(buf, line+50, 10); buf[10]=0; states[num_states].per_student_spending = atoi(buf);
            strncpy(buf, line+60, 10); buf[10]=0; states[num_states].he_earn       = atoi(buf);
            strncpy(buf, line+70, 10); buf[10]=0; states[num_states].he_fac_salary = atoi(buf);
            num_states++;
        }
    }
    fclose(fp);
    printf("Parsed %d states\n", num_states);
}

// Comparator functions for all 14 menu options
int cmp_start_asc(const void *a, const void *b) {
    return ((StateData*)a)->start_salary - ((StateData*)b)->start_salary;
}
int cmp_start_desc(const void *a, const void *b) {
    return ((StateData*)b)->start_salary - ((StateData*)a)->start_salary;
}
int cmp_avg_asc(const void *a, const void *b) {
    return ((StateData*)a)->avg_salary - ((StateData*)b)->avg_salary;
}
int cmp_avg_desc(const void *a, const void *b) {
    return ((StateData*)b)->avg_salary - ((StateData*)a)->avg_salary;
}
int cmp_start_min_gap_asc(const void *a, const void *b) {
    int gap_a = ((StateData*)a)->start_salary - ((StateData*)a)->min_wage;
    int gap_b = ((StateData*)b)->start_salary - ((StateData*)b)->min_wage;
    return gap_a - gap_b;
}
int cmp_start_min_gap_desc(const void *a, const void *b) {
    int gap_a = ((StateData*)a)->start_salary - ((StateData*)a)->min_wage;
    int gap_b = ((StateData*)b)->start_salary - ((StateData*)b)->min_wage;
    return gap_b - gap_a;
}
int cmp_avg_min_gap_asc(const void *a, const void *b) {
    int gap_a = ((StateData*)a)->avg_salary - ((StateData*)a)->min_wage;
    int gap_b = ((StateData*)b)->avg_salary - ((StateData*)b)->min_wage;
    return gap_a - gap_b;
}
int cmp_avg_min_gap_desc(const void *a, const void *b) {
    int gap_a = ((StateData*)a)->avg_salary - ((StateData*)a)->min_wage;
    int gap_b = ((StateData*)b)->avg_salary - ((StateData*)b)->min_wage;
    return gap_b - gap_a;
}
int cmp_per_student_asc(const void *a, const void *b) {
    return ((StateData*)a)->per_student_spending - ((StateData*)b)->per_student_spending;
}
int cmp_per_student_desc(const void *a, const void *b) {
    return ((StateData*)b)->per_student_spending - ((StateData*)a)->per_student_spending;
}
int cmp_start_he_gap_asc(const void *a, const void *b) {
    int gap_a = ((StateData*)a)->start_salary - ((StateData*)a)->he_earn;
    int gap_b = ((StateData*)b)->start_salary - ((StateData*)b)->he_earn;
    return gap_a - gap_b;
}
int cmp_start_he_gap_desc(const void *a, const void *b) {
    int gap_a = ((StateData*)a)->start_salary - ((StateData*)a)->he_earn;
    int gap_b = ((StateData*)b)->start_salary - ((StateData*)b)->he_earn;
    return gap_b - gap_a;
}
int cmp_avg_hefac_gap_asc(const void *a, const void *b) {
    int gap_a = ((StateData*)a)->avg_salary - ((StateData*)a)->he_fac_salary;
    int gap_b = ((StateData*)b)->avg_salary - ((StateData*)b)->he_fac_salary;
    return gap_a - gap_b;
}
int cmp_avg_hefac_gap_desc(const void *a, const void *b) {
    int gap_a = ((StateData*)a)->avg_salary - ((StateData*)a)->he_fac_salary;
    int gap_b = ((StateData*)b)->avg_salary - ((StateData*)b)->he_fac_salary;
    return gap_b - gap_a;
}

// Display top results with appropriate data based on sort type
void display_states(int limit, int sort_type) {
    for (int i = 0; i < limit && i < num_states; i++) {
        printf("%2d. %-20s", i+1, states[i].state);
        
        switch (sort_type) {
            case 1: case 2: // Starting salary sorts
                printf(" %6d\n", states[i].start_salary);
                break;
            case 3: case 4: // Average salary sorts
                printf(" %6d\n", states[i].avg_salary);
                break;
            case 5: case 6: // Starting salary vs minimum wage gap
                printf(" %6d\n", states[i].start_salary - states[i].min_wage);
                break;
            case 7: case 8: // Average salary vs minimum wage gap
                printf(" %6d\n", states[i].avg_salary - states[i].min_wage);
                break;
            case 9: case 10: // Per student spending
                printf(" %6d\n", states[i].per_student_spending);
                break;
            case 11: case 12: // Starting salary vs HE ESP earnings gap
                printf(" %6d\n", states[i].start_salary - states[i].he_earn);
                break;
            case 13: case 14: // Average salary vs HE faculty salary gap
                printf(" %6d\n", states[i].avg_salary - states[i].he_fac_salary);
                break;
            default:
                printf(" %6d  %6d  %6d\n", states[i].start_salary, states[i].avg_salary, states[i].min_wage);
        }
    }
}

int main() {
    const char *pdf_url = "https://www.nea.org/sites/default/files/2025-04/2025_rankings_and_estimates_report.pdf";
    download_pdf(pdf_url, "report.pdf");
    pdf_to_text("report.pdf", "report.txt");
    parse_text("report.txt");

    int choice;
    do {
        printf("---\n");
        printf("1) Sort states in the ascending order based on \"Average Teacher Starting Salary\"\n");
        printf("2) Sort states in the descending order based on \"Average Teacher Starting Salary\"\n");
        printf("3) Sort states in the ascending order based on \"Average Teacher Salary\"\n");
        printf("4) Sort states in the descending order based on \"Average Teacher Salary\"\n");
        printf("5) Sort states in the ascending order based on gap between \"Average Starting Teacher Salary\" and \"Minimum Living Wage\"\n");
        printf("6) Sort states in the descending order based on gap between \"Average Starting Teacher Salary\" and \"Minimum Living Wage\"\n");
        printf("7) Sort states in the ascending order based on gap between \"Average Teacher Salary\" and \"Minimum Living Wage\"\n");
        printf("8) Sort states in the descending order based on gap between \"Average Teacher Salary\" and \"Minimum Living Wage\"\n");
        printf("9) Sort states in the ascending order based on \"Per Student Spending\"\n");
        printf("10) Sort states in the descending order based on \"Per Student Spending\"\n");
        printf("11) Sort states in the ascending order based on gap between \"Average Starting Teacher Salary\" and \"Average HE ESP Earnings\"\n");
        printf("12) Sort states in the descending order based on gap between \"Average Starting Teacher Salary\" and \"Average HE ESP Earnings\"\n");
        printf("13) Sort states in the ascending order based on gap between \"Average Teacher Salary\" and \"Average Higher Ed Faculty Salary\"\n");
        printf("14) Sort states in the descending order based on gap between \"Average Teacher Salary\" and \"Average Higher Ed Faculty Salary\"\n");
        printf("15) Exit\n");
        printf("---\n");
        printf("Please enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                qsort(states, num_states, sizeof(StateData), cmp_start_asc);
                display_states(10, choice);
                break;
            case 2:
                qsort(states, num_states, sizeof(StateData), cmp_start_desc);
                display_states(10, choice);
                break;
            case 3:
                qsort(states, num_states, sizeof(StateData), cmp_avg_asc);
                display_states(10, choice);
                break;
            case 4:
                qsort(states, num_states, sizeof(StateData), cmp_avg_desc);
                display_states(10, choice);
                break;
            case 5:
                qsort(states, num_states, sizeof(StateData), cmp_start_min_gap_asc);
                display_states(10, choice);
                break;
            case 6:
                qsort(states, num_states, sizeof(StateData), cmp_start_min_gap_desc);
                display_states(10, choice);
                break;
            case 7:
                qsort(states, num_states, sizeof(StateData), cmp_avg_min_gap_asc);
                display_states(10, choice);
                break;
            case 8:
                qsort(states, num_states, sizeof(StateData), cmp_avg_min_gap_desc);
                display_states(10, choice);
                break;
            case 9:
                qsort(states, num_states, sizeof(StateData), cmp_per_student_asc);
                display_states(10, choice);
                break;
            case 10:
                qsort(states, num_states, sizeof(StateData), cmp_per_student_desc);
                display_states(10, choice);
                break;
            case 11:
                qsort(states, num_states, sizeof(StateData), cmp_start_he_gap_asc);
                display_states(10, choice);
                break;
            case 12:
                qsort(states, num_states, sizeof(StateData), cmp_start_he_gap_desc);
                display_states(10, choice);
                break;
            case 13:
                qsort(states, num_states, sizeof(StateData), cmp_avg_hefac_gap_asc);
                display_states(10, choice);
                break;
            case 14:
                qsort(states, num_states, sizeof(StateData), cmp_avg_hefac_gap_desc);
                display_states(10, choice);
                break;
            case 15:
                printf("Goodbye!\n");
                break;
            default:
                printf("Not implemented yet.\n");
        }
    } while (choice != 15);

    return 0;
}