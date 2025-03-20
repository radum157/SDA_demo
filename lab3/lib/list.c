#include "list.h"

#include <stdlib.h>
// #include <string.h>

inline list_t createList(size_t dataSize)
{
	return (list_t)
	{
		.head = NULL,
			.tail = NULL,
			.dataSize = dataSize,
			.size = 0,
			.freeData = free
	};
}

list_t *newList(size_t dataSize)
{
	list_t *l = (list_t *)malloc(sizeof(list_t));
	// DIE()...

	*l = createList(dataSize);
	return l;
}

node_t *newNode(void *data /*, size_t dataSize*/)
{
	node_t *n = (node_t *)calloc(1, sizeof(node_t));
	// DIE()

	n->data = data;

	// n->data = malloc(dataSize);
	// DIE()
	// memcpy(n->data, data, dataSize);

	return n;
}

node_t *findNode(list_t list, size_t pos)
{
	if (list.size == 0 || pos >= list.size) {
		return NULL;
	}

	node_t *it = list.head;
	size_t i = 0;

	for (; it != NULL; it = it->next, i++) {
		if (i == pos) {
			return it;
		}
	}

	return NULL;
}

static inline void _insertNode(node_t *node, node_t *prev)
{
	if (!prev || !node) {
		return;
	}

	node->prev = prev;
	node->next = prev->next;

	if (prev->next != NULL) {
		prev->next->prev = node;
	}
	prev->next = node;
}

void insertNode(list_t *list, node_t *node, size_t pos)
{
	if (list->size == 0) {
		list->head = node;
		list->tail = node;
		list->size = 1;
		return;
	}

	if (pos == 0) {
		node->next = list->head;
		list->head->prev = node;

		list->head = node;
		list->size++;

		return;
	}

	if (pos > list->size) {
		pos = list->size;
	}

	node_t *prev = findNode(*list, pos - 1);
	if (!prev) {
		return;
	}

	_insertNode(node, prev);
	if (pos >= list->size) {
		list->tail = node;
	}

	list->size++;
}

static inline void _removeNode(node_t *node)
{
	if (!node) {
		return;
	}

	if (node->prev != NULL) {
		node->prev->next = node->next;
	}
	if (node->next != NULL) {
		node->next->prev = node->prev;
	}
}

node_t *removeNode(list_t *list, size_t pos)
{
	node_t *target = findNode(*list, pos);
	if (!target) {
		return NULL;
	}

	list->size--;

	if (target == list->head) {
		list->head = list->head->next;
	}
	if (target == list->tail) {
		list->tail = list->tail->prev;
	}

	_removeNode(target);
	return target;
}

void freeList(list_t *list)
{
	for (node_t *it = list->head; it != NULL;) {
		node_t *next = it->next;

		if (list->freeData != NULL) {
			list->freeData(it->data);
		}

		free(it);
		it = next;
	}

	*list = createList(list->dataSize);
}

inline void forEach(list_t list, void (*func)(node_t *))
{
	for (node_t *it = list.head; it != NULL; it = it->next) {
		func(it);
	}
}

list_t filterRange(node_t *first, node_t *last, int (*func)(node_t *))
{
	list_t filtered = createList(sizeof(node_t));
	filtered.freeData = NULL; // nu dati free la nodurile din list...

	for (node_t *it = first; it != last; it = it->next) {
		if (func(it)) {
			insertNode(&filtered, newNode(it), filtered.size);
		}
	}

	return filtered;
}

list_t filter(list_t list, int (*func)(node_t *))
{
	list_t filtered = createList(sizeof(node_t));
	filtered.freeData = NULL; // nu dati free la nodurile din list...

	for (node_t *it = list.head; it != NULL; it = it->next) {
		if (func(it)) {
			insertNode(&filtered, newNode(it), filtered.size);
		}
	}

	return filtered;
}

// se pot extinde si astea la un range. la fel si freeList si findNode

node_t *findFirst(list_t list, int (*func)(node_t *))
{
	for (node_t *it = list.head; it != NULL; it = it->next) {
		if (func(it)) {
			return it;
		}
	}

	return NULL;
}

node_t *findLast(list_t list, int (*func)(node_t *))
{
	for (node_t *it = list.tail; it != NULL; it = it->prev) {
		if (func(it)) {
			return it;
		}
	}

	return NULL;
}
