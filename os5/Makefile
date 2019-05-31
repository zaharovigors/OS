CC = gcc
FLAGS = -std=c99 -pthread -w -pipe -O2 -Wextra -Werror -Wall -Wno-sign-compare -pedantic -lm

all: run

run: liblist.so mainStat.o mainDyn.o
	$(CC) $(FLAGS) -o run-stat mainStat.o -L. -llist -Wl,-rpath,.
	$(CC) $(FLAGS) -o run-dyn mainDyn.o -ldl

mainStat.o: mainStat.c
	$(CC) -c $(FLAGS) mainStat.c

mainDyn.o: mainDyn.c
	$(CC) -c $(FLAGS) mainDyn.c

list.o: list.c
	$(CC) -c -fPIC $(FLAGS) list.c

liblist.so: list.o
	$(CC) $(FLAGS) -shared -o liblist.so list.o

clean:
	rm -f *.o run-stat run-dyn *.so
