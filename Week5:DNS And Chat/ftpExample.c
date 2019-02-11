#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<ncurses.h>
#include<term.h>
#include<fcntl.h>
#include<dirent.h>
#include<signal.h>
#include <pwd.h>
#include <grp.h>
#define clearScreen() printf("\033[H\033[J")
#define die(e) do { fprintf(stderr, "%s\n", e); exit(EXIT_FAILURE); } while (0);
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <string.h> 
#define PORT 21
char lastread[1000]={0};
void ftpread(int sock)
{
	char ch;
	int dash=0,index=0;
	while(read(sock,&ch,1)>0)
	{
		printf("%c",ch);
		lastread[index++]=ch;
		if(ch=='-')
			dash=1;
		if(ch=='\n'&&dash==1)
		{
			dash=0;
			bzero(lastread,sizeof(lastread));
		}
		else if(ch=='\n'&&dash==0)
			break;
	}
}
int servConnect(char buffer[])
{
	int sock = 0, sock2 = 0; 
	struct sockaddr_in serv_addr;  
	char buffer2[512]={0};
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		printf("\n Socket creation error \n"); 
		return -1; 
	} 
	memset(&serv_addr, '0', sizeof(serv_addr)); 
	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 
	// Convert IPv4 and IPv6 addresses from text to binary form 
	if(inet_pton(AF_INET,buffer, &serv_addr.sin_addr)<=0) 
	{ 
		printf("\nInvalid address/ Address not supported \n"); 
		return -1; 
	} 
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{ 
		printf("\nConnection Failed \n"); 
		return -1; 
	} 
	ftpread(sock);
	char mesUser[100]="USER ftptest\n";//Replace with your own or use %s for customisation
    char mesPass[100]="PASS ftptest\n";
    if( send(sock, mesUser , strlen(mesUser) , 0) < 0)
    {
        perror("send failed");
        return 1;
    }
	ftpread(sock);
    if( send(sock , mesPass , strlen(mesPass) , 0) < 0)
    {
        perror("send failed");
        return 1;
    }
	ftpread(sock);
	char com1[100]="PASV\n";
	if( send(sock , com1 , strlen(com1) , 0) < 0)
    {
        perror("send failed");
        return 1;
    }
	ftpread(sock);
	char *tok=strtok(lastread,",");
	tok=strtok(NULL,",");
	tok=strtok(NULL,",");
	tok=strtok(NULL,",");
	int a=atoi(strtok(NULL,","));
	int b=atoi(strtok(NULL,","));
	printf("%d %d\n",a,b);
	serv_addr.sin_port = htons(a*256+b);
	if ((sock2 = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		printf("\n Socket creation error \n"); 
		return -1; 
	} 
	if(inet_pton(AF_INET,buffer, &serv_addr.sin_addr)<=0) 
	{ 
		printf("\nInvalid address/ Address not supported \n"); 
		return -1; 
	} 
	if (connect(sock2, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{ 
		printf("\nConnection Failed \n"); 
		return -1; 
	} 
	char com2[100]="put /home/aditya/Documents/Week1/client.c /home/aditya/Desktop/p.c";
	if( send(sock2 , com2 , strlen(com2) , 0) < 0)
    {
        perror("send failed");
        return 1;
    }
	ftpread(sock);
	return 0;
}
int main(int argc,char *argv[])
{
	if(argc!=2)
	{
		printf("Connecting port?\n");
		exit(1);
	}
	servConnect(argv[1]);
	return 0;
}