#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <math.h>
#include <sys/mman.h>
#include <pthread.h>

// number of numbers in file
size_t filesize(char *fname) {
	FILE  *fp = fopen(fname, "r");
	size_t sz = 0;
    
    if (fp) {
		int num;
		
		// более универсально, чем сравнивать с EOF
		// тк прервётся и в том случае, если встретит не-int
		while ((fscanf(fp, "%d ", &num) == 1))
			sz++;

		fclose(fp);
	}

    return sz;
}

int cmpfunc (const void * a, const void * b) {
    return ( *(int*)a - *(int*)b );
}

// file sorting 
// size_t - особый тип для хранения размеров
int sort(char *fname, int *ar, size_t sz) {
    int err = 0;
    
    if (fname && ar && sz) {
        FILE *f = fopen(fname, "r");
        
        if (f) {
            for (size_t i = 0; i < sz; i++) 
                fscanf(f, "%d ", &ar[i]);

            qsort(ar, sz, sizeof(int), cmpfunc);

			f = fopen(fname, "w");
            for (size_t i = 0; i < sz; i++)
                fprintf(f, "%d ", ar[i]);

            fprintf(f, "\n");
            
            fclose(f);
        }
        else
            err = 1;
    }
    return err;
}

// printing function for parent process
void parent(int *ar, size_t sz) {
	if (ar && sz > 0) {
		for (size_t i = 0; i < sz; ++i)
			printf("%d ", ar[i]);

		printf("\n");
	}
}

int usage() {
	printf("Usage: ./a.out file1 file2\n");
	return 1;
}

int main(int argc, char **argv) {
    int  err = 0;
    int  s1, s2;
	int *ar;
    
    if (argc == 3) {
		s1 = filesize(argv[1]);
		s2 = filesize(argv[2]);

		// Теперь этот указатель доступен во всех процессах
		ar = mmap(NULL, (s1 + s2) * sizeof(int), PROT_WRITE | PROT_READ, MAP_ANONYMOUS | MAP_SHARED, -1, 0);

		if (!ar)
			printf("Seems no memory was allocated\n");
	
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
				/*
				 * ar + s1 - передача указателя на s1-ый элемент массива
				 * таким образом, ф-иця sort() может начать записывать с ar[s1]
				 * и до конца
				 */
                err = sort(argv[2], ar + s1, s2);
            else {
				/* Ф-ция ниже пишет с ar[0] по ar[s1 - 1] */
                err = sort(argv[1], ar, s1);
                waitpid(pid2, 0, 0);
            }
        }
        else if (pid1) {
			/*
			 * После того, как ф-ции sort() заполнили массив
			 * числами в нужн. порядке, можно его вывести
			 */
            waitpid(pid1, 0, 0);
            parent (ar, s1 + s2);
            /* Освобождение mmap()-нутой памяти */
            munmap (ar, s1 + s2);
        }
    }
    else 
		err = usage();
    
    return err;
}
