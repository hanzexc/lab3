#ifndef FILE_IO_H
#define FILE_IO_H

#include <stdio.h>
#include "queue.h"

void clean_input_buffer();
int read_line(FILE* f, char** out_line);
int queue_from_line(Queue* q, const char* line);
void write_queue_line(FILE* f, const Queue* q);
void chomp(char* s);

#endif
