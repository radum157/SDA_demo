#include <stdio.h>
#include <stdlib.h>

#include "list.h"

void printNode(node_t *node)
{
	if (node) {
		printf("%d ", *(int *)node->data);
	}
}

void printList(list_t list)
{
	printf("List: ");
	forEach(list, printNode);
	printf("\n");
}

int even(node_t *node)
{
	int x = *(int *)node->data;
	return x % 2 == 0;
}

int main()
{
	list_t myList = createList(sizeof(int));

	int values[] = { 11, 22, 33, 44, 55 };
	for (int i = 0; i < 5; i++) {
		node_t *new_node = newNode(malloc(sizeof(int)));
		*(int *)new_node->data = values[i];
		insertNode(&myList, new_node, 10);
	}

	printf("After insertion:\n");
	printList(myList);

	list_t f = filterRange(myList.head, NULL, even);

	for (node_t *it = f.head; it != NULL; it = it->next) {
		node_t *n = (node_t *)it->data;
		printf("%d ", *(int *)n->data);
	}
	printf("\n");

	freeList(&f);

	node_t *n = findFirst(myList, even);
	if (!n) {
		printf("NULL\n");
	} else {
		printf("%d\n", *(int *)n->data);
	}

	n = findLast(myList, even);
	if (!n) {
		printf("NULL\n");
	} else {
		printf("%d\n", *(int *)n->data);
	}

	// while (myList.head) {
	// 	node_t *node = removeNode(&myList, 0);
	// 	free(node->data);
	// 	free(node);
	// }

	// while (myList.head) {
	// 	node_t *node = removeNode(&myList, myList.size - 1);
	// 	free(node->data);
	// 	free(node);
	// }

	node_t *node = removeNode(&myList, 2); // Remove element at index 2

	if (!node) {
		printf("NULL\n");
	} else {
		free(node->data);
		free(node);
	}

	printf("After removing element at index 2:\n");
	printList(myList);

	freeList(&myList);
	return 0;
}
