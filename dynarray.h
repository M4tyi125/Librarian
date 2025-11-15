#ifndef DYNARRAY_H_INCLUDED
#define DYNARRAY_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>

// The main structure to hold all necessary info
// to manage a dynamic array.
// The dynamic array shall always contain pointers to
// data.
struct dynarray {
    size_t array_size;
    void **items;
};

// prints some info about dynarray to the given stream
// (e.g. stdout, stderr, ...) for troubleshooting
void dynarray_debug(struct dynarray *dynarray, FILE* stream);

// returns the size (i.e. the number of items) of this dynarray
size_t dynarray_size(struct dynarray *dynarray);

// returns the Nth (index) item of dynarray. The index must
// be in the range of 0 .. dynarray_size(dynarray) - 1, or else
// the result is undefined (segmentation fault)
void * dynarray_get(
    struct dynarray *dynarray,
    size_t index);

// creates an empty dynarray.
struct dynarray * dynarray_create();

// frees all (memory) resources allocated for
// dynarray, de-allocating item resources by calling
// item_free() for each item
void dynarray_destroy(struct dynarray *dynarray, void (*item_free)(void *));

// inserts a new item to dynarray, returning the same item (pointer)
// if it was inserted to dynarrazý successfully, or NULL of allocating
// new memory failed, or the pointer to an existing item with an equivalent
// value to item.
// Items are stored in a sorted order, as defined by compare(...) function,
// which must behave akin to strcmp(...) - returning 1, 0, or -1, if the
// two values are *first > *second, *first == *second, or *first < *second,
// respectively.
void * dynarray_insert(
    struct dynarray *dynarray,
    void *item,
    int (*compare)(const void *, const void *));

// finds a value in dynarray (as defined by compare(...) function,
// and removes it from dynarray, also de-allocating related resources
// by invoking item_free(), if found.
// If the item is not found, dynarray is not modified, and nothing
// is deallocated.
// Returns the number of items removed from dynarray (1, or 0).
int dynarray_delete(
    struct dynarray *dynarray,
    void *item,
    int (*compare)(const void *, const void *),
    void (*item_free)(void *));

// walks through all the items of a dynarray, in their (sorted) order
// invoking visit(...) for each item.
// The first argument to visit(...) will be the item (pointer), and
// the second will always be cookie, which may be NULL. With a help of
// a cookie, the visit(...) function can be stateful.
// Function visit(...) is to return 1 if it wants to break from the
// walking loop (e.g. found the value it needed), or 0 to continue.
// Returns the stop value returned by the callback: a non-zero
// value if the item walking was stopped in the middle, or 0
// if all items were visited.
int dynarray_walk(
    struct dynarray *dynarray,
    int (*visit)(void*, void*),
    void *cookie);



#endif // DYNARRAY_H_INCLUDED
