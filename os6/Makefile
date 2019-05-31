all: client server
client: client.c ; gcc client.c -lzmq -o client -pthread
server: bst.o server.c ; gcc server.c bst.o -lzmq -o server
bst.o : bst.c bst.h; gcc -c bst.c
clean: ; rm -f *.o client server

