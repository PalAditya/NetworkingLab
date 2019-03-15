// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <dirent.h>
#define PORT 8080
int main(int argc, char const *argv[]) 
{ 
	int server_fd, new_socket; 
	struct sockaddr_in address; 
	int opt = 1, bytes = 0; 
	int addrlen = sizeof(address); 
	char buffer[1024] = {0},buffcopy[1024]={0};
	// Creating socket file descriptor 
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
	{ 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	// Forcefully attaching socket to the port 8080 
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
												&opt, sizeof(opt))) 
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( PORT ); 
	
	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr *)&address, 
								sizeof(address))<0) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	if (listen(server_fd, 3) < 0) //max size of queue without terminating any other client 
	{ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 
	
	//Equivalent with keep-alive:MaybeNot
	while(1)
	{
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0) 
		{ 
			perror("accept"); 
			exit(EXIT_FAILURE); 
		}
		if(fork()==0)
		{
			close(server_fd);
			bzero(buffer,sizeof(buffer));
			bzero(buffcopy,sizeof(buffer));
			read(new_socket , buffer, 1024);
			memcpy(buffcopy,buffer,sizeof(buffer));//Save the request
			char *token=NULL;
			if(strlen(buffer)>=0)
				token = strtok(buffer, " ");
			if(token!=NULL)
				token = strtok(NULL, " ");//Get the full request
			if(token==NULL)
				continue;
			printf("%s\n%s\n",token,buffcopy);
			char *path="mun18";
			char fullpath[256]={0};
			strcat(fullpath,path);
			strcat(fullpath,token);
			//snprintf(fullpath,path,token);
			printf("Fullpath is:%s\n",fullpath);
			FILE *fp = fopen(fullpath,"r"); 
			if (fp == NULL)  // opendir returns NULL if couldn't open directory 
			{ 
				printf("Not found...\n");
				char *message="HTTP/1.1 404 Not Found\r\nConnection: keep-alive\r\nContent-Type: text/html; charset=ISO-8859-4\r\n\r\n<!DOCTYPE HTML><html><body>Not Found</body></html>";
				send(new_socket,message,strlen(message),0);
				//return 0;
				exit(0);
			} 
			else
			{
				char *messag="HTTP/1.1 200 OK\r\nConnection: keep-alive\r\nContent-Type: %s; charset=ISO-8859-4\r\n\r\n";
				int l=strlen(fullpath);
				char message[256];
				bzero(message,sizeof(message));
				//printf("\nHHHHHHHHH%c\n",fullpath[l-1]);
				if(fullpath[l-1]=='s'&&fullpath[l-2]=='s')
					sprintf(message,messag,"text/css");
				else if(fullpath[l-1]=='l')
					sprintf(message,messag,"text/html");
				else if(fullpath[l-1]=='s')
					sprintf(message,messag,"text/javascript");
				else
					sprintf(message,messag,"image/jpeg");
				send(new_socket,message,strlen(message),0);
				char ch=' ';
				while((bytes=fread(&ch,1,1,fp))==1)
				{
					//printf("%c",ch);
					if(send(new_socket,&ch,1,0)<0)
						perror("Send failed\n");
				}
				close(new_socket);
				exit(0);
			}
		}
		else
		{
			close(new_socket);
		}
	}
	return 0;
} 
