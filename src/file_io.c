#include "file_io.h"
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>

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
    if (!q || !line) return 0;

    int count = 0;
    const char* p = line;

    while (*p) {
        while (*p && !isdigit((unsigned char)*p) && *p != '-' && *p != '+') p++;
        if (!*p) break;

        errno = 0;
        char* end = NULL;
        long v = strtol(p, &end, 10);
        if (end == p) { p++; continue; }
        if (errno == ERANGE) { p = end; continue; }

        if (!enqueue(q, (int)v)) break;
        count++;

        p = end;
    }

    return count;
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
