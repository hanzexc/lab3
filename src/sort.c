#include "sort.h"

/* -------------------- SORT 1 -------------------- */
void selection_sort_queue(Queue* q) {
    if (!q || !q->BegQ || !q->BegQ->next) return;

    elem* unsorted = q->BegQ;
    elem* sorted_h = NULL;
    elem* sorted_t = NULL;

    while (unsorted != NULL) {
        elem* min_prev = NULL;
        elem* min_node = unsorted;

        elem* prev = unsorted;
        elem* cur  = unsorted->next;

        while (cur != NULL) {
            if (cur->data < min_node->data) {
                min_node = cur;
                min_prev = prev;
            }
            prev = cur;
            cur = cur->next;
        }

        if (min_prev == NULL) {
            unsorted = unsorted->next;
        } else {
            min_prev->next = min_node->next;
        }
        min_node->next = NULL;

        if (sorted_h == NULL) {
            sorted_h = min_node;
            sorted_t = min_node;
        } else {
            sorted_t->next = min_node;
            sorted_t = min_node;
        }
    }

    q->BegQ = sorted_h;
    q->EndQ = sorted_t;
}

/* -------------------- QuickSort -------------------- */

static void append_node(elem** h, elem** t, elem* node) {
    node->next = NULL;
    if (*h == NULL) {
        *h = node;
        *t = node;
    } else {
        (*t)->next = node;
        *t = node;
    }
}

static elem* quicksort_list(elem* head, elem** out_tail) {
    if (head == NULL || head->next == NULL) {
        if (out_tail) *out_tail = head;
        return head;
    }

    int pivot = head->data;

    elem *less_h = NULL,  *less_t = NULL;
    elem *eq_h   = NULL,  *eq_t   = NULL;
    elem *gt_h   = NULL,  *gt_t   = NULL;

    elem* cur = head;
    while (cur != NULL) {
        elem* next = cur->next;
        cur->next = NULL;

        if (cur->data < pivot) {
            append_node(&less_h, &less_t, cur);
        } else if (cur->data > pivot) {
            append_node(&gt_h, &gt_t, cur);
        } else {
            append_node(&eq_h, &eq_t, cur);
        }
        cur = next;
    }

    elem *less_sorted_t = NULL, *gt_sorted_t = NULL;
    if (less_h) less_h = quicksort_list(less_h, &less_sorted_t);
    if (gt_h)   gt_h   = quicksort_list(gt_h, &gt_sorted_t);

    elem* res_h = NULL;
    elem* res_t = NULL;

    if (less_h) {
        res_h = less_h;
        res_t = less_sorted_t;
    }

    if (eq_h) {
        if (!res_h) {
            res_h = eq_h;
            res_t = eq_t;
        } else {
            res_t->next = eq_h;
            res_t = eq_t;
        }
    }

    if (gt_h) {
        if (!res_h) {
            res_h = gt_h;
            res_t = gt_sorted_t;
        } else {
            res_t->next = gt_h;
            res_t = gt_sorted_t;
        }
    }

    if (out_tail) *out_tail = res_t;
    return res_h;
}

void quicksort_queue(Queue* q) {
    if (!q || !q->BegQ || !q->BegQ->next) return;

    elem* tail = NULL;
    q->BegQ = quicksort_list(q->BegQ, &tail);
    q->EndQ = tail;
}
