#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

/* Macro pentru tratat erori */
#define DIE(assertion, call_description)				\
	do {												\
		if (assertion) {								\
			fprintf(stderr, "(%s, %d): ",				\
					__FILE__, __LINE__);				\
			perror(call_description);					\
			exit(errno);								\
		}												\
	} while (0)

/* Funcție auxiliară de print */
void safe_print(void *int_ptr)
{
	if (!int_ptr) {
		printf("NULL ");
	} else {
		int val = *(int *)int_ptr;
		printf("%d ", val);
	}
}

/* Definim un nod din lista simplu inlantuita */
typedef struct node_t {
	void *data;
	struct node_t *next;
} node_t;

/* Structura cozii */
typedef struct queue_t {
	unsigned int data_size; /* dimensiunea in octeti a tipului de date stocat */
	unsigned int size;      /* numarul curent de elemente din coada */
	node_t *front;          /* nodul din fata cozii */
	node_t *back;           /* nodul din spatele cozii */
} queue_t;

/*
 * Alocă memorie pentru o nouă coadă și o initializează
 */
queue_t *q_create(unsigned int data_size)
{
	queue_t *q = (queue_t *)malloc(sizeof(queue_t));
	DIE(!q, "malloc queue_t failed");

	q->data_size = data_size;
	q->size = 0;
	q->front = NULL;
	q->back = NULL;

	return q;
}

/*
 * Functia întoarce numărul de elemente din coada al cărei pointer este trimis
 * ca parametru.
 */
unsigned int q_get_size(queue_t *q)
{
	if (!q) {
		return 0;
	}
	return q->size;
}

/*
 * Functia întoarce 1 dacă coada este goală și 0 în caz contrar.
 */
unsigned int q_is_empty(queue_t *q)
{
	if (!q) {
		return 1;
	}
	return (q->size == 0);
}

/*
 * Functia întoarce primul element din coadă, fără să îl elimine.
 * Daca nu există element, se întoarce NULL.
 */
void *q_front(queue_t *q)
{
	if (!q || q_is_empty(q)) {
		return NULL;
	}

	return q->front->data;
}

/*
 * Functia scoate un element din coadă. Se va întoarce primul element dacă operația
 * s-a efectuat cu succes (exista cel puțin un element pentru a fi eliminat)
 * și 0 în caz contrar.
 */
void *q_dequeue(queue_t *q)
{
	if (!q || q_is_empty(q)) {
		return 0;
	}

	/* Scoatem nodul din fața cozii */
	node_t *temp = q->front;
	q->front = q->front->next;

	/* Eliberam memoria pentru data si pentru nod */
	void *value = temp->data;
	free(temp->data);
	free(temp);

	q->size--;

	/* Daca am golit coada, back devine NULL */
	if (q->size == 0) {
		q->back = NULL;
	}

	return value;
}

/*
 * Functia introduce un nou element in coada. Se va întoarce 1 dacă
 * operația s-a efectuat cu succes și 0 în caz contrar (de ex. alocare eșuată).
 */
int q_enqueue(queue_t *q, void *new_data)
{
	if (!q) {
		return 0;
	}

	/* Alocam un nod nou */
	node_t *new_node = (node_t *)malloc(sizeof(node_t));
	DIE(!new_node, "malloc node_t failed");

	/* Alocam data pentru noul nod */
	new_node->data = malloc(q->data_size);
	DIE(!new_node->data, "malloc new_node->data failed");

	/* Copiem informatia in noul nod */
	memcpy(new_node->data, new_data, q->data_size);
	new_node->next = NULL;

	/* Dacă coada e goală, front = back = noul nod */
	if (q_is_empty(q)) {
		q->front = new_node;
		q->back = new_node;
	} else {
		/* Adaugam nodul la sfarsitul cozii */
		q->back->next = new_node;
		q->back = new_node;
	}

	q->size++;
	return 1;
}

/*
 * Functia elimină toate elementele din coada primită ca parametru.
 * După aceea, coada va fi goală.
 */
void q_clear(queue_t *q)
{
	if (!q) {
		return;
	}

	while (!q_is_empty(q)) {
		q_dequeue(q);
	}
}

/*
 * Functia eliberează toată memoria ocupată de coadă.
 */
void q_free(queue_t *q)
{
	if (!q) {
		return;
	}

	q_clear(q);
	free(q);
}

/*---------------------------------------- Exemplu main ----------------------------------------------*/

typedef long long llint;

llint sum(int n, int *v, llint a, llint b)
{
	if (a > b) {
		a ^= b;
		b ^= a;
		a ^= b;
	}

	queue_t *q = q_create(sizeof(llint));

	llint *start = (llint *)malloc(sizeof(llint));
	*start = 0;
	q_enqueue(q, start);

	llint sol = 0;

	while (!q_is_empty(q)) {
		llint front = *(llint *)q_front(q);
		q_dequeue(q);

		if (front >= a) {
			sol += front;
		}

		for (int i = 0; i < n; i++) {
			if (front > (b - v[i]) / 10) {
				continue;
			}
			if (front == 0 && v[i] == 0) {
				continue;
			}

			llint *new = (llint *)malloc(sizeof(llint));
			*new = front * 10 + v[i];

			q_enqueue(q, new);
		}
	}

	q_free(q);
	return sol;
}

int main()
{
	int n, *v;
	long long a, b;
	scanf("%d", &n);
	v = (int *)malloc(n * sizeof(int));
	DIE(!v, "malloc v failed");
	for (int i = 0; i < n; i++) {
		scanf("%d", &v[i]);
	}
	scanf("%lld %lld", &a, &b);

	printf("%lld", sum(n, v, a, b));
	free(v);
}
