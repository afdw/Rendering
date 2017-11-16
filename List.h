#ifndef LIST_H
#define LIST_H

#include <stddef.h>

typedef struct List {
    size_t elementSize;
    size_t size;
    size_t capacity;
    void *data;
} List;

List *listNew(size_t elementSize);

size_t listGetSize(List *list);

void listGet(List *list, size_t index, void *out);

void listSet(List *list, size_t index, void *element);

void listAdd(List *list, void *element);

void listDelete(List *list);

#endif
