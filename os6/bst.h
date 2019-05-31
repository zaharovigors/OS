#ifndef _BST_H_
#define _BST_H_

#define SUCCESS 0
#define FAILURE 1

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>


typedef struct _bst {
    struct _bst *left;
    struct _bst *right;
    int id;
    int money;
} *BST;

void add(BST *root, int newid , int money);
BST find(BST root, int id);
BST del(BST root, int id);
BST minValueNode(BST root);
bool empty(BST root);
int show_cash(BST root,int id);
int get_money(BST root,int id,int money);

#endif /* _BST_H */