#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 1000000

typedef bool (*eq_func_t)(const void *k1, const void *k2,
                            unsigned int key_size);

typedef struct Node {
    void *key;
    void *value;

    struct Node *next;

} node_t;

typedef struct hashmap {
    node_t *buckets[TABLE_SIZE];
    unsigned int key_len;
    unsigned int val_len;

    eq_func_t eq_func;
} hashmap_t;

unsigned int hash(const char *key);

void init_map(hashmap_t *map, eq_func_t,
              unsigned int key_len, unsigned int val_len);

void insert(hashmap_t *map, const void *key, const void *value);

void *search(hashmap_t *map, const void *key);

void display(hashmap_t *map);

