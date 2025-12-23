#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>   // size_t

typedef struct elem {
    int data;
    struct elem* next;
} elem;

typedef struct Queue {  
    elem* BegQ;
    elem* EndQ;
    size_t size;
} Queue;

void   init_queue(Queue* q);
int    enqueue(Queue* q, int value);
int    dequeue(Queue* q, int* out_value);

int    is_empty(const Queue* q);
size_t queue_size(const Queue* q);
void   clear_queue(Queue* q);

int    get_at(const Queue* q, size_t index, int* out_value);
int    set_at(Queue* q, size_t index, int value);

int    copy_queue(Queue* dest, const Queue* src);
void   print_queue(const Queue* q);

#endif