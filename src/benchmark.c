#include "benchmark.h"
#include "queue.h"
#include "sort.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#ifndef BENCH_MAX_N
#define BENCH_MAX_N 65536
#endif

#ifndef BENCH_START_N
#define BENCH_START_N 2048
#endif

static bool load_prefix_queue(Queue* q, const char* filename, size_t max_n) {
    if (!q || !filename || max_n == 0) return false;

    FILE* f = fopen(filename, "r");
    if (!f) return false;

    init_queue(q);

    size_t count = 0;
    while (count < max_n) {
        int v;
        int r = fscanf(f, "%d", &v);
        if (r == 1) {
            if (!enqueue(q, v)) {
                fclose(f);
                clear_queue(q);
                return false;
            }
            count++;
            continue;
        }
        if (r == EOF) break;

        int c = fgetc(f);
        if (c == EOF) break;
    }

    fclose(f);
    return (count > 0);
}

bool run_benchmark(const char* input_filename, const char* output_filename) {
    if (!input_filename || !output_filename) return false;

    Queue full_data;
    init_queue(&full_data);

    if (!load_prefix_queue(&full_data, input_filename, BENCH_MAX_N)) {
        clear_queue(&full_data);
        return false;
    }

    size_t total = queue_size(&full_data);
    if (total < BENCH_START_N) {
        clear_queue(&full_data);
        return false;
    }

    FILE* results = fopen(output_filename, "w");
    if (!results) {
        clear_queue(&full_data);
        return false;
    }

    fprintf(results, "n selection sort quick sort\n");
    printf("n selection sort quick sort\n");

    for (size_t n = BENCH_START_N; n <= total; ) {
        Queue test_q;
        init_queue(&test_q);

        elem* cur = full_data.BegQ;
        for (size_t i = 0; i < n && cur; i++) {
            if (!enqueue(&test_q, cur->data)) {
                clear_queue(&test_q);
                clear_queue(&full_data);
                fclose(results);
                return false;
            }
            cur = cur->next;
        }
        Queue copy1;
        if (!copy_queue(&copy1, &test_q)) {
            clear_queue(&test_q);
            clear_queue(&full_data);
            fclose(results);
            return false;
        }

        clock_t start_sel = clock();
        selection_sort_queue(&copy1);
        clock_t end_sel = clock();
        double sel_time = (double)(end_sel - start_sel) / CLOCKS_PER_SEC;

        Queue copy2;
        if (!copy_queue(&copy2, &test_q)) {
            clear_queue(&copy1);
            clear_queue(&test_q);
            clear_queue(&full_data);
            fclose(results);
            return false;
        }

        clock_t start_qck = clock();
        quicksort_queue(&copy2);
        clock_t end_qck = clock();
        double qck_time = (double)(end_qck - start_qck) / CLOCKS_PER_SEC;
        fprintf(results, "%zu %.9f %.9f\n", n, sel_time, qck_time);
        printf("%zu %.9f %.9f\n", n, sel_time, qck_time);

        clear_queue(&copy1);
        clear_queue(&copy2);
        clear_queue(&test_q);
        
        if (n > total / 2) break;
        if (n > (size_t)-1 / 2) break;
        n *= 2;
    }

    clear_queue(&full_data);
    fclose(results);
    return true;
}
