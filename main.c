#include <stdio.h>
#include <stdlib.h>
#include <error.h>

#include "entities.h"
#include "test data.h"


int book_print(struct book * book, int row_id) {
    printf("%10s | %s %s\n", book->id, book->author, book->title);
    return 0;
}


int main()
{
    int result;
	struct persistence * persistence = persistence_create();

	populate_test_data(persistence);

    book_find(persistence, &book_print, NULL);
	persistence_destroy(persistence);



    return 0;
}
