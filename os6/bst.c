#include "bst.h"

int show_cash(BST root,int id){
    BST tmp = find(root,id);
    if(tmp)
        return tmp->money;
    else
        return 0;
}

int get_money(BST root,int id , int money){
    BST tmp = find(root,id);
    if(!tmp)
        return 0;
    if(tmp->money>=money){
        tmp->money-=money;        
    }
    else
        return 0; 
    return 1;
}

void add(BST *root, int newid , int money)
{
    if (!(*root)) {
        BST newNode = (BST) malloc(sizeof(*newNode));
        if (!newNode) {
            printf("Error: no memory\n");
            exit(FAILURE);
        }
        newNode->left = newNode->right = NULL;
        newNode->id = newid;
        newNode->money = money;
        *root = newNode;

        return;
    }
    if(newid == (*root)->id){
        (*root)->money+=money;
        
    } else if (newid < (*root)->id){
        add(&(*root)->left, newid , money);
    } else if(newid > (*root)->id){
        add(&(*root)->right, newid , money);
    }
}

BST find(BST root, int id)
{
    if (!root) {
        return root;
    }

    if (id < root->id) {
        return find(root->left, id);
    } else if (id > root->id) {
        return find(root->right, id);
    } else {
        return root;
    }
}

BST minValueNode(BST root)
{
    BST cur = root;
    while (cur->left)
        cur = cur->left;
    return cur;
}

BST del(BST root, int id)
{
    if (!root)
        return root;
    printf("try delete\n");
    if (id < root->id) {
        root->left = del(root->left, id);
    }
    else if (id > root->id) {
        root->right = del(root->right, id);
    } 
    else {
        if (!root->left) {
            BST tmp = root->right;
            free(root);
            root = NULL;
            printf("kek\n");
            return tmp;
        }

        else if (!root->right) {
            BST tmp = root->left;
            free(root);
            root = NULL;
            printf("kek2\n");
            return tmp;
        }

        BST tmp2,tmp = minValueNode(root->right);
        root->id = tmp->id;
        root->money=tmp->money;
        tmp=root->right;
        while(tmp2!=tmp||tmp2->left!=tmp){
            tmp2=tmp2->left;
        }
        if(tmp2==tmp){
            root->right==tmp->right;
            free(tmp);
        }
        else{
            tmp2->left=tmp->right;
            free(tmp);
        }
        
    }
    printf("succes\n");
    return root;
}

bool empty(BST root)
{
    return !root;
}