#include <stdio.h>

#include "list_debug.h"
#include "list.h"

int main()
{
	struct List list = {};
	list_ctor(&list);

	size_t ind = 0;
	for (int i = 0; i < 10; i++)
		list_insert(&list, list.head, i, &ind);
	list_insert(&list, 0, 20, &ind);
	list_graphic_dump(&list);
	list_dtor(&list);
	return 0;
}
