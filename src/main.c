#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"
#include "sort.h"
#include "file_io.h"
#include "benchmark.h"

static int prompt_finish_continue(void) {
    int choice = 0;
    for (;;) {
        puts("Введите 1, если хотите завершить, 0 - если хотите продолжить.");
        if (scanf("%d", &choice) != 1) {
            puts("Ошибка ввода");
            clean_input_buffer();
            continue;
        }
        clean_input_buffer();
        if (choice == 0 || choice == 1) {
            return choice;
        }
        puts("Ошибка ввода");
    }
}

int main(int argc, char* argv[]) {
    if (argc == 4 && strcmp(argv[1], "--benchmark") == 0) {
        return run_benchmark(argv[2], argv[3]) ? 0 : 1;
    }

    const char* filename = NULL;
    int file_mode = 0;

    if (argc == 3 && strcmp(argv[1], "--file") == 0) {
        filename = argv[2];
        file_mode = 1;
    }

    for (;;) {
        if (file_mode && filename) {
            FILE* read_file = fopen(filename, "r");
            if (read_file) {
                char *prev_orig = NULL, *prev_sorted = NULL;

                if (read_line(read_file, &prev_orig) && read_line(read_file, &prev_sorted)) {
                    chomp(prev_orig);
                    chomp(prev_sorted);

                    puts("Несортированная очередь (прошлая):");
                    printf("%s\n", prev_orig);

                    puts("Сортированная очередь (прошлая):");
                    printf("%s\n", prev_sorted);
                }

                free(prev_orig);
                free(prev_sorted);
                fclose(read_file);
            }
        }

        Queue q, sorted;
        init_queue(&q);

        for (;;) {
            puts("Введите новую очередь:");
            char* input = NULL;
            if (!read_line(stdin, &input)) {
                puts("Программа завершена.");
                clear_queue(&q);
                return 0;
            }

            int parsed = queue_from_line(&q, input);
            free(input);

            if (parsed < 0) {
                clear_queue(&q);
                puts("Ошибка ввода");

                int choice = prompt_finish_continue();
                puts("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");
                if (choice == 1) {
                    puts("Программа завершена.");
                    return 0;
                }
                continue;
            }

            break;
        }

        if (!copy_queue(&sorted, &q)) {
            clear_queue(&q);
            if (!file_mode) {
                puts("Программа завершена.");
                break;
            }
            continue;
        }

        selection_sort_queue(&sorted);

        puts("Несортированная очередь (новая):");
        write_queue_line(stdout, &q);

        puts("Сортированная очередь (новая):");
        write_queue_line(stdout, &sorted);

        if (file_mode && filename) {
            FILE* write_file = fopen(filename, "w");
            if (write_file) {
                write_queue_line(write_file, &q);
                write_queue_line(write_file, &sorted);
                fclose(write_file);
            }
        }

        clear_queue(&q);
        clear_queue(&sorted);

        if (!file_mode) {
            puts("Программа завершена.");
            break;
        }

        int choice = prompt_finish_continue();

        puts("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");

        if (choice == 1) {
            puts("Программа завершена.");
            break;
        }
    }

    return 0;
}
