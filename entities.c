#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "entities.h"
#include "dynarray.h"


void book_destroy(struct book * book) {
    if (book != NULL) {
        free((void *)book->id);
        free((void *)book->author);
        free((void *)book->title);
        free((void *)book);
    }
}


struct persistence * persistence_create() {
    struct persistence * persistence = malloc(sizeof(struct persistence));
    if (persistence != NULL) {
        persistence->books = dynarray_create();
        if (persistence->books == NULL) {
            free(persistence);
            persistence = NULL;
        }
    }
    return persistence;
}


void persistence_destroy(struct persistence * persistence) {
    if (persistence != NULL) {
        if (persistence->books != NULL) {
            dynarray_destroy(persistence->books, (void (*)(void*))book_destroy);
        }
        free(persistence);
    }
}


int book_compare(struct book * book1, struct book * book2) {
    return strcmp(book1->id, book2->id);
}


struct book * book_create(
    struct persistence *persistence,
    const char *id,
    const char *author,
    const char *title) {
    struct book *book = malloc(sizeof(struct book));
    if (book != NULL) {
        book->id = strdup(id);
        book->author = strdup(author);
        book->title = strdup(title);

        struct book *new_book = dynarray_insert(
            persistence->books,
            book,
            (int (*)(const void *, const void *))book_compare);

        if (new_book != book) {
            // a car with the same licence plate already exists
            book_destroy(book);
            book = NULL;
        } else if (new_book == NULL) {
            book = NULL;
        }
    }
    return book;
}


struct book_find_cookie {
    int (*callback)(struct book*, int);
    const char * match;
    int row_id;
};


int book_find_walk_action(
    void * item,
    void * cookie) {

    struct book * book = (struct book *)item;
    struct book_find_cookie * c = (struct book_find_cookie *)cookie;

    int stop = 0;
    int relevant =
        c->match == NULL
        || strstr(book->id, c->match) != NULL
        || strstr(book->author, c->match) != NULL
        || strstr(book->title, c->match) != NULL;
    if (relevant) {
        stop = c->callback(book, c->row_id++);
    }
    return stop;
}


int book_find(
    struct persistence *persistence,
    int (*callback)(struct book*, int),
    const char *match) {

    struct book_find_cookie cookie;
    cookie.callback = callback;
    cookie.match = match;
    cookie.row_id = 0;

    dynarray_walk(persistence->books, book_find_walk_action, &cookie);

    return 0;
}
