
#include "dynarray.h"

void dynarray_debug(struct dynarray *dynarray, FILE* stream) {
    fprintf(stream, "dynarray @%p: size: %zu, items @%p\n", (void*)dynarray, dynarray->array_size, (void*)dynarray->items);
}


size_t dynarray_size(struct dynarray *dynarray) {
    return dynarray->array_size;
}


void * dynarray_get(
    struct dynarray *dynarray,
    size_t index) {
    return dynarray->items[index];
}


struct dynarray * dynarray_create() {
    struct dynarray * dynarray = malloc(sizeof(struct dynarray));
    if (dynarray != NULL) {
        dynarray->array_size = 0;
        dynarray->items = NULL;
    }
    return dynarray;
}


void dynarray_destroy(
    struct dynarray *dynarray,
    void (*item_free)(void *)) {

    if (dynarray->array_size > 0) {
        void **end = dynarray->items + dynarray->array_size;
        for (void **p = dynarray->items; p < end; p++) {
            item_free(*p);
        }

        free(dynarray->items);
    }

    free(dynarray);
}


void * dynarray_insert(
    struct dynarray *dynarray,
    void *item,
    int (*compare)(const void *, const void *)) {

    void **new_items = malloc((dynarray->array_size+1) * sizeof(void*));
    if (new_items == NULL) // failed to allocate
        return NULL;

    void **target = new_items;
    void **src_end = dynarray->items + dynarray->array_size;
    int uninserted = 1;
    for (void **src = dynarray->items; src < src_end; src++) {
        int comparison = compare(*src, item);
        if (comparison == 0) { // item already exists, will change nothing
            free(new_items);
            return *src; // return already existing item
        } else if (comparison > 0 && uninserted) {
            // *src is greater than item for the _first_ time
            // insert item here
            *target = item;
            // move target to next place
            ++target;
            uninserted = 0; //
        }
        *target = *src;
        ++target;
    }
    if (uninserted) {
        *target = item;
    }

    ++dynarray->array_size;
    free(dynarray->items);
    dynarray->items = new_items;
    return item;  // return item itsel to indicate successful insertion
}


int dynarray_delete(
    struct dynarray *dynarray,
    void *item,
    int (*compare)(const void *, const void *),
    void (*item_free)(void *)) {

    if (dynarray->array_size == 0) {
        return 0;
    }

    void **new_items = NULL;
    if (dynarray->array_size > 0) {
        new_items = malloc((dynarray->array_size-1) * sizeof(void*));
        if (new_items == NULL) // failed to allocate
            return -1;
    }

    void **target = new_items;
    void **src_end = dynarray->items + dynarray->array_size;
    int found = 0;
    for (void **src = dynarray->items; src < src_end; src++) {
        int comparison = compare(*src, item);
        if (comparison == 0) {
            item_free(*src);
            ++src;
            found = 1;
        }
        *target = *src;
        ++target;
    }
    if (!found) {
        if (new_items != NULL) {
            free(new_items);
        }
        return 0;
    }

    --dynarray->array_size;
    free(dynarray->items);
    dynarray->items = new_items;
    return 1;
}


int dynarray_walk(
    struct dynarray *dynarray,
    int (*visit)(void*, void*),
    void *cookie) {

    void **end = dynarray->items + dynarray->array_size;
    for (void **p = dynarray->items; p < end; p++) {
        int stop = (*visit)(*p, cookie);
        if (stop) {
            return stop;
        }
    }
    return 0;
}
