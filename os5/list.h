#ifndef _LIST_H_
#define _LIST_H_
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct list_el list_el;

struct list_el{
    int val;
    list_el* next;
};

typedef struct {
    list_el* first;
    list_el* last;
    size_t size;
} list;

typedef struct {
    list* l;
    list_el* prev;
    list_el* cur;
} iter;

list* list_create();
size_t size(list* l);
void list_destroy(list* l);
iter list_begin_iter(list* l);
bool it_has_next(iter* it);
void it_move_next(iter* it);
int it_get_val(iter* it);
void it_set_val(iter* it, int v);
void it_delete(iter* it);
bool it_insert(iter* it, int v);
bool insert(list* l, int place, int v);
iter get_iter_place(list* l, int place);
iter get_iter(list* l, int v);
void print(list* l);

#endif
