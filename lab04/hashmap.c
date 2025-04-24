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


void insert(hashmap_t *map, const void *key, const void *value) {
    // TODO: Change to other hash function and observe differences
    unsigned int index = hash((const char *)key);
    node_t *new_node = (node_t *) malloc(sizeof(node_t));

    new_node->key = malloc(map->key_len);
    memcpy(new_node->key, key, map->key_len);

    new_node->value = malloc(map->val_len);
    memcpy(new_node->value, value, map->val_len);

    new_node->next = map->buckets[index];
    map->buckets[index] = new_node;
}

void *search(hashmap_t *map, const void *key) {
    // TODO: Change to other hash function and observe differences
    unsigned int index = hash((const char *)key);
    node_t *node = map->buckets[index];

    while (node) {
        if (map->eq_func(key, node->key, map->key_len)) {
            return node->value;
        }
        node = node->next;
    }
    return NULL; // Not found
}

bool comp(const void *k1, const void *k2, unsigned int size) {
    return memcmp(k1, k2, size) == 0;
}

void init_map(hashmap_t *map, eq_func_t func, unsigned int key_len,
              unsigned int val_len) {
    if (func == NULL) {
        map->eq_func = comp;
    } else {
        map->eq_func = func;
    }
    map->key_len = key_len;
    map->val_len = val_len;
}
