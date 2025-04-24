#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    void *arr;
    size_t size;
    size_t capacity;
    size_t data_size;
    bool (*cmp)(const void *, const void *);
} priority_queue_t;

priority_queue_t *pq_create(bool (*cmp_f)(const void *, const void *), size_t data_size);

void pq_resize(priority_queue_t *pq);

void pq_push(priority_queue_t *pq, void *element);

void pq_pop(priority_queue_t *pq);

void *pq_peek(priority_queue_t *pq);

void pq_destroy(priority_queue_t *pq);

bool is_empty(priority_queue_t *pq);

#ifdef __cplusplus
}
#endif
