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
#define PORT 8080 
int servConnect()
{
    int server_fd, new_socket; 
	struct sockaddr_in address; 
	int opt = 1; 
	int addrlen = sizeof(address);  
	char buffer[20];
	char buffer2[512]="";
	// Creating socket file descriptor 
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
	{ 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	// Forcefully attaching socket to the port 8080 
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( PORT ); 
	// Forcefully attaching socket to the port 8080 
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	}
	if (listen(server_fd, 3) < 0) 
	{ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 
	while(1)
	{
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) 
		{ 
			perror("accept"); 
			exit(EXIT_FAILURE); 
		} 
		if(fork()==0)
		{
			struct sockaddr_in clientAddr;
			socklen_t addr_size = sizeof(struct sockaddr_in);
			int res = getpeername(new_socket, (struct sockaddr *)&clientAddr, &addr_size);
			close(server_fd);
			bzero(buffer2,sizeof(buffer2));
			char *hello="Enter 1 to register,2 to check online friends!";
			strncpy(buffer2,hello,strlen(hello)+1);
			send(new_socket , buffer2 , strlen(buffer2) , 0 );
			bzero(buffer,sizeof(buffer));
			read( new_socket ,buffer, sizeof(buffer));
			int choice=atoi(buffer);
			printf("Choice was: %d\n",choice);
			char user[21];
			bzero(user,21);
			switch(choice)
			{
				case 1:
					;
					printf("Entered choice 1\n");
					char *message="Please provide us with your user name! Limited upto 20 characters only";
					
					printf("Res is: %d\n",res);
					if(res<0)
					{
						perror("Peername"); 
						exit(EXIT_FAILURE); 
					}		
					
					//strcpy(client_ip, inet_ntoa(clientAddr.sin_addr));
					//char client_ip[20];
					//strcpy(client_ip, "127.0.0.1");
					bzero(buffer2,sizeof(buffer2));
					strncpy(buffer2,message,strlen(message)+1);
					printf("%s\n",inet_ntoa(clientAddr.sin_addr));
					send(new_socket , buffer2 , strlen(buffer2) , 0 );
					printf("Entered choice 1 again\n");
					bzero(buffer,sizeof(buffer));
					read( new_socket ,buffer, sizeof(buffer));						
					FILE *fp=fopen("registered.txt","a+");
					strncpy(user,buffer,21);
					fprintf(fp,"%s:%s\n",buffer,inet_ntoa(clientAddr.sin_addr));
					fclose(fp);
					break;
				case 2:
					printf("Entered choice 2\n");
					FILE *fp2=fopen("online.txt","r");
					if(fp2==NULL)
					{
						perror("Failed to open file");
						exit(EXIT_FAILURE);
					}
					char ch;
					while((ch=fgetc(fp2))>0)
					{
						if(send(new_socket,&ch,1,0)<0)
							break;
					}
					fclose(fp);
					break;
				default:
					printf("Not supported yet\n");
					break;
			}
			bzero(buffer,sizeof(buffer));
			read( new_socket ,buffer, sizeof(buffer));
			choice=atoi(buffer);
			if(choice==1)
			{
				printf("The person wishes to chat!\n");
				bzero(buffer,sizeof(buffer));
				read( new_socket ,buffer, sizeof(buffer));
				choice=atoi(buffer);
				printf("Received that choice of chat is with: %d\n",choice);
				FILE *fp3=fopen("online.txt","r");
				int index=0;
				char online[80];
				while(index<choice)
				{
					bzero(online,sizeof(online));
					fgets(online,80,fp3);
					printf("Online: %s\n",online);
					index++;
				}
				int l=strlen(online),i;
				for(i=0;i<l;i++)
				{
					if(online[i]==':')
						break;
				}
				write(new_socket,online+i+1,(l-i-1));
				fclose(fp3);
				printf("Socket closed\n");
			}
			else
			{
				FILE *fp=fopen("online.txt","a+");
				fprintf(fp,"%s:%s\n",user,inet_ntoa(clientAddr.sin_addr));
				fclose(fp);
			}
			close(new_socket);
		}
		else
		{
		    close(new_socket);
		}
	}	
    return 0;
}

int main()
{
	servConnect();
	return 0;
}
