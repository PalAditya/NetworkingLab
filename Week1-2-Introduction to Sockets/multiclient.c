#include<stdio.h>		//fputs
#include<sys/types.h>	//socket, connect
#include<sys/socket.h>	//socket, connect
#include<stdlib.h>	//exit
#include<string.h>	//explicit_bzero
#include<arpa/inet.h>	//inet_pton
#include<unistd.h>	//read
#include<time.h>		//time

#define MAXCLIENTQ 10	//backlog
#define MAXLEN 50

int main(int argc, char **argv)
{
	int listenfd, connfd;
	struct sockaddr_in servaddr;
	char buff[MAXLEN];
	time_t ticks;
	if((listenfd=socket(AF_INET, SOCK_STREAM, 0))==-1)
	{
		perror("socket");
		exit(1);
	}
	
	explicit_bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servaddr.sin_port=htons(7999);
	if(bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr))==-1)
	{
		perror("bind");
		exit(2);
	}
	if(listen(listenfd, MAXCLIENTQ)==-1)
	{
		perror("listen");
		exit(3);
	}
	
	while(1)
	{
		if((connfd=accept(listenfd, (struct sockaddr *)NULL, NULL))==-1)
		{
			perror("accept");
			exit(4);
		}
		
		if(fork()==0)
		{
			close(listenfd);
			ticks=time(NULL);
			snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
			write(connfd, buff, strlen(buff));
			close(connfd);
			exit(0);
		}
		close(connfd);
	}
}
