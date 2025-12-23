#include "benchmark.h"
#include "queue.h"
#include "sort.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifndef BENCH_MAX_N
#define BENCH_MAX_N 40000
#endif

static bool load_numbers_prefix(const char* filename, int** out_values, size_t* out_count, size_t max_read) {
    if (!filename || !out_values || !out_count || max_read == 0) return false;

    FILE* f = fopen(filename, "r");
    if (!f) return false;

    int* values = (int*)malloc(max_read * sizeof(int));
    if (!values) {
        fclose(f);
        return false;
    }

    size_t count = 0;
    while (count < max_read) {
        int v;
        int r = fscanf(f, "%d", &v);
        if (r == 1) {
            values[count++] = v;
            continue;
        }
        if (r == EOF) break;
        int c = fgetc(f);
        if (c == EOF) break;
    }

    fclose(f);

    if (count == 0) {
        free(values);
        return false;
    }

    *out_values = values;
    *out_count = count;
    return true;
}

static bool queue_from_array(Queue* q, const int* a, size_t n) {
    if (!q || (!a && n != 0)) return false;
    init_queue(q);
    for (size_t i = 0; i < n; i++) {
        if (!enqueue(q, a[i])) {
            clear_queue(q);
            return false;
        }
    }
    return true;
}

static double measure_sort_seconds(void (*sort_fn)(Queue*), Queue* q) {
    clock_t start = clock();
    sort_fn(q);
    clock_t end = clock();
    return (double)(end - start) / (double)CLOCKS_PER_SEC;
}

bool run_benchmark(const char* input_filename, const char* output_filename) {
    if (!input_filename || !output_filename) return false;

    int* values = NULL;
    size_t total = 0;

    // читаем первые BENCH_MAX_N чисел из файла
    if (!load_numbers_prefix(input_filename, &values, &total, BENCH_MAX_N)) {
        return false;
    }

    if (total < 1000) {
        free(values);
        return false;
    }

    FILE* results = fopen(output_filename, "w");
    if (!results) {
        free(values);
        return false;
    }

    // n: 1000, 4000, 8000, потом +4000
    size_t n_list_first[] = {1000, 4000, 8000};
    size_t n_list_first_len = sizeof(n_list_first) / sizeof(n_list_first[0]);

    for (size_t i = 0; i < n_list_first_len; i++) {
        size_t n = n_list_first[i];
        if (n > total) break;

        Queue q_sel, q_qck;
        init_queue(&q_sel);
        init_queue(&q_qck);

        if (!queue_from_array(&q_sel, values, n) || !queue_from_array(&q_qck, values, n)) {
            clear_queue(&q_sel);
            clear_queue(&q_qck);
            fclose(results);
            free(values);
            return false;
        }

        double t_sel = measure_sort_seconds(selection_sort_queue, &q_sel);
        double t_qck = measure_sort_seconds(quicksort_queue, &q_qck);

        fprintf(results, "n=%zu\n", n);
        fprintf(results, "selection sort\n%.9f\n", t_sel);
        fprintf(results, "quick sort\n%.9f\n", t_qck);

        clear_queue(&q_sel);
        clear_queue(&q_qck);
    }

    for (size_t n = 12000; n <= total; n += 4000) {
        Queue q_sel, q_qck;
        init_queue(&q_sel);
        init_queue(&q_qck);

        if (!queue_from_array(&q_sel, values, n) || !queue_from_array(&q_qck, values, n)) {
            clear_queue(&q_sel);
            clear_queue(&q_qck);
            fclose(results);
            free(values);
            return false;
        }

        double t_sel = measure_sort_seconds(selection_sort_queue, &q_sel);
        double t_qck = measure_sort_seconds(quicksort_queue, &q_qck);

        fprintf(results, "n=%zu\n", n);
        fprintf(results, "selection sort\n%.9f\n", t_sel);
        fprintf(results, "quick sort\n%.9f\n", t_qck);

        clear_queue(&q_sel);
        clear_queue(&q_qck);
    }

    fclose(results);
    free(values);
    return true;
}
