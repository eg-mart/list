#include <stdio.h>

#include "list_debug.h"
#include "list.h"

int main()
{
	struct List list = {};
	list_ctor(&list);

	size_t ind = 0;
	int err = 0;
	int i = 0;
	while (!err) {
		err = list_insert(&list, list.head, i, &ind);
		if (!err)
			i++;
	}
	list_delete(&list, list.tail);
	list_graphic_dump(&list);
	list_defragment(&list);
	list_graphic_dump(&list);
	list_dtor(&list);
	return 0;
}
