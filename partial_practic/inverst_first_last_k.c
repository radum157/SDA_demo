#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
	int data;
	struct Node *next;
} Node;

// Funcție pentru crearea unui nod nou
Node *createNode(int data)
{
	Node *newNode = (Node *)malloc(sizeof(Node));
	newNode->data = data;
	newNode->next = NULL;
	return newNode;
}

// Functie care verifica daca lista este goala
int isEmpty(Node *head)
{
	return head == NULL;
}

// Functie care elibereaza memoria alocata pentru o lista
void freeList(Node *head)
{
	Node *temp;
	while (head != NULL) {
		temp = head;
		head = head->next;
		free(temp);
	}
}

// Funcție pentru inserarea unui nod la finalul listei
void append(Node **headRef, int data)
{
	Node *newNode = createNode(data);
	if (*headRef == NULL) {
		*headRef = newNode;
		return;
	}
	Node *temp = *headRef;
	while (temp->next)
		temp = temp->next;
	temp->next = newNode;
}

// Funcție pentru afișarea listei
void printList(Node *head)
{
	while (head) {
		printf("%d ", head->data);
		head = head->next;
	}
	printf("-> NULL\n");
}

// Funcție de ajutor: întoarce pointer la nodul de pe poziția `index` (0-based)
Node *getNodeAt(Node *head, int index)
{
	Node *temp = head;
	while (index-- > 0 && temp) {
		temp = temp->next;
	}
	return temp;
}

// Functie care inverseaza primele k elemente cu ultimele k
Node *swapSegments(Node *head, int k)
{
	/*
		Parametrii:
			1) Node* head = pointer la primul element din lista simplu inlantuita;
			2) int k      = primele k elemente ale listei trebuie inversate cu ultimele k;
							se garanteaza ca numarul k nu va depasi jumatatea dimensiunii listei;

		### TODO:
			Returnati pointer-ul catre capul listei modificate conform cerintei;
	*/

	if (k == 0) {
		return head;
	}

	int n = 0;
	for (Node *it = head; it != NULL; it = it->next, n++);

	Node *it2 = getNodeAt(head, n - k);

	for (Node *it = head; it2 != NULL; it = it->next, it2 = it2->next) {
		it->data ^= it2->data;
		it2->data ^= it->data;
		it->data ^= it2->data;
	}

	return head;
}


int main()
{

	Node *head = NULL;
	int n, k;

	scanf("%d %d", &n, &k);

	for (int i = 0; i < n; i++) {
		int value;
		scanf("%d", &value);
		append(&head, value);
	}

	// Doar functia de swapSegments trebuie modificata !!!
	head = swapSegments(head, k);

	printList(head);

	freeList(head);

	return 0;
}
