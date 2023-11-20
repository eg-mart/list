#include <stdlib.h>

#include "list.h"

static enum ListError _list_reallocate(struct List *list, size_t new_size);

enum ListError list_ctor(struct List *list)
{
	list->capacity = INIT_CAPACITY;
	list->prev = (size_t*) calloc(INIT_CAPACITY, sizeof(size_t));
	if (!list->prev)
		return LIST_NO_MEM_ERR;
	list->next = (size_t*) calloc(INIT_CAPACITY, sizeof(size_t));
	if (!list->next)
		return LIST_NO_MEM_ERR;
	list->data = (int*) calloc(INIT_CAPACITY, sizeof(int));
	if (!list->data)
		return LIST_NO_MEM_ERR;

	for (size_t i = 0; i < INIT_CAPACITY; i++) {
		list->data[i] = POISON_INT;
		list->prev[i] = POISON_ST;
	}

	list->size = 1;
	list->head = 0;
	list->tail = 0;
	list->next[0] = 0;
	list->prev[0] = 0;
	list->free = 1;
	for (size_t i = 1; i < INIT_CAPACITY; i++)
		list->next[i] = i + 1;
	list->next[INIT_CAPACITY - 1] = 0;

	return LIST_NO_ERR;
}

void list_dtor(struct List *list)
{
	free(list->data);
	list->data = NULL;
	free(list->prev);
	list->prev = NULL;
	free(list->next);
	list->next = NULL;
	list->capacity = POISON_ST;
	list->head = POISON_ST;
	list->tail = POISON_ST;
	list->free = POISON_ST;
	list->size = POISON_ST;
}

enum ListError list_insert(struct List *list, size_t dest, int x, size_t *ind)
{
	if (list->capacity <= list->size) {
		enum ListError err = _list_reallocate(list, list->capacity * REALLOC_COEFF);
		if (err < 0)
			return err;
	}

	*ind = (size_t) list->free;
	list->free = list->next[list->free];

	list->data[*ind] = x;
	list->next[*ind] = dest;
	list->prev[*ind] = list->prev[dest];
	list->next[list->prev[dest]] = *ind;
	list->prev[dest] = *ind;

	list->head = list->next[0];
	list->tail = list->prev[0];

	list->size++;

	return LIST_NO_ERR;
}

enum ListError list_delete(struct List *list, size_t dest)
{
	if (dest <= 0)
		return LIST_WRONG_IND_ERR;
	
	list->next[list->prev[dest]] = list->next[dest];
	list->prev[list->next[dest]] = list->prev[dest];
	list->prev[dest] = POISON_ST;
	list->next[dest] = list->free;
	list->free = dest;

	list->head = list->next[0];
	list->tail = list->prev[0];

	list->size--;

	return LIST_NO_ERR;
}

enum ListError list_shrink(struct List *list, size_t delta)
{
	if (delta == 0 && list->size * SHRINK_COEFF <= list->capacity)
		return _list_reallocate(list, list->capacity / REALLOC_COEFF);
	else if (delta == 0)
		return LIST_NO_REALLOC; //no reallocation was needed
	if (delta >= list->capacity)
		return LIST_WRONG_DELTA_ERR;
	return _list_reallocate(list, list->capacity - delta);
}

enum ListError _list_reallocate(struct List *list, size_t new_size)
{
	if (new_size < list->capacity) {
		enum ListError err = list_defragment(list);
		if (err < 0)
			return err;
	}
	
	int *new_data = (int*) realloc(list->data, new_size * sizeof(int));
	if (!new_data)
		return LIST_NO_MEM_ERR;
	size_t *new_next = (size_t*) realloc(list->next, new_size * sizeof(size_t));
	if (!new_next)
		return LIST_NO_MEM_ERR;
	size_t  *new_prev = (size_t*) realloc(list->prev, new_size * sizeof(size_t));
	if (!new_prev)
		return LIST_NO_MEM_ERR;

	list->data = new_data;
	list->next = new_next;
	list->prev = new_prev;

	for (size_t i = list->capacity; i < new_size; i++) {
		list->prev[i] = POISON_ST;
		list->next[i] = list->free;
		list->data[i] = POISON_INT;
		list->free = i;
	}

	list->capacity = new_size;

	return LIST_NO_ERR;
}

enum ListError list_defragment(struct List *list)
{
	int *new_data = (int*) calloc(list->capacity, sizeof(int));
	if (!new_data)
		return LIST_NO_MEM_ERR;
	size_t *new_prev = (size_t*) calloc(list->capacity, sizeof(size_t));
	if (!new_prev)
		return LIST_NO_MEM_ERR;
	size_t *new_next = (size_t*) calloc(list->capacity, sizeof(size_t));
	if (!new_next)
		return LIST_NO_MEM_ERR;

	new_data[0] = POISON_INT;
	new_prev[0] = list->size - 1;
	if (list->size > 1)
		new_next[0] = 1;

	size_t ind = list->head;
	size_t j = 1;
	while (ind != 0) {
		if (list->prev[ind] == POISON_ST)
			continue;
		new_data[j] = list->data[ind];
		new_prev[j] = j - 1;
		new_next[j] = j + 1;
		j++;
		ind = list->next[ind];
	}
	new_next[list->size - 1] = 0;

	list->free = 0;
	for (size_t i = list->size; i < list->capacity; i++) {
		new_prev[i] = POISON_ST;
		new_next[i] = list->free;
		new_data[i] = POISON_INT;
		list->free = i;
	}

	free(list->data);
	free(list->prev);
	free(list->next);
	list->head = new_next[0];
	list->tail = new_prev[0];
	list->data = new_data;
	list->prev = new_prev;
	list->next = new_next;

	return LIST_NO_ERR;
}