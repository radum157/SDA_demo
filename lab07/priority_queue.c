#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "priority_queue.h"

void swap(void *a, void *b, size_t data_size) {
    void *temp = malloc(data_size);
    memcpy(temp, a, data_size);
    memcpy(a, b, data_size);
    memcpy(b, temp, data_size);
    free(temp);
}

priority_queue_t *pq_create(bool (*cmp_f)(const void *, const void *), size_t data_size) {
    priority_queue_t *pq = malloc(sizeof(*pq));
    *pq = (priority_queue_t){
        .arr = malloc(5 * data_size),
        .capacity = 5,
        .data_size = data_size,
        .size = 0,
        .cmp = cmp_f
    };
    return pq;
}

void pq_resize(priority_queue_t *pq) {
    pq->capacity *= 2;
    pq->arr = realloc(pq->arr, pq->capacity * pq->data_size);
}

// Heapify up
void heapify_up(priority_queue_t *pq, size_t index) {
    if (index == 0) return;
    size_t parent = (index - 1) / 2;
    void *curr = (char *)pq->arr + index * pq->data_size;
    void *par = (char *)pq->arr + parent * pq->data_size;
    if (pq->cmp(curr, par)) {
        swap(curr, par, pq->data_size);
        heapify_up(pq, parent);
    }
}

// Heapify down
void heapify_down(priority_queue_t *pq, size_t index) {
    size_t left = 2 * index + 1;
    size_t right = 2 * index + 2;
    size_t smallest = index;

    void *base = pq->arr;
    void *curr = (char *)base + index * pq->data_size;

    if (left < pq->size) {
        void *left_ptr = (char *)base + left * pq->data_size;
        if (pq->cmp(left_ptr, curr)) smallest = left;
    }

    if (right < pq->size) {
        void *right_ptr = (char *)base + right * pq->data_size;
        void *smallest_ptr = (char *)base + smallest * pq->data_size;
        if (pq->cmp(right_ptr, smallest_ptr)) smallest = right;
    }

    if (smallest != index) {
        void *smallest_ptr = (char *)base + smallest * pq->data_size;
        swap(curr, smallest_ptr, pq->data_size);
        heapify_down(pq, smallest);
    }
}

void pq_push(priority_queue_t *pq, void *element) {
    if (pq->size == pq->capacity) pq_resize(pq);
    void *dest = (char *)pq->arr + pq->size * pq->data_size;
    memcpy(dest, element, pq->data_size);
    heapify_up(pq, pq->size);
    pq->size++;
}

void pq_pop(priority_queue_t *pq) {
    if (pq->size == 0) return;
    void *first = pq->arr;
    void *last = (char *)pq->arr + (pq->size - 1) * pq->data_size;
    memcpy(first, last, pq->data_size);
    pq->size--;
    heapify_down(pq, 0);
}

void *pq_peek(priority_queue_t *pq) {
    if (pq->size == 0) return NULL;
    return pq->arr;
}

void pq_destroy(priority_queue_t *pq) {
    free(pq->arr);
    free(pq);
}

bool is_empty(priority_queue_t *pq) {
    return pq->size == 0;
}
