#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

int sockfd,newfd[2];
void getChoose();
int main(){
	int ret;
	struct sockaddr_in server_addr;
	memset(&server_addr,0,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(5678);
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0){
		exit(1);
	}
	ret = bind(sockfd,(struct sockaddr *)(&server_addr),sizeof(server_addr));
	if(ret<0){
		exit(2);
	}
	ret = listen(sockfd,4);
	if(ret<0) exit(3);
	int i = 0;
	int p;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size = sizeof(clnt_addr);
	while(1){
		if((newfd[i] = accept(sockfd,NULL,NULL))>=0){
			printf("welcome player %d\n",i);
			i++;
		}
	
		if(i==1){
			char str[] = "please wait anonther player\n";
			send(newfd[0],str,sizeof(str),0);
		}
		else if(i==2){
			printf("game start!\n");
			if(!(newfd[0]<0 & newfd[1]<0)){
				while((p=fork())==-1);
				if(p==0){
					char start[] = "start!\n";
					send(newfd[0],start,sizeof(start),0);
					send(newfd[1],start,sizeof(start),0);
					getChoose();
					exit(0);
				}
				else{
					i=0;
					newfd[0] = -1;
					newfd[1] = -1;
				}
			}
		}
	}
}
void getChoose(){
	char buffer1[20];
	char buffer2[20];
	recv(newfd[0],buffer1,20,0);
	recv(newfd[1],buffer2,20,0);
	int val1 = atoi(buffer1);
	int val2 = atoi(buffer2);
	char win[] = "you win!\n";
	char lose[] = "you lose!\n";
	char draw[] = "draw!\n";
	if(val1 == val2){
		send(newfd[0],draw,sizeof(draw),0);
		send(newfd[1],draw,sizeof(draw),0);
	}
	else if((val1-val2)==1|(val1-val2)==-2){
		send(newfd[0],win,sizeof(win),0);
		send(newfd[1],lose,sizeof(lose),0);
	}
	else{
		send(newfd[0],lose,sizeof(lose),0);
		send(newfd[1],win,sizeof(win),0);
	}
}
