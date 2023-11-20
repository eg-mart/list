#include <stdio.h>

#include "list_debug.h"

#define min(a, b) (a) < (b) ? (a) : (b)

uint64_t list_validate(struct List *list)
{
	uint64_t err = 0;
	if (list->size > list->capacity)
		err |= LIST_WRONG_CAPACITY_ERR;
	
	
	return 0;
}

int list_graphic_dump(struct List *list)
{
	const char *BEGIN = \
	"digraph {\n"
	"graph [dpi = 200, splines=ortho];\n"
	"node [shape = \"rectangle\", style = \"rounded\"];\n"
	"{rank=same;\n"
	"node [shape = \"Mrecord\"];\n";

	FILE *dump = fopen("graph_dump.dot", "a");
	if (!dump) return 1;

	fprintf(dump, "%s", BEGIN);

	for (size_t i = 0; i < list->capacity; i++) {
		fprintf(dump, "node%lu [label=\"{%lu | ", i, i);
		if (list->data[i] == POISON_INT)
			fprintf(dump, "POISON");
		else
			fprintf(dump, "%d", list->data[i]);
		fprintf(dump, " | {");
		if (list->prev[i] == POISON_ST)
			fprintf(dump, "POISON");
		else
			fprintf(dump, "%lu", list->prev[i]);
		fprintf(dump, " | %lu}}\"];\n", list->next[i]);
	}

	fprintf(dump, "}\n");
	fprintf(dump, "{rank=min\n");
	fprintf(dump, "node_head [label=\"head\"];\n");
	fprintf(dump, "node_tail [label=\"tail\"];\n");
	fprintf(dump, "node_free [label=\"free\"];\n");
	fprintf(dump, "}\n");

	fprintf(dump, "node_head -> node%lu;\n", list->head);
	fprintf(dump, "node_tail -> node%lu;\n", list->tail);
	fprintf(dump, "node_free -> node%lu;\n", list->free);

	size_t ind = list->head;
	while (ind != 0) {
		fprintf(dump, "node%lu -> node%lu [color=green];\n", ind, list->next[ind]);
		ind = list->next[ind];
	}
	fprintf(dump, "node%lu -> node%lu [color=green];\n", ind, list->next[ind]);
	ind = list->next[ind];

	ind = list->tail;
	while (ind != 0) {
		fprintf(dump, "node%lu -> node%lu [color=blue];\n", ind, list->prev[ind]);
		ind = list->prev[ind];
	}
	fprintf(dump, "node%lu -> node%lu [color=blue];\n", ind, list->prev[ind]);
	ind = list->prev[ind];

	if (list->free != 0) {
		ind = list->free;
		do {
			fprintf(dump, "node%lu -> node%lu [color=red];\n", ind, list->next[ind]);
			ind = list->next[ind];
		} while (ind != 0);
	}

	for (size_t i = 0; i < list->capacity - 1; i++) {
		fprintf(dump, "node%lu -> node%lu [weight=5, style=invis];\n", i, i + 1);
	}

	fprintf(dump, "}\n");

	return 0;
}
