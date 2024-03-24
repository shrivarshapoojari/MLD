#include "mld.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 

typedef struct book_ {
    char title[100];
    char author[100];
    int year;
    struct book_ *next;
} book_t;

typedef struct patron_ {
    char name[100];
    int age;
    book_t *borrowed_book;
    struct patron_ *next;
} patron_t;

int main() {
    struct_db_t *struct_db = calloc(1, sizeof(struct_db_t));
     
        
    static field_info_t book_fields[] = {
        FIELD_INFO(book_t, title, CHAR, 0),
        FIELD_INFO(book_t, author, CHAR, 0),
        FIELD_INFO(book_t, year, INT32, 0),
        FIELD_INFO(book_t, next, OBJ_PTR, book_t)
    };
    REG_STRUCT(struct_db, book_t, book_fields);

    static field_info_t patron_fields[] = {
        FIELD_INFO(patron_t, name, CHAR, 0),
        FIELD_INFO(patron_t, age, INT32, 0),
        FIELD_INFO(patron_t, borrowed_book, OBJ_PTR, book_t),
        FIELD_INFO(patron_t, next, OBJ_PTR, patron_t)
    };
    REG_STRUCT(struct_db, patron_t, patron_fields);

    print_structure_db(struct_db);

    object_db_t *object_db = calloc(1, sizeof(object_db_t));
    object_db->struct_db = struct_db;

    book_t *book1 =xcalloc(object_db, "book_t", 1);
    strncpy(book1->title, "The Great Gatsby", sizeof(book1->title));
    strncpy(book1->author, "F. Scott Fitzgerald", sizeof(book1->author));
    book1->year = 1925;

    book_t *book2 = xcalloc(object_db, "book_t", 1);
    strncpy(book2->title, "To Kill a Mockingbird", sizeof(book2->title));
    strncpy(book2->author, "Harper Lee", sizeof(book2->author));
    book2->year = 1960;

    patron_t *patron1 =xcalloc(object_db, "patron_t", 1);
    strncpy(patron1->name, "Alice", sizeof(patron1->name));
    patron1->age = 25;

    patron_t *patron2 =xcalloc(object_db, "patron_t", 1);
    strncpy(patron2->name, "Bob", sizeof(patron2->name));
    patron2->age = 30;

    book1->next = book2;
    book2->next = NULL;

    patron1->borrowed_book = book1;
    patron1->next = patron2;

    patron2->borrowed_book =book2;   
    patron2->next = NULL;

    mld_set_dynamic_object_as_root(object_db, book1);
     
    mld_set_dynamic_object_as_root(object_db, patron1);
    
    book_t *lost_book =xcalloc(object_db, "book_t", 1); 
    strncpy(lost_book->title, "Lost Book", sizeof(lost_book->title));
    strncpy(lost_book->author, "Unknown Author", sizeof(lost_book->author));
    lost_book->year = 2022;
    xfree(object_db,lost_book,"book_t");
    print_object_db(object_db);

    run_mld_algorithm(object_db);
    printf("Leaked Objects: \n");
    report_leaked_objects(object_db);

    return 0;
}
