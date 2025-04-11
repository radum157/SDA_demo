#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define HMAX        10000
#define PATH_MAX    1024
#define CONTENT_MAX 10000

#define DIE(assertion, call_description)                \
    do {                                                \
        if (assertion) {                                \
            fprintf(stderr, "(%s, %d): ",               \
                    __FILE__, __LINE__);                \
            perror(call_description);                   \
            exit(errno);                                \
        }                                               \
    } while (0)

typedef struct ll_node_t {
	void *data;
	struct ll_node_t *next;
} ll_node_t;

typedef struct linked_list_t {
	ll_node_t *head;
	unsigned int data_size;
	unsigned int size;
} linked_list_t;


linked_list_t *ll_create(unsigned int data_size);
void ll_add_nth_node(linked_list_t *list, unsigned int n, const void *new_data);
ll_node_t *ll_remove_nth_node(linked_list_t *list, unsigned int n);
void ll_free(linked_list_t **pp_list);

typedef struct info info;
typedef struct hashtable_t hashtable_t;

unsigned int hash_function_string(void *a);
int compare_function_strings(void *a, void *b);
void key_val_free_function(void *data);

hashtable_t *ht_create(unsigned int hmax, unsigned int (*hash_function)(void *),
	int (*compare_function)(void *, void *),
	void (*key_val_free_function)(void *));
void ht_put(hashtable_t *ht, void *key, unsigned int key_size,
	void *value, unsigned int value_size);
void *ht_get(hashtable_t *ht, void *key);
void ht_free(hashtable_t *ht);

typedef struct {
	char *path;
	char *content;
} file_t;


struct info {
	void *key;
	void *value;
};

struct hashtable_t {
	linked_list_t **buckets; /* Array de liste simplu-inlantuite. */
	/* Nr. total de noduri existente curent in toate bucket-urile. */
	unsigned int size;
	unsigned int hmax; /* Nr. de bucket-uri. */
	/* (Pointer la) Functie pentru a calcula valoarea hash asociata cheilor. */
	unsigned int (*hash_function)(void *);
	/* (Pointer la) Functie pentru a compara doua chei. */
	int (*compare_function)(void *, void *);
	/* (Pointer la) Functie pentru a elibera memoria ocupata de cheie si valoare. */
	void (*key_val_free_function)(void *);
};

void file_search(linked_list_t *files, linked_list_t *found_paths)
{
	ll_node_t *current = files->head;

	hashtable_t *ht = ht_create(HMAX, hash_function_string,
		compare_function_strings, NULL);

	while (current != NULL) {
		char *content = ((file_t *)(current->data))->content;
		char *path = ((file_t *)(current->data))->path;

		// TODO: Solve
		void *value = ht_get(ht, content);
		if (value == NULL) {
			int *cnt = (int *)malloc(sizeof(int));
			*cnt = 1;
			ht_put(ht, content, strlen(content) + 1,
				cnt, sizeof(int));
		} else {
			int *cnt = (int *)value;
			*cnt += 1;
		}

		current = current->next;
	}

	// TODO: Populeaza found_paths conform cerintei
	char *content = NULL;
	int cntmax = 0;

	for (int i = 0; i < ht->hmax; i++) {
		linked_list_t *bucket = ht->buckets[i];
		ll_node_t *it = bucket->head;

		while (it != NULL) {
			info *data = (info *)it->data;
			int *cnt = (int *)data->value;

			if (*cnt > cntmax) {
				cntmax = *cnt;
				content = data->key;
			}

			it = it->next;
		}
	}

	if (content == NULL) {
		printf("NO DATA\n");
		return;
	}

	for (ll_node_t *it = files->head; it != NULL; it = it->next) {
		file_t *file = (file_t *)it->data;
		if (strcmp(file->content, content) == 0) {
			char *newPath = malloc(strlen(file->path) + 1);
			newPath[strlen(file->path)] = '\0';
			strcpy(newPath, file->path);
			ll_add_nth_node(found_paths, found_paths->size, &newPath);
		}
	}

	ht_free(ht);
}

int main(void)
{
	int n; scanf("%d", &n);
	linked_list_t *files = ll_create(sizeof(file_t));

	char path[PATH_MAX];
	char content[CONTENT_MAX];

	for (int i = 0; i < n; ++i) {
		scanf("%s%s", path, content);

		file_t *file = malloc(sizeof(*file));
		DIE(!file, "malloc file failed");

		file->path = strdup(path);
		file->content = strdup(content);

		ll_add_nth_node(files, i, file);
		free(file);
	}

	linked_list_t *result = ll_create(sizeof(char *));
	file_search(files, result);

	ll_node_t *current = result->head;
	if (current) {
		printf("[");
		for (int i = 0; i < result->size - 1; ++i) {
			printf("%s, ", *((char **)current->data));
			current = current->next;
		}

		printf("%s]\n", *((char **)current->data));
	}

	current = files->head;

	for (int i = 0; i < n; ++i) {
		free(((file_t *)current->data)->path);
		free(((file_t *)current->data)->content);

		current = current->next;
	}

	ll_free(&files);
	ll_free(&result);

	return 0;
}


linked_list_t *ll_create(unsigned int data_size)
{
	linked_list_t *ll;

	ll = malloc(sizeof(*ll));

	ll->head = NULL;
	ll->data_size = data_size;
	ll->size = 0;

	return ll;
}

/*
 * Pe baza datelor trimise prin pointerul new_data, se creeaza un nou nod care e
 * adaugat pe pozitia n a listei reprezentata de pointerul list. Pozitiile din
 * lista sunt indexate incepand cu 0 (i.e. primul nod din lista se afla pe
 * pozitia n=0). Daca n >= nr_noduri, noul nod se adauga la finalul listei. Daca
 * n < 0, eroare.
 */
void ll_add_nth_node(linked_list_t *list, unsigned int n, const void *new_data)
{
	ll_node_t *prev, *curr;
	ll_node_t *new_node;

	if (!list) {
		return;
	}

	/* n >= list->size inseamna adaugarea unui nou nod la finalul listei. */
	if (n > list->size) {
		n = list->size;
	}

	curr = list->head;
	prev = NULL;
	while (n > 0) {
		prev = curr;
		curr = curr->next;
		--n;
	}

	new_node = malloc(sizeof(*new_node));
	new_node->data = malloc(list->data_size);
	memcpy(new_node->data, new_data, list->data_size);

	new_node->next = curr;
	if (prev == NULL) {
		/* Adica n == 0. */
		list->head = new_node;
	} else {
		prev->next = new_node;
	}

	list->size++;
}

/*
 * Elimina nodul de pe pozitia n din lista al carei pointer este trimis ca
 * parametru. Pozitiile din lista se indexeaza de la 0 (i.e. primul nod din
 * lista se afla pe pozitia n=0). Daca n >= nr_noduri - 1, se elimina nodul de
 * la finalul listei. Daca n < 0, eroare. Functia intoarce un pointer spre acest
 * nod proaspat eliminat din lista. Este responsabilitatea apelantului sa
 * elibereze memoria acestui nod.
 */
ll_node_t *ll_remove_nth_node(linked_list_t *list, unsigned int n)
{
	ll_node_t *prev, *curr;

	if (!list || !list->head) {
		return NULL;
	}

	/* n >= list->size - 1 inseamna eliminarea nodului de la finalul listei. */
	if (n > list->size - 1) {
		n = list->size - 1;
	}

	curr = list->head;
	prev = NULL;
	while (n > 0) {
		prev = curr;
		curr = curr->next;
		--n;
	}

	if (prev == NULL) {
		/* Adica n == 0. */
		list->head = curr->next;
	} else {
		prev->next = curr->next;
	}

	list->size--;

	return curr;
}

/*
 * Functia intoarce numarul de noduri din lista al carei pointer este trimis ca
 * parametru.
 */
unsigned int ll_get_size(linked_list_t *list)
{
	if (!list) {
		return -1;
	}

	return list->size;
}

/*
 * Procedura elibereaza memoria folosita de toate nodurile din lista, iar la
 * sfarsit, elibereaza memoria folosita de structura lista si actualizeaza la
 * NULL valoarea pointerului la care pointeaza argumentul (argumentul este un
 * pointer la un pointer).
 */
void ll_free(linked_list_t **pp_list)
{
	ll_node_t *currNode;

	if (!pp_list || !*pp_list) {
		return;
	}

	while (ll_get_size(*pp_list) > 0) {
		currNode = ll_remove_nth_node(*pp_list, 0);
		free(currNode->data);
		currNode->data = NULL;
		free(currNode);
		currNode = NULL;
	}

	free(*pp_list);
	*pp_list = NULL;
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza int-uri. Functia afiseaza toate valorile int stocate in nodurile
 * din lista inlantuita separate printr-un spatiu.
 */
void ll_print_int(linked_list_t *list)
{
	ll_node_t *curr;

	if (!list) {
		return;
	}

	curr = list->head;
	while (curr != NULL) {
		printf("%d ", *((int *)curr->data));
		curr = curr->next;
	}

	printf("\n");
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza string-uri. Functia afiseaza toate string-urile stocate in
 * nodurile din lista inlantuita, separate printr-un spatiu.
 */
void ll_print_string(linked_list_t *list)
{
	ll_node_t *curr;

	if (!list) {
		return;
	}

	curr = list->head;
	while (curr != NULL) {
		printf("%s ", (char *)curr->data);
		curr = curr->next;
	}

	printf("\n");
}


/*
 * Functii de comparare a cheilor:
 */
int compare_function_ints(void *a, void *b)
{
	int int_a = *((int *)a);
	int int_b = *((int *)b);

	if (int_a == int_b) {
		return 0;
	} else if (int_a < int_b) {
		return -1;
	} else {
		return 1;
	}
}

int compare_function_strings(void *a, void *b)
{
	char *str_a = (char *)a;
	char *str_b = (char *)b;

	return strcmp(str_a, str_b);
}

/*
 * Functii de hashing:
 */
unsigned int hash_function_int(void *a)
{
	/*
	 * Credits: https://stackoverflow.com/a/12996028/7883884
	 */
	unsigned int uint_a = *((unsigned int *)a);

	uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
	uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
	uint_a = (uint_a >> 16u) ^ uint_a;
	return uint_a;
}

unsigned int hash_function_string(void *a)
{
	/*
	 * Credits: http://www.cse.yorku.ca/~oz/hash.html
	 */
	unsigned char *puchar_a = (unsigned char *)a;
	unsigned long hash = 5381;
	int c;

	while ((c = *puchar_a++))
		hash = ((hash << 5u) + hash) + c; /* hash * 33 + c */

	return hash;
}

/*
 * Functie apelata pentru a elibera memoria ocupata de cheia si valoarea unei
 * perechi din hashtable. Daca cheia sau valoarea contin tipuri de date complexe
 * aveti grija sa eliberati memoria luand in considerare acest aspect.
 */
void key_val_free_function(void *data)
{
	free(((info *)(data))->key);
	free(((info *)(data))->value);
}

/*
 * Functie apelata dupa alocarea unui hashtable pentru a-l initializa.
 * Trebuie alocate si initializate si listele inlantuite.
 */
hashtable_t *
ht_create(unsigned int hmax, unsigned int (*hash_function)(void *),
	int (*compare_function)(void *, void *),
	void (*key_val_free_function)(void *))
{
	hashtable_t *hashtable = malloc(sizeof(hashtable_t));

	hashtable->key_val_free_function = key_val_free_function;
	hashtable->compare_function = compare_function;
	hashtable->hash_function = hash_function;
	hashtable->hmax = hmax;
	hashtable->size = 0;

	hashtable->buckets = malloc(sizeof(linked_list_t *) * hmax);

	for (unsigned int i = 0; i < hmax; ++i) {
		hashtable->buckets[i] = ll_create(sizeof(struct info));
	}

	return hashtable;
}

/*
 * Atentie! Desi cheia este trimisa ca un void pointer (deoarece nu se impune tipul ei), in momentul in care
 * se creeaza o noua intrare in hashtable (in cazul in care cheia nu se gaseste deja in ht), trebuie creata o copie
 * a valorii la care pointeaza key si adresa acestei copii trebuie salvata in structura info asociata intrarii din ht.
 * Pentru a sti cati octeti trebuie alocati si copiati, folositi parametrul key_size_bytes.
 *
 * Motivatie:
 * Este nevoie sa copiem valoarea la care pointeaza key deoarece dupa un apel put(ht, key_actual, value_actual),
 * valoarea la care pointeaza key_actual poate fi alterata (de ex: *key_actual++). Daca am folosi direct adresa
 * pointerului key_actual, practic s-ar modifica din afara hashtable-ului cheia unei intrari din hashtable.
 * Nu ne dorim acest lucru, fiindca exista riscul sa ajungem in situatia in care nu mai stim la ce cheie este
 * inregistrata o anumita valoare.
 */
void
ht_put(hashtable_t *ht, void *key, unsigned int key_size,
	void *value, unsigned int value_size)
{
	if (ht == NULL)
		return;

	unsigned int index = ht->hash_function(key) % ht->hmax;

	linked_list_t *current_bucket = ht->buckets[index];

	if (current_bucket->size == 0) {
		ht->size += 1;
	}

	ll_node_t *current_node = current_bucket->head;

	while (current_node != NULL) {
		struct info *data = (struct info *)current_node->data;
		if (!ht->compare_function(key, data->key)) {
			memcpy(data->value, value, value_size);

			return;
		}

		current_node = current_node->next;
	}

	struct info *new_info = malloc(sizeof(struct info));
	new_info->key = malloc(key_size);
	memcpy(new_info->key, key, key_size);

	new_info->value = malloc(value_size);
	memcpy(new_info->value, value, value_size);

	ll_add_nth_node(current_bucket, current_bucket->size, new_info);
	free(new_info);
}

void *
ht_get(hashtable_t *ht, void *key)
{
	if (ht == NULL)
		return NULL;

	unsigned int index = ht->hash_function(key) % ht->hmax;

	linked_list_t *current_bucket = ht->buckets[index];
	ll_node_t *current_node = current_bucket->head;

	while (current_node != NULL) {
		struct info *data = (struct info *)current_node->data;
		if (!ht->compare_function(key, data->key)) {
			return data->value;
		}

		current_node = current_node->next;
	}

	return NULL;
}

/*
 * Functie care intoarce:
 * 1, daca pentru cheia key a fost asociata anterior o valoare in hashtable folosind functia put
 * 0, altfel.
 */
int
ht_has_key(hashtable_t *ht, void *key)
{
	if (ht == NULL)
		return 0;

	unsigned int index = ht->hash_function(key) % ht->hmax;

	linked_list_t *current_bucket = ht->buckets[index];
	ll_node_t *current_node = current_bucket->head;

	while (current_node != NULL) {
		struct info *data = (struct info *)current_node->data;
		if (!ht->compare_function(key, data->key)) {
			return 1;
		}

		current_node = current_node->next;
	}

	return 0;
}

/*
 * Procedura care elimina din hashtable intrarea asociata cheii key.
 * Atentie! Trebuie avuta grija la eliberarea intregii memorii folosite pentru o intrare din hashtable (adica memoria
 * pentru copia lui key --vezi observatia de la procedura put--, pentru structura info si pentru structura Node din
 * lista inlantuita).
 */
void
ht_remove_entry(hashtable_t *ht, void *key)
{
	if (ht == NULL || ht->size == 0)
		return;

	unsigned int index = ht->hash_function(key) % ht->hmax;

	linked_list_t *current_bucket = ht->buckets[index];
	ll_node_t *current_node = current_bucket->head;

	unsigned int pos = 0;
	while (current_node != NULL) {
		struct info *data = (struct info *)current_node->data;
		if (!ht->compare_function(key, data->key)) {
			free(data->key);
			free(data->value);
			free(data);

			free(ll_remove_nth_node(current_bucket, pos));

			if (current_bucket->size == 0) {
				ht->size -= 1;
			}

			return;
		}

		current_node = current_node->next;
		pos += 1;
	}
}

/*
 * Procedura care elibereaza memoria folosita de toate intrarile din hashtable, dupa care elibereaza si memoria folosita
 * pentru a stoca structura hashtable.
 */
void
ht_free(hashtable_t *ht)
{
	if (ht == NULL)
		return;
	if (ht->key_val_free_function == NULL) {
		free(ht->buckets);
		free(ht);
		return;
	}

	for (unsigned int i = 0; i < ht->hmax; ++i) {
		ll_node_t *current_node = ht->buckets[i]->head;

		while (current_node != NULL) {
			struct info *current_data = (struct info *)current_node->data;
			free(current_data->key);
			free(current_data->value);

			current_node = current_node->next;
		}

		ll_free(&ht->buckets[i]);
	}

	free(ht->buckets);
	free(ht);
}

unsigned int
ht_get_size(hashtable_t *ht)
{
	if (ht == NULL)
		return 0;

	return ht->size;
}

unsigned int
ht_get_hmax(hashtable_t *ht)
{
	if (ht == NULL)
		return 0;

	return ht->hmax;
}
