#include "file_io.h"
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>

void clean_input_buffer(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}
void chomp(char* s) {
    if (!s) return;
    char* p = s;
    while (*p) p++;
    if (p != s && p[-1] == '\n') p[-1] = '\0';
}

int read_line(FILE* f, char** out_line) {
    if (!f || !out_line) return 0;

    size_t cap = 128;
    size_t len = 0;
    char* buf = (char*)malloc(cap);
    if (!buf) return 0;

    int c;
    while ((c = fgetc(f)) != EOF) {
        if (len + 1 >= cap) {
            cap *= 2;
            char* nb = (char*)realloc(buf, cap);
            if (!nb) {
                free(buf);
                return 0;
            }
            buf = nb;
        }
        buf[len++] = (char)c;
        if (c == '\n') break;
    }

    if (len == 0 && c == EOF) {
        free(buf);
        return 0;
    }

    buf[len] = '\0';
    *out_line = buf;
    return 1;
}

int queue_from_line(Queue* q, const char* line) {
    if (!q || !line) return -1;
    int count = 0;
    const char* p = line;

    while (*p) {
        while (*p && isspace((unsigned char)*p)) p++;
        if (!*p || *p == '\n') break;

        errno = 0;
        char* end = NULL;
        long v = strtol(p, &end, 10);

        if (end == p) {
            clear_queue(q);
            return -1;
        }
        if (*end && !isspace((unsigned char)*end)) {
            clear_queue(q);
            return -1;
        }

        if (errno == ERANGE || v < INT_MIN || v > INT_MAX) {
            clear_queue(q);
            return -1;
        }

        if (!enqueue(q, (int)v)) {
            clear_queue(q);
            return -1;
        }
        count++;
        p = end;
    }

    return (count > 0) ? count : -1;
}

void write_queue_line(FILE* f, const Queue* q) {
    if (!f || !q) return;

    elem* cur = q->BegQ;
    while (cur) {
        fprintf(f, "%d", cur->data);
        if (cur->next) fputc(' ', f);
        cur = cur->next;
    }
    fputc('\n', f);
}
