#include <stdio.h>
#include "entities.h"
#include "test data.h"

static const struct book books[]={
    {"1", "Arany Janos", "Toldi"},
    {"2", "Gardonyi Geza", "Egri csillagok"},
    {"3", "Petofi Sandor", "Janos vitez"},
    {NULL}
};

int populate_test_data(struct persistence *persistence) {
    for (const struct book * book = books; book->id != NULL; book++) {
        struct book * new_book = book_create(
            persistence,
            book->id,
            book->author,
            book->title);
        if (new_book == NULL) {
            printf("Could not add new book %s\n", book->id);
        }
    }
    return 0;
}
