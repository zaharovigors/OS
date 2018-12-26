#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <math.h>


// size of file
int filesize( FILE *fp )
{
    int k = 0;
    int s;
    while ((fscanf(fp, "%d ", &s)!=EOF))
    {    if(!fp) break;    //чтобы не делал лишнего
        k+=1;
    }
    return k;
}

int cmpfunc (const void * a, const void * b) {
    return ( *(int*)a - *(int*)b );
}

// sorting for file
int sort(char *fname) {
    int err = 0;
    int sizef;
    if (fname) {
        FILE *f = fopen(fname, "r");
        if (f) {
            sizef = filesize(f);
            fclose(f);
            int *A;
            A = (int*)malloc(sizef * sizeof(int));
            f = fopen(fname, "r");
            for(int i = 0; i < sizef; i++) {
                fscanf(f, "%d ", &A[i]);
            }
            
            fclose(f);
            
            qsort(A, sizef, sizeof(int), cmpfunc);

            f = fopen(fname, "w");
            for (int i = 0; i < sizef; i++) {
                fprintf(f, "%d ", A[i]);
            }
            free(A);
            fclose(f);
        }
        else
            err = 1;
    }
    return err;
}

// function printing for parent process
int parent(char *fname1, char *fname2) {
    int err = 0;
    printf("Printing files : %s %s\n", fname1, fname2);
    if (fname1 && fname2) {
        FILE *f1 = fopen(fname1, "r");
        int size1 = filesize(f1);
        if (f1) {
            FILE *f2 = fopen(fname2, "r");
            int size2 = filesize(f2);
            if (f2) {
                
                f1 = fopen(fname1, "r");
                f2 = fopen(fname2, "r");
                printf("FIRST FILE : \n ");
                int *A1 = (int*)malloc(size1 * sizeof(int));
                for(int i = 0; i < size1; i++){
                    fscanf(f1, "%d", &A1[i]);
                    printf("%d ", A1[i]);
                }
                printf("\nSECOND FILE : \n ");
                int* A2 = (int*)malloc(size2 * sizeof(int));
                for(int i = 0; i < size2; i++){
                    fscanf(f2, "%d", &A2[i]);
                    printf("%d ", A2[i]);
                }
                
                free(A1);
                free(A2);
                fclose(f1);
                fclose(f2);
            }
            else
                err = -2;
        }
        else
            err = -1;
    }
    printf("\n____________________\n");
    printf("Finished printing\n");
    return err;
}

int main(int argc, char **argv) {
    int err = 0;
    if (argc > 2) {
        int pid1 = fork();
        if (pid1 < 0) {
            perror("Fork error: ");
            err = -1;
        }
        else if (!pid1) {
            int pid2 = fork();
            if (pid2 < 0) {
                perror("Fork error: ");
                err = -2;
            }
            else if (!pid2)
                err = sort(argv[2]);
            else {
                err = sort(argv[1]);
                waitpid(pid2, 0, 0);
            }
        }
        else if (pid1) {
            waitpid(pid1, 0, 0);
            err = parent(argv[1], argv[2]);
        }
    }
    return err;
}
