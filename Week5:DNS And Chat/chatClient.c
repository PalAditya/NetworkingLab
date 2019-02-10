// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#define PORT 8080 
void *readIt(void *vargp)
{
    char arr2[80];
    int fd=*((int *)vargp);
    while (1)
    {
        fgets(arr2, 80, stdin);
        write(fd, arr2, strlen(arr2)+1);    
    }
	close(fd);
    pthread_exit(NULL);
}
void *writeIt(void *vargp)
{
    int fd=*((int *)vargp);
    char arr1[80];
    while(1)
    {
        read(fd, arr1, sizeof(arr1));
        printf("A sent me: %s", arr1);
    }
	close(fd);
    pthread_exit(NULL);
}
void becomeServer(int sock)
{
	int server_fd, new_socket; 
	close(sock);
	struct sockaddr_in address; 
	int opt = 1; 
	int addrlen = sizeof(address);  
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
		int rc;
		int *i = malloc(sizeof(int *));
		*i=new_socket;
		pthread_t tread,twrite;
    	rc=pthread_create(&tread, NULL, readIt,(void *)i);
   	    if(rc<0)
    	    exit(0);
   		rc=pthread_create(&twrite, NULL, writeIt, (void *)i);
    	if(rc<0)
      	  exit(0);	
    	pthread_join(tread,NULL);
    	pthread_join(twrite,NULL);
		close(server_fd);
	}
}
void becomeClient(int sock2)
{
	printf("Who do you wish to chat with?\n");
	int x;
	scanf("%d",&x);
	char buf[20];
	bzero(buf,sizeof(buf));
	snprintf(buf,20,"%d",x);
	write(sock2,buf,strlen(buf));
	bzero(buf,sizeof(buf));
	read(sock2,buf,sizeof(buf));
	printf("Server sent address as: %s\n",buf);
	close(sock2);
	int sock = 0; 
	struct sockaddr_in serv_addr;  
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		printf("\n Socket creation error [2] \n"); 
		exit(EXIT_FAILURE);
	} 
	memset(&serv_addr, '0', sizeof(serv_addr)); 
	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 
	// Convert IPv4 and IPv6 addresses from text to binary form 
	if(inet_pton(AF_INET, "10.0.32.217", &serv_addr.sin_addr)<=0) 
	{ 
		printf("\nInvalid address/ Address not supported [2]\n"); 
		exit(EXIT_FAILURE);
	} 
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{ 
		printf("\nConnection Failed [2]\n"); 
		exit(EXIT_FAILURE);
	} 
	int rc;
	int *i = malloc(sizeof(int *));
	*i=sock;
	pthread_t tread,twrite;
    rc=pthread_create(&tread, NULL, readIt, (void *)i);
    if(rc<0)
       exit(0);
   	rc=pthread_create(&twrite, NULL, writeIt, (void *)i);
    if(rc<0)
       exit(0);
    pthread_join(tread,NULL);
    pthread_join(twrite,NULL);
	close(sock);
}
void startChatting(int sock)
{
	printf("Do you wish to chat (1) or just stay online (2)?\n");
	int choice=0;
	scanf("%d",&choice);
	char buf[20];
	bzero(buf,sizeof(buf));
	snprintf(buf,20,"%d",choice);
	write(sock,buf,strlen(buf));
	if(choice==1)
	{
		becomeClient(sock);
	}
	else
	{
		becomeServer(sock);
	}
	/*pthread_t tread,twrite;
    rc=pthread_create(&tread, NULL, readIt, NULL);
    if(rc<0)
        exit(0);
    rc=pthread_create(&twrite, NULL, writeIt, NULL);
    if(rc<0)
        exit(0);
    pthread_join(tread,NULL);
    pthread_join(twrite,NULL);*/
}
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
	if(inet_pton(AF_INET, "10.0.32.217", &serv_addr.sin_addr)<=0) 
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
	printf("%s\n",buffer2);
	printf("Enter the choice\n");
	scanf("%d",&i);
	char buf[20];
	bzero(buf,sizeof(buf));
	snprintf(buf,20,"%d",i);
	write(sock,buf,strlen(buf));
	if(i==1)
	{	
		bzero(buffer2,sizeof(buffer2));
		read( sock , buffer2, 512);
		printf("%s\n",buffer2);
		bzero(buf,sizeof(buf));
		scanf(" %[^\n]",buf);
		send(sock,buf,strlen(buf),0);
	}
	else
	{
		char ch;
		while(read(sock,&ch,1)>0)
		{
			printf("%c",ch);
		}
	}
	startChatting(sock);
	return 0; 
} 