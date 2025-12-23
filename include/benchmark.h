#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <stdbool.h>

// Запускает бенчмарк и сохраняет результаты в output_file
bool run_benchmark(const char* input_filename, const char* output_filename);

#endif