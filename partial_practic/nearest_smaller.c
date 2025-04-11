#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
	int data;
	struct Node *next;
} Node;

typedef struct Stack {
	Node *top;
} Stack;

void initStack(Stack *stack)
{
	stack->top = NULL;
}

int isEmpty(Stack *stack)
{
	return stack->top == NULL;
}

void push(Stack *stack, int data)
{
	Node *newNode = (Node *)malloc(sizeof(Node));
	if (!newNode) {
		printf("Memory allocation failed\n");
		exit(1);
	}
	newNode->data = data;
	newNode->next = stack->top;
	stack->top = newNode;
}

int pop(Stack *stack)
{
	if (isEmpty(stack)) {
		printf("NONO STACK EMPTY:(");
		exit(1);
	}
	Node *temp = stack->top;
	int popped = temp->data;
	stack->top = temp->next;
	free(temp);
	return popped;
}

int top(Stack *stack)
{
	if (isEmpty(stack)) {
		return -1;
	}
	return stack->top->data;
}

void freeStack(Stack *stack)
{
	while (!isEmpty(stack)) {
		pop(stack);
	}
}

void function(int *arr, int n)
{
	//TODO
	Stack st;
	initStack(&st);

	int cnt = 0;

	for (int i = 0; i < n; i++) {
		while (!isEmpty(&st) && top(&st) >= arr[i]) {
			pop(&st);
		}

		if (!isEmpty(&st)) {
			printf("%d ", top(&st));
		} else {
			printf("-1 ");
			cnt++;
		}

		push(&st, arr[i]);
	}

	printf("\n%d\n", cnt);

	freeStack(&st);
}

int main()
{
	int n;
	scanf("%d", &n);
	int *arr = (int *)malloc(n * sizeof(int));
	if (!arr) {
		fprintf(stderr, "Memory allocation failed\n");
		return 1;
	}

	for (int i = 0; i < n; i++) {
		scanf("%d", &arr[i]);
	}

	function(arr, n);

	free(arr);
	return 0;
}
