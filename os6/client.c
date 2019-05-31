#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <zmq.h>

#include "message.h"

void usage_user(){
    printf("enter:\n");    
    printf("1) to insert money\n");  
    printf("2) to get money\n");  
    printf("3) to transfer money to another client\n");  
    printf("4) to show cash\n");  
    printf("5) to test server (+1 -1)\n");  
    printf("-1) to exit\n");  

}


void send_recv(message *mes){

    zmq_msg_t request;
    zmq_msg_init_size(&request, sizeof(message));
    memcpy(zmq_msg_data(&request), mes, sizeof(message));
    zmq_msg_send(&request, mes->requester, 0);
    zmq_msg_close(&request);

    zmq_msg_init(&request);
    zmq_msg_recv(&request, mes->requester, 0);
    mes = (message*)zmq_msg_data(&request);
    printf("%s\n", mes->text);
    zmq_msg_close(&request);
}


int main (int argc, char *argv[]) 
{
    message mes;
    char txt[30];
    void* context = zmq_ctx_new();
    if(context == NULL)
        return 0;
    int adress;
    void* request = zmq_socket(context, ZMQ_REQ);
    if(request == NULL)
        return 0;

    printf("Client Starting….\n");
    printf("enter adress of server : ");
    scanf("%d",&adress);
    sprintf(txt,"%s%d","tcp://localhost:",adress);
    if(zmq_connect(request, txt)){
        perror("zmq_connect");
        exit(0);
    }//create socket here   
		int counter=0;
        usage_user();
        char act[20];   
        do{ 
			
            scanf("%s",act);
            mes.action=atoi(act);
            switch(mes.action){
                case 1 :
                        printf("enter sum of money: ");
                        scanf("%d",&mes.sum);
                        if(mes.sum<=0){
                            printf("enter sum > 0\n");
                            break;
                        }
                        mes.id=atoi(argv[1]);
                        mes.requester=request;
                        send_recv(&mes);break;
                case 2 :
                        printf("enter sum of money: ");
                        scanf("%d",&mes.sum);
                        if(mes.sum<=0){
                            printf("enter sum > 0\n");
                            break;
                        }
                        mes.id=atoi(argv[1]);
                        mes.requester=request;
                        send_recv(&mes);break;                
                case 3 :
                        printf("enter sum of money: ");
                        scanf("%d",&mes.sum);
                        printf("enter id of client: ");
                        scanf("%d",&mes.receiverClient);
                        mes.id=atoi(argv[1]);
                        mes.requester=request;
                        send_recv(&mes);break;                
                case 4 :
                        mes.id=atoi(argv[1]);
                        mes.requester=request;
                        send_recv(&mes);break;
				case 5 :
						
						while(counter<100000){
							mes.sum=1;
							mes.id=atoi(argv[1]);
							mes.requester=request;
							mes.action=1;
							send_recv(&mes);
							mes.sum=1;
							mes.id=atoi(argv[1]);
							mes.requester=request;
							mes.action=2;
							send_recv(&mes);
							counter++;
						}counter=0;break;    
                default:break;            
            }    
        }
        while(mes.action!=-1);
        mes.id=atoi(argv[1]);
        send_recv(&mes);
    zmq_close(request);
    zmq_ctx_destroy(context);
    
    return 0;
}