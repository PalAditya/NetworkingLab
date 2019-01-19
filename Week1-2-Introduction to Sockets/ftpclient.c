// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <arpa/inet.h>
#include <unistd.h>
#define PORT 8080 

int main(int argc, char const *argv[]) 
{ 
	int sock = 0; 
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
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
	{ 
		printf("\nInvalid address/ Address not supported \n"); 
		return -1; 
	} 
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{ 
		printf("\nConnection Failed \n"); 
		return -1; 
	} 
	int i;
	read( sock , buffer2, 512); 
	printf("Available files are: %s\n",buffer2);
	printf("Enter the file number to download\n");
	scanf("%d",&i);
	char buf[20];
	snprintf(buf,20,"%d",i);
	send(sock , buf , strlen(buf) , 0 ); 
	memset(&buf, '\0', sizeof(buf));
	recv(sock , buf,20,0);
	printf("In string form:%s\n",buf);
	long int r=atoi(buf);
	printf("Received filesize: %ld\n",r);
	//printf("%s\n",buffer);
	printf("Save file as...\n");
	char name[100],ch;
	scanf(" %[^\n]",name);
	//char ch;
	FILE *fptr = fopen("download", "w"); 
	while(r-->0)
	{
		read(sock,&ch,1);
		printf("%c",ch);
		fprintf(fptr,"%c",ch);
	}
	fclose(fptr);
	return 0; 
} 