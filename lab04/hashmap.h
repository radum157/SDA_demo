#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 1000000

typedef struct Node {
    char *key;
    int value;
    struct Node *next;
} node_t;

typedef struct hashmap {
    node_t *buckets[TABLE_SIZE];
} hashmap_t;

unsigned int hash(const char *key);

void insert(hashmap_t *map, const char *key, int value);

int search(hashmap_t *map, const char *key);

void display(hashmap_t *map);

