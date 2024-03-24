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

// Global book and patron objects
book_t *book;
patron_t *patron;

void createObjects(object_db_t *object_db) {
     
    book_t *book2 = xcalloc(object_db, "book_t", 1);
    strncpy(book2->title, "Book 2", sizeof(book2->title));
    strncpy(book2->author, "Author 2", sizeof(book2->author));
    book2->year = 2000;

    book_t *book3 = xcalloc(object_db, "book_t", 1);
    strncpy(book3->title, "Book 3", sizeof(book3->title));
    strncpy(book3->author, "Author 3", sizeof(book3->author));
    book3->year = 2010;

    book_t *book4 = xcalloc(object_db, "book_t", 1);
    strncpy(book4->title, "Book 4", sizeof(book4->title));
    strncpy(book4->author, "Author 4", sizeof(book4->author));
    book4->year = 2020;

    
    book->next = book2;
    book2->next = book3;
    book3->next = book4;
    book4->next = NULL;

  
    patron_t *patron2 = xcalloc(object_db, "patron_t", 1);
    strncpy(patron2->name, "Patron 2", sizeof(patron2->name));
    patron2->age = 30;
    patron2->borrowed_book = book2;

    patron_t *patron3 = xcalloc(object_db, "patron_t", 1);
    strncpy(patron3->name, "Patron 3", sizeof(patron3->name));
    patron3->age = 40;
    patron3->borrowed_book = book3;

    patron_t *patron4 = xcalloc(object_db, "patron_t", 1);
    strncpy(patron4->name, "Patron 4", sizeof(patron4->name));
    patron4->age = 50;
    patron4->borrowed_book = book4;

     
    patron->next = patron2;
    patron2->next = patron3;
    patron3->next = patron4;
    patron4->next = NULL;

     
    book_t *lost_book = xcalloc(object_db, "book_t", 1); 
    strncpy(lost_book->title, "Lost Book", sizeof(lost_book->title));
    strncpy(lost_book->author, "Unknown Author", sizeof(lost_book->author));
    lost_book->year = 2022;
    lost_book->next=NULL;
    xfree(object_db,lost_book,"book_t");
}

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

    
    book = xcalloc(object_db, "book_t", 1);
    patron = xcalloc(object_db, "patron_t", 1);

    
    strncpy(book->title, "Book 1", sizeof(book->title));
    strncpy(book->author, "Author 1", sizeof(book->author));
    book->year = 1990;

  
    strncpy(patron->name, "Patron 1", sizeof(patron->name));
    patron->age = 20;
    patron->borrowed_book = book;

 
    mld_set_dynamic_object_as_root(object_db, book);
    mld_set_dynamic_object_as_root(object_db, patron);

    createObjects(object_db);

    print_object_db(object_db);

    run_mld_algorithm(object_db);
    printf("Leaked Objects: \n");
    report_leaked_objects(object_db);

    return 0;
}

