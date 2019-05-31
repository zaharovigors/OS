#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

struct stat buff_2;
char file_name[128];

typedef struct command{
	char *key;
	char todo[256];
}Command;

void replace_word(char *);

void menu(){
	printf("In this text preprocessor you can use the following commands :\n");
	printf("key -r <old>/<new> : for replace old string on new string one time\n");
	printf("key -<N>d : for delete this string in file\n");
	printf("key -d <word>: for delete this word from file\n");
}

int check_command(char *command){
	if(strlen(command)<3 //минимальная длина команды
		&&command[0]!='-'//начало любой команды
		&&command[1]!='r'//замена
		&&(command[1]<48||command[1]>57)//номер строки
		&&command[1]!='d')//удаление слова
		return -1;

	if(command[1]=='r')
		return 1;
	if(command[1]=='d')
		return 2;
	if(command[1]>=48||command[1]<=57)
		return 3;
}

Command* parse_command(char *command,int command_num){
	char todo[50];
	Command *cmd = malloc(sizeof(Command));

	switch(command_num){
		case 1:
			cmd->key="-r";
			strncpy(todo,command+3,strlen(command)-2);
			sprintf(cmd->todo,"%s",todo);
			break;

		case 2:
			cmd->key="-d";
			strncpy(todo,command+3,strlen(command)-2);
			sprintf(cmd->todo,"%s",todo);
		break;
		case 3: 
			cmd->key=command;
		break;
	}
	return cmd;
}
void del_word(char *todo){
	char *todo_2 = malloc(sizeof(todo)+sizeof(char));
	sprintf(todo_2,"%s/",todo);
	replace_word(todo_2);
}

void del_string(char *key){
	int fd = open(file_name,O_RDWR);
	int *ptr = mmap(NULL,buff_2.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	close(fd);

	if(key[strlen(key)-1]!='d')
		return;

	char *tmp = malloc(sizeof(key));
	strncpy(tmp,key+1,strlen(key)-2);
	int number_str = atoi(tmp);
	free(tmp);

	char *temp = malloc(sizeof(*temp));
	sprintf(temp,"%s",(char*)ptr);



	int counter = 0;
	int strings_number = 1;

	while(counter<strlen(temp)){
		if(temp[counter]=='\n')
			strings_number++;
		if(strings_number==number_str)
			break;
		counter++;
	}

	if(number_str>strings_number)
		return;
	int a,b;
	counter = 0;
	strings_number = 1;
	
	while(counter<strlen(temp)){
		if(temp[counter]=='\n')
			strings_number++;
		if(strings_number==number_str){
			break;
		}
		counter++;
	}
	a = counter;
	while(counter<strlen(temp)){
		counter++;
		if(temp[counter]=='\n')
			break;
	}

	b = counter;
	printf("%d %d\n",a,b);
	printf("%s\n",temp);

	char before[256];
	memset(before,0,256);

	char after[256];
	memset(after,0,256);
	int i=0;

	if(a==0)
	for(i=0;i<a;i++){
		before[i]=temp[i];
	}
	if(a!=0)
	for(i=0;i<=a;i++){
		before[i]=temp[i];
	}

	for(i=b;i<strlen(temp);i++){
		after[i-b]=temp[i+1];
	}

	printf("before %s after %s\n",before,after);
	memset(ptr,0,strlen((char*)ptr));//очищаем память
	sprintf((char*)ptr,"%s%s",before,after);
}

void replace_word(char *todo){

	int fd = open(file_name,O_RDWR);
	int *ptr = mmap(NULL,buff_2.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	close(fd);
	
	int counter=0;
	while(counter<strlen(todo)){//поиск разделителя
		if(todo[counter]=='/')
			break;
		counter++;
	}
	if(counter==strlen(todo)-1&&todo[strlen(todo)-1]!='/'){
		printf("error in command (enter slash between old and new words)\n");
		return;
	}

	char old[50],new[50];
	memset(old,0,50);
	memset(old,0,50);

	strncpy(old,todo,counter);
	strncpy(new,todo+counter+1,strlen(todo)-counter);

	char *find = malloc(sizeof(ptr));
	find = strstr((char*)ptr,old);

	if(find==NULL)
		return;

	counter = strlen((char*)ptr) - strlen(find);

	char *before = malloc(sizeof(*before));
	char *after = malloc(sizeof(*after));

	strncpy(before,(char*)ptr,counter);
	strncpy(after,(char*)ptr+counter+strlen(old),strlen((char*)ptr) - counter - strlen(old));

	memset(ptr,0,strlen((char*)ptr));//очищаем память
	sprintf((char*)ptr,"%s%s%s",before,new,after);

}
void change_file(char *file_name,char *command){

	Command *cmd = NULL;

	int command_num = check_command(command);

	if(command_num>0){
		cmd = parse_command(command,command_num);
	}

	if(!strcmp(cmd->key,"-r")){
		replace_word(cmd->todo);
	}
	if(!strcmp(cmd->key,"-d")){
		del_word(cmd->todo);
	}
	else{
		del_string(cmd->key);
	}
}


char *find_command(){
	int fd = open("history.txt",O_RDWR);
	lseek(fd,0,0);
	pid_t pid;

	int offset=0,i=0;
	char cash_commands[50][50];//запросы из кэша
	memset (cash_commands, 0, sizeof(cash_commands));//обнуляем массив
	char *command = malloc(sizeof(char)*50),c;
	printf("enter your choice and press 'y' or press 'n' to enter new command\n");
	while(read(fd,&c,1)>0){//заполняем массив данными из кэша
		if(c=='\n'){
			i++;
		}
		else{
			strcat(cash_commands[i],&c);
		}
	}
	offset=0;
	printf("command from cash : %s\n",cash_commands[offset]);
	offset++;
	scanf("%s",command);
	while(strcmp(command,"n")&&strcmp(command,"y")&&offset<i){
		pid = fork();
		if(pid==0){
			execl("/usr/bin/clear","./clear",NULL);//запускаем очитску экрана
			exit(0);
		}
		else{
			wait(0);
			printf("command from cash : %s\n",cash_commands[offset]);
			offset++;
			scanf("%s",command);
		}
	}
	offset--;
	if(!strcmp(command,"y"))
		strcpy(command,cash_commands[offset]);
	else{
		printf("enter command\n");
		scanf("%s",command);
	}
	printf("end with %s\n",command);
	close(fd);
	return command;
}

int main(int argc,char* argv[]){

	menu();
	
	if(argc<2){
		printf("enter file and command!\n");
		return 0;
	}

	strcpy(file_name,argv[1]);

	int fd = open(argv[1],O_RDWR,0666);//открываем файл для изменения

	if(fd<0){
		perror("open");
		exit(-1);
	}

	int info_file_2 = fstat(fd,&buff_2);//узнаем размер файла
	if(info_file_2!=0){
		perror("fstat file");
		exit(-1);
	}
	close(fd);

	struct stat buff;

	int history = open("history.txt",O_RDWR|O_CREAT,0666);//открываем файл с историей запросов

	int info_file = fstat(history,&buff);//узнаем размер файла с кэшэм
	if(info_file!=0){
		perror("fstat cash");
		exit(-1);
	}
	int *ptr_history = mmap(0,buff.st_size,PROT_WRITE|PROT_READ,MAP_SHARED,history,0);


	lseek(history,buff.st_size,0); //сдвигаем в файл дескрипт указатель на позицию в файле на "2ой арг"
	char command[50];
	if(argc==3){
		strcpy(command,argv[2]);
		if(strstr((char*)ptr_history,command)==NULL){
			ftruncate(history,buff.st_size+strlen(argv[2]));//увеличиваем размер файла с кэшэм
			write(history,argv[2],strlen(argv[2]));
			write(history,"\n",1);
		}
	}
	else{
		
		printf("enter option for change file or enter n for find comand in history\n");
		scanf("%s",command);
		if(!strcmp(command,"n"))
			strcpy(command,find_command());
		ftruncate(history,buff.st_size+strlen(command));
		if(strstr((char*)ptr_history,command)==NULL){
			printf("no\n");
			write(history,command,strlen(command));
			write(history,"\n",1);
		}
	}
	close(history);
	//int *ptr = mmap(0,buff_2.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	change_file(argv[1],command);//меняем сам файл

	printf("succes\n");
	return 0;
}
