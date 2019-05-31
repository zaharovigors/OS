#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "list.h"

list* list_create(){
    list* l = (list*)malloc(sizeof(list));
    if(l == NULL) return l;
    l->first=NULL;
    l->last = NULL;
    l->size = 0;
    return l;
}

size_t size(list* l){
    return l->size;
}

void list_destroy(list* l){
    list_el* cur = l->first;
    while (cur != NULL){
        list_el* n = cur->next;
        free(cur);
        cur = n;
    }
    free(l);
}

iter list_begin_iter(list* l){
    
    iter it;
    it.l = l;
    it.cur = NULL;
    it.prev = NULL;
    return it;
}

bool it_has_next(iter* it){
    if(it->cur != NULL) return (it->cur->next != NULL);
    return (it->l->first != NULL);
}

void it_move_next(iter* it){
    if ( it->cur != NULL){
        it->prev = it->cur;
        it->cur = it->cur->next;
    }else{
        it->cur = it->l->first;
    }
}

int it_get_val(iter* it){
    return it->cur->val;
}

void it_set_val(iter* it, int v){
    it->cur->val = v;
}

void it_delete(iter* it){
    if (it->cur == NULL) return;
    if (it->prev == NULL){
        it->l->first = it->cur->next;
        it->prev = it->cur;
        it->cur = it->cur->next;
        free(it->prev);
        it->prev = NULL;
        it->l->size -= 1;
        return;
    }
    it->prev->next = it->cur->next;
    free(it->cur);
    it->cur = it->prev->next;
    it->l->size -= 1;
}

bool it_insert(iter* it, int v){
    list_el* el = (list_el*)malloc(sizeof(list_el));
    if(el == NULL){
        return false;
    }
    el->val = v;
    if(it->cur == NULL){
        el->next = it->l->first;
        it->l->first = el;
        if(it->l->last == NULL){
            it->l->last = el;
        }
        it->l->size++;
    }else{
        el->next = it->cur->next;
        it->cur->next = el;
        if (el->next == NULL){
            it->l->last=el;
        }
        it->l->size++;
    }
    it_move_next(it);
    return true;
}

bool insert(list* l, int place, int v){
    if (l == NULL) return false;
    if (place > l->size + 1) return false;
    iter it = list_begin_iter(l);
    for(int i = 1; i < place; ++i){
        it_move_next(&it);
    }
    if (it_insert(&it,v)) return true;
    return false;
}

iter get_iter_place(list* l, int place){
    iter it = list_begin_iter(l);
    for(int i = 1; i <= place; ++i){
        it_move_next(&it);
    }
    return it;
}

iter get_iter(list* l, int v){
    iter it = list_begin_iter(l);
    for (int i = 0; i < l->size; ++i){
        it_move_next(&it);
        if (v == it_get_val(&it)) return it;;
    }
    (&it)->cur = NULL;
    return it;
}

void print(list* l){
    if(l == NULL || l->size == 0){
        return;
    }
    iter it = list_begin_iter(l);
    if (it_has_next(&it)) it_move_next(&it);
    while (it_has_next(&it)){
        printf("%d ", it_get_val(&it));
        it_move_next(&it);
    }
    printf("%d\n", it_get_val(&it));
}
