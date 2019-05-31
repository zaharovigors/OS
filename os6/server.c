#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#include <time.h>
#include <sys/ioctl.h>
//#include <stropts.h>

#include "zmq.h"
#include "message.h"
#include "bst.h"

#define ppl_number 100
#define file_name "file.txt"

volatile sig_atomic_t flag = 0;

typedef struct ppls{
	int id;
	int money;
}PPL;

void quit_server_func()
{
	printf("\nserver quit\n");
	flag = 3;
}

int main (int argc, char *argv[]) 
{	

	int cash,num_of_people=0,index=0;
	char txt[20];
	int counter=0;
	PPL ppl[ppl_number];
	FILE* fd = fopen(file_name,"rw+");
	if(fd!=NULL){
		while(fscanf(fd,"%d%d",&ppl[counter].id,&ppl[counter].money)!=EOF){
			counter++;
		}
	}
	remove(file_name);
	index=counter;
    BST Persons=NULL;

    
	zmq_msg_t request;
    zmq_msg_t reply;
	void* context = zmq_ctx_new();
	if(context == NULL)
		return 0;
    message *mes;
	void* respond = zmq_socket(context, ZMQ_REP);
	if(respond == NULL)
		return 0;
	if(argc==2){
		sprintf(txt,"%s%s","tcp://*:",argv[1]);
		if(zmq_bind(respond, txt)==-1)
			return 0;
	}
	else
		if(zmq_bind(respond, "tcp://*:4040")==-1)
			return 0;

	signal(SIGINT,quit_server_func);
		while(flag!=3){
				zmq_msg_init(&request);
				zmq_msg_recv(&request, respond, 0);
				mes=(message*)zmq_msg_data(&request);
				zmq_msg_close(&request);
				zmq_msg_init_size(&reply,sizeof(message));
				if(flag==3)break;

				int flag_1=0,ind=0;
				switch(mes->action){ 		
					case 1 :
						
						if(!find(Persons,mes->id)){
							for(counter=0;counter<index;counter++){
								if(ppl[counter].id==mes->id){
									
									ppl[counter].money+=mes->sum;
									
									flag_1=1;
									ind=counter;
								}
							}
							if(flag_1==0){
								ppl[index].id=mes->id;
								ppl[index].money=mes->sum;
								index++;	
							}
							num_of_people++;
						}
						if(flag_1>0)
							add(&Persons,mes->id,ppl[ind].money);
						else
							add(&Persons,mes->id,mes->sum);
						
						strcpy(mes->text,"successful inserting money");
						if(mes->test!=1)
						printf("id : %d ; insert sum : %d\n",mes->id,mes->sum);
						memcpy(zmq_msg_data(&reply),mes,sizeof(message));
						zmq_msg_send(&reply, respond, 0);
						zmq_msg_close(&reply);  break;


					case 2 :

						if(get_money(Persons,mes->id,mes->sum)){
							
							counter=0;
							for(counter;ppl[counter].id!=mes->id;counter++){}
							ppl[counter].money-=mes->sum;
							strcpy(mes->text,"successful getting money");
							printf("id : %d ; get sum : %d\n",mes->id,mes->sum);
						}
						else{
							printf("id : %d ; money < ask \n",mes->id);
							strcpy(mes->text,"money on accaunt less than you ask");
						}
						memcpy(zmq_msg_data(&reply),mes,sizeof(message));
						zmq_msg_send(&reply, respond, 0);
						zmq_msg_close(&reply);  break;            
					case 3 :
						if(find(Persons,mes->receiverClient)){
							if(get_money(Persons,mes->id,mes->sum)){

								counter=0;
								for(counter;ppl[counter].id!=mes->id;counter++){}
								ppl[counter].money-=mes->sum;

								add(&Persons,mes->receiverClient,mes->sum);

								counter=0;
								for(counter;ppl[counter].id!=mes->receiverClient;counter++){}
								ppl[counter].money+=mes->sum;

								strcpy(mes->text,"successful transmiting");
								printf("id : %d to id : %d ; transfer sum : %d\n",mes->id,mes->receiverClient,mes->sum);
							}      
						}
						else{
							strcpy(mes->text,"no such client");
							printf("id : %d ; dosnt exist\n",mes->receiverClient);
						}
							  

						memcpy(zmq_msg_data(&reply),mes,sizeof(message));
						zmq_msg_send(&reply, respond, 0);
						zmq_msg_close(&reply);  break;
					case 4 :
						cash=show_cash(Persons,mes->id);
						printf("id : %d ; sum on accaunt : %d\n",mes->id,cash);
						sprintf(mes->text,"%s %d","your cash is :",cash);
						memcpy(zmq_msg_data(&reply),mes,sizeof(message));
						zmq_msg_send(&reply, respond, 0);
						zmq_msg_close(&reply);break;            
					case -1 :
						if(find(Persons,mes->id)){
							num_of_people--;
						}
						del(Persons,mes->id);
						strcpy(mes->text,"exit client");	
						memcpy(zmq_msg_data(&reply),mes,sizeof(message));
						zmq_msg_send(&reply, respond, 0);
						zmq_msg_close(&reply);
						break;
				}	
		if(!num_of_people)
		    flag=3;
		}

		counter=0;
		fd = fopen(file_name,"a+");
		printf("%d %d \n",counter,index);
		while(counter!=index){
			fprintf(fd,"%d %d ",ppl[counter].id,ppl[counter].money);
			counter++;
		}
		for(counter=0;counter<index;counter++){
			printf("id %d sum %d\n",ppl[counter].id,ppl[counter].money);
		}
	zmq_close(respond);
	zmq_ctx_destroy(context);
	
	return 0;
}
