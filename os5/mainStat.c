#include <stdio.h>
#include <stdlib.h>
#include "list.h"

int main() {
    list* l;
    l = list_create();
    int i = 1;
    int n;
    do {
        printf("\n выберите действие\n");
        printf(" 1- вставить элемент в список\n");
        printf(" 2- удалить по индексу\n");
        printf(" 3- распечатать список\n");
        printf(" 4- удалить список\n");
        printf(" 0- завершение программы\n");
        scanf("%d",&n);
        
        switch (n) {
            case 1: {
                int val;
                printf("введите значение: ");
                scanf("%d",&val);
                insert(l, i, val);
                i++;
                break;
            }
            case 2: {
                int place;
                printf("введите индекс: ");
                scanf("%d",&place);
                iter it;
                it = get_iter_place(l, place);
                it_delete(&it);
                break;
            }
            case 3: {
                print(l);
                break;
            }
            case 4: {
                list_destroy(l);
                break;
            }
        }
        
    } while(n != 0);
return 0;
}
