#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"
#include "sort.h"
#include "file_io.h"
#include "benchmark.h"

int main(int argc, char* argv[]) {
    if (argc == 4 && strcmp(argv[1], "--benchmark") == 0) {
        return run_benchmark(argv[2], argv[3]) ? 0 : 1;
    }

    const char* filename = NULL;

    if (argc == 3 && strcmp(argv[1], "--file") == 0) {
        filename = argv[2];

        FILE* read_file = fopen(filename, "r");
        if (read_file) {
            char *prev_orig = NULL, *prev_sorted = NULL;

            if (read_line(read_file, &prev_orig) && read_line(read_file, &prev_sorted)) {
                chomp(prev_orig);
                chomp(prev_sorted);
                printf("%s\n", prev_orig);
                printf("%s\n", prev_sorted);
            }

            free(prev_orig);
            free(prev_sorted);
            fclose(read_file);
        }
    }

    char* input = NULL;
    if (!read_line(stdin, &input)) return 0;

    Queue q, sorted;
    init_queue(&q);

    queue_from_line(&q, input);
    free(input);

    if (!copy_queue(&sorted, &q)) {
        clear_queue(&q);
        return 0;
    }

    selection_sort_queue(&sorted);

    write_queue_line(stdout, &q);
    write_queue_line(stdout, &sorted);


    if (filename) {
        FILE* write_file = fopen(filename, "w");
        if (write_file) {
            write_queue_line(write_file, &q);
            write_queue_line(write_file, &sorted);
            fclose(write_file);
        }
    }

    clear_queue(&q);
    clear_queue(&sorted);
    return 0;
}