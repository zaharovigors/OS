#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include <dlfcn.h>

#define FAILURE 1

int main() {
    
    list *(*n_list_create)();
    bool (*list_insert)(list* l, int place, int v);
    iter (*list_get_iter_place)(list* l, int place);
    void (*list_print)(list* l);
    void (*n_list_destroy)(list* l);
    void (*list_it_delete)(iter* it);
    
    
    void *libHandle;
    libHandle = dlopen("./liblist.so", RTLD_LAZY);
    if (!libHandle) {
        fprintf(stderr, "%s\n", dlerror());
        exit(FAILURE);
    }
    
    n_list_create = dlsym(libHandle, "list_create");
    list_insert= dlsym(libHandle,"insert");
    list_get_iter_place=dlsym(libHandle, "get_iter_place");
    list_print=dlsym(libHandle,"print");
    n_list_destroy=dlsym(libHandle,"list_destroy");
    list_it_delete=dlsym(libHandle,"it_delete");
    
//    if(err = dlerror()) {
//        fprintf(stderr, "%s\n", err);
//        exit(FAILURE);
//    }
    
    list* l;
    l = n_list_create();
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
                list_insert(l, i, val);
                i++;
                break;
            }
            case 2: {
                int place;
                printf("введите индекс: ");
                scanf("%d",&place);
                iter it;
                it = list_get_iter_place(l, place);
                list_it_delete(&it);
                break;
            }
            case 3: {
                list_print(l);
                break;
            }
            case 4: {
                n_list_destroy(l);
                break;
            }
        }
        
    } while(n != 0);
    return 0;
}
