#include "queue.h"
#include <stdio.h>

void init_queue(Queue* q) {
    if (!q) return;
    q->BegQ = NULL;
    q->EndQ = NULL;
    q->size = 0;
}

int enqueue(Queue* q, int value) {
    if (!q) return 0;

    elem* e = (elem*)malloc(sizeof(elem));
    if (!e) return 0;

    e->data = value;
    e->next = NULL;

    if (q->EndQ == NULL) {
        q->BegQ = e;
        q->EndQ = e;
    } else {
        q->EndQ->next = e;
        q->EndQ = e;
    }

    q->size++;
    return 1;
}

int dequeue(Queue* q, int* out_value) {
    if (!q || q->BegQ == NULL) return 0;

    elem* first = q->BegQ;
    if (out_value) *out_value = first->data;

    q->BegQ = first->next;
    if (q->BegQ == NULL) q->EndQ = NULL;

    free(first);
    q->size--;
    return 1;
}

int is_empty(const Queue* q) {
    return (!q || q->BegQ == NULL);
}

size_t queue_size(const Queue* q) {
    return q ? q->size : 0;
}

void clear_queue(Queue* q) {
    if (!q) return;
    while (!is_empty(q)) {
        dequeue(q, NULL);
    }
}

int get_at(const Queue* q, size_t index, int* out_value) {
    if (!q || !out_value) return 0;
    if (index >= q->size) return 0;

    elem* cur = q->BegQ;
    for (size_t i = 0; i < index; i++) {
        cur = cur->next;
    }

    *out_value = cur->data;
    return 1;
}

int set_at(Queue* q, size_t index, int value) {
    if (!q) return 0;
    if (index >= q->size) return 0;

    elem* cur = q->BegQ;
    for (size_t i = 0; i < index; i++) {
        cur = cur->next;
    }

    cur->data = value;
    return 1;
}

int copy_queue(Queue* dest, const Queue* src) {
    if (!dest || !src) return 0;

    init_queue(dest);
    for (elem* cur = src->BegQ; cur != NULL; cur = cur->next) {
        if (!enqueue(dest, cur->data)) {
            clear_queue(dest);
            return 0;
        }
    }
    return 1;
}

void print_queue(const Queue* q) {
    if (!q) {
        printf("(null)\n");
        return;
    }

    printf("[ ");
    for (elem* cur = q->BegQ; cur != NULL; cur = cur->next) {
        printf("%d ", cur->data);
    }
    printf("] (size=%zu)\n", q->size);
}
