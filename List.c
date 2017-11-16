#include "List.h"

#include <stdlib.h>
#include <string.h>

List *listNew(size_t elementSize) {
    List *list = malloc(sizeof(List));
    list->elementSize = elementSize;
    list->size = 0;
    list->capacity = 16;
    list->data = malloc(list->elementSize * list->capacity);
    return list;
}

size_t listGetSize(List *list) {
    return list->size;
}

void listGet(List *list, size_t index, void *out) {
    memcpy(out, list->data + index * list->elementSize, list->elementSize);
}

void listSet(List *list, size_t index, void *element) {
    memcpy(list->data + index * list->elementSize, element, list->elementSize);
}

void listAdd(List *list, void *element) {
    if (list->size + 1 > list->capacity) {
        list->capacity *= 2;
        list->data = realloc(list->data, list->elementSize * list->capacity);
    }
    listSet(list, list->size, element);
    list->size++;
}

void listDelete(List *list) {
    free(list->data);
    free(list);
}
