#ifndef _LIST_H
#define _LIST_H

#include <limits.h>
#include <stdio.h>

struct List {
	size_t capacity;
	size_t size;
	size_t head;
	size_t tail;
	size_t free;
	size_t *prev;
	size_t *next;
	int *data;
};

enum ListReallocMode {
	SHRINK,
	GROW,
	AUTO
};

enum ListError {
	LIST_NO_ERR = 0,
	LIST_NO_MEM_ERR = 1 << 1,
	LIST_FREE_ERR = 1 << 2,
	LIST_WRONG_IND_ERR = 1 << 3,
	LIST_WRONG_DELTA_ERR = 1 << 4,
#ifdef DEBUG
	LIST_LOOPS_ERR = 1 << 5,
	LIST_POISON_ERR = 1 << 6,
	LIST_NO_POISON_ERR = 1 << 7,
	LIST_WRONG_CAPACITY_ERR = 1 << 8,
	LIST_WRONG_SIZE_ERR = 1 << 9,
#endif
};

const int POISON_INT = INT_MAX;
const size_t POISON_ST = (size_t) -1;

const size_t INIT_CAPACITY = 5;
const size_t REALLOC_COEFF = 2;
const size_t SHRINK_COEFF = 4;

enum ListError list_ctor(struct List *list);
void list_dtor(struct List *list);
enum ListError list_insert(struct List *list, size_t dest, int x, size_t *ind);
enum ListError list_delete(struct List *list, size_t dest);
enum ListError list_reallocate(struct List *list, size_t delta, enum ListReallocMode mode);
enum ListError list_defragment(struct List *list);

#endif
