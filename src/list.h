#ifndef _LIST_H
#define _LIST_H

#include <limits.h>
#include <stdio.h>

struct List {
	size_t capacity;
	size_t size;
	int head;
	int tail;
	int free;
	int *prev;
	int *next;
	int *data;
};

const size_t INIT_CAPACITY = 20;
const int POISON = INT_MAX;

int list_ctor(struct List *list);
void list_dtor(struct List *list);
int list_insert(struct List *list, size_t dest, int x, size_t *ind);

#endif
