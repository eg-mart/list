#include <stdio.h>

#include "list_debug.h"

const char *BEGIN = \
"digraph {\n\
graph [dpi = 200, splines=ortho];\n\
node [shape = \"rectangle\", style = \"rounded\"];\n\
{rank=same;\n\
node [shape = \"Mrecord\"];\n\
";

int list_graphic_dump(struct List *list)
{
	FILE *dump = fopen("graph_dump.dot", "w");
	if (!dump) return 1;

	fprintf(dump, "%s", BEGIN);

	for (size_t j = 0; j < list->capacity; j++) {
		fprintf(dump, "node%lu [label=\"{%lu | %d | {%d | %d}}\"];\n", 
				j, j, list->data[j], list->prev[j], list->next[j]);
	}

	fprintf(dump, "}\n");
	fprintf(dump, "{rank=min\n");
	fprintf(dump, "node_head [label=\"head\"];\n");
	fprintf(dump, "node_tail [label=\"tail\"];\n");
	fprintf(dump, "node_free [label=\"free\"];\n");
	fprintf(dump, "}\n");

	fprintf(dump, "node_head -> node%d;\n", list->head);
	fprintf(dump, "node_tail -> node%d;\n", list->tail);
	fprintf(dump, "node_free -> node%d;\n", list->free);

	int i = list->head;
	do {
		fprintf(dump, "node%d -> node%d [color=green];\n", i, list->next[i]);
		i = list->next[i];
	} while (i != 0);

	i = list->tail;
	do {
		fprintf(dump, "node%d -> node%d [color=blue];\n", i, list->prev[i]);
		i = list->prev[i];
	} while (i != 0);

	i = list->free;
	do {
		fprintf(dump, "node%d -> node%d [color=red];\n", i, list->next[i]);
		i = list->next[i];
	} while (i != 0);

	for (size_t j = 0; j < list->capacity - 1; j++) {
		fprintf(dump, "node%lu -> node%lu [weight=5, style=invis];\n", j, j + 1);
	}

	fprintf(dump, "}\n");

	return 0;
}
