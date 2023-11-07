#include <stdlib.h>

#include "list.h"

int list_ctor(struct List *list)
{
	list->capacity = INIT_CAPACITY;
	list->prev = (int*) calloc(INIT_CAPACITY, sizeof(int));
	if (!list->prev) return 1;
	list->next = (int*) calloc(INIT_CAPACITY, sizeof(int));
	if (!list->next) return 1;
	list->data = (int*) calloc(INIT_CAPACITY, sizeof(int));
	if (!list->data) return 1;

	for (size_t i = 0; i < INIT_CAPACITY; i++) {
		list->data[i] = POISON;
		list->prev[i] = -1;
	}

	list->size = 1;
	list->head = 0;
	list->tail = 0;
	list->next[0] = 0;
	list->prev[0] = 0;
	list->free = 1;
	for (size_t i = 1; i < INIT_CAPACITY; i++)
		list->next[i] = (int) i + 1;
	list->next[INIT_CAPACITY - 1] = 0;

	return 0;
}

void list_dtor(struct List *list)
{
	free(list->data);
	list->data = NULL;
	free(list->prev);
	list->prev = NULL;
	free(list->next);
	list->next = NULL;
	list->capacity = 0;
	list->head = 0;
	list->tail = 0;
	list->free = 0;
	list->size = 0;
}

int list_insert(struct List *list, size_t dest, int x, size_t *ind)
{
	if (list->capacity == list->size)
		return 1;

	if (list->free == 0) //this shouldn't happen if there is free space
		return 1;

	*ind = (size_t) list->free;
	list->free = list->next[list->free];

	list->data[*ind] = x;
	list->next[*ind] = (int) dest;
	list->prev[*ind] = list->prev[dest];
	list->next[list->prev[dest]] = (int) *ind;
	list->prev[dest] = (int) *ind;

	list->head = list->next[0];
	list->tail = list->prev[0];

	list->size++;

	return 0;
}

int list_delete(struct List *list, size_t dest)
{
	list->next[list->prev[dest]] = list->next[dest];
	list->prev[list->next[dest]] = list->prev[dest];
	list->prev[dest] = -1;
	list->next[dest] = list->free;
	list->free = (int) dest;

	list->head = list->next[0];
	list->tail = list->prev[0];

	list->size--;

	return 0;
}
