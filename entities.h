#ifndef ENTITIES_H_INCLUDED
#define ENTITIES_H_INCLUDED

#include "dynarray.h"

struct book {
    const char *id; // unique ID
    const char *title;
    const char *author;
};

struct persistence {
    struct dynarray * books;
};


void book_destroy(struct book * book);

int book_compare(struct book * book1, struct book * book2);


// Allocates dynamic memory for struct persistence,
// initializes it to empty entities. Use destroy_persistence()
// to de-allocate all related structure.
struct persistence * persistence_create();

// Deallocates all dynamic memory used by this persistence.
void persistence_destroy(struct persistence *persistence);

// returns a pointer to the newly created book,
// or NULL if a book with the same id already exists
struct book * book_create(
    struct persistence *persistence,
    const char *id,
    const char *title,
    const char *author);


// Finds all books (in persistence) and calls
// callback(book, row-id) for each books.
// If match is specified, then only those books are selected
// that contain the given string - case insensitively - in
// any of the book's attributes (id, title, author, ...).
// If the callback returns non-0 value, no further
// invocations will be made to callback.
int book_find(
    struct persistence *persistence,
    int (*callback)(struct book*, int),
    const char *match);




#endif // ENTITIES_H_INCLUDED
