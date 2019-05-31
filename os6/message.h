#ifndef MESSAGE_H
#define MESSAGE_H

typedef struct _msg{    
    int id;
    int sum;
	int test;
    int action;
    int receiverClient;
    void *requester;
    char text[256];
} message;

#endif
