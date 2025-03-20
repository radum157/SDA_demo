#pragma once

#include <stdlib.h>

typedef struct node_t node_t;

struct node_t {
	void *data;

	node_t *next;
	node_t *prev;
};

typedef struct list_t {
	node_t *head;
	node_t *tail;

	size_t dataSize;
	size_t size;

	void (*freeData)(void *);
} list_t;

list_t createList(size_t dataSize);
list_t *newList(size_t dataSize);

/**
 * @brief descriere
 * @return ce returneaza
 *
 * @param data detalii parametru
 * @param dataSize alt parametru
 */
node_t *newNode(void *data /*, size_t dataSize*/);

node_t *findNode(list_t list, size_t pos);
void insertNode(list_t *list, node_t *node, size_t pos);

node_t *removeNode(list_t *list, size_t pos);
void freeList(list_t *list);

void forEach(list_t list, void (*func)(node_t *));
list_t filterRange(node_t *first, node_t *last, int (*func)(node_t *));
list_t filter(list_t list, int (*func)(node_t *));

node_t *findFirst(list_t list, int (*func)(node_t *));
node_t *findLast(list_t list, int (*func)(node_t *));
