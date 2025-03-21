#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

inline unsigned int hash(const char *key) {
    unsigned int hash = 0;
    while (*key) {
        hash = (hash * 31) + *key++;
    }
    return hash % TABLE_SIZE;
}

inline unsigned int hash2(const char *key) {
    return ((key[0] << 10) + (key[1] << 15) +
            (key[2] << 20)) % TABLE_SIZE;
}

inline unsigned int
hash3(const char *str)
{
    unsigned long hash = 5381;

    while (*str) {
        hash = ((hash << 5) + hash) + *str++; /* hash * 33 + c */
    }

    return hash % TABLE_SIZE;
}

void insert(hashmap_t *map, const char *key, int value) {
    // TODO: Change to other hash function and observe differences
    unsigned int index = hash3(key);
    node_t *newNode = (node_t *) malloc(sizeof(node_t));

    newNode->key = strdup(key);
    newNode->value = value;
    newNode->next = map->buckets[index];
    map->buckets[index] = newNode;
}

int search(hashmap_t *map, const char *key) {
    // TODO: Change to other hash function and observe differences
    unsigned int index = hash3(key);
    node_t *node = map->buckets[index];

    while (node) {
        if (strcmp(node->key, key) == 0) {
            return node->value;
        }
        node = node->next;
    }
    return -1; // Not found
}
