// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}
int main(int argc, char const *argv[]) 
{ 
	int port;
	int server_fd, new_socket; 
	struct sockaddr_in address; 
	int opt = 1; 
	int addrlen = sizeof(address); 
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
	{ 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	printf("Enter portno\n");
	scanf("%d",&port);
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons(port); 
	
	// Forcefully attaching socket to the port 8080 
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	if (listen(server_fd, 3) < 0) //max size of queue without terminating any other client 
	{ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) 
	{ 
		perror("accept"); 
		exit(EXIT_FAILURE); 
	} 
	char *ack="Received";
	write(new_socket,ack,sizeof(ack));
	//Static example
	/*int i,arr[250];
	for(i=0;i<250;i++)
		read(new_socket,&arr[i],sizeof(int));
	qsort(arr, 250, sizeof(int), cmpfunc);
	for(i=0;i<250;i++)
		write(new_socket,&arr[i],sizeof(int));*/
	int i,*arr;
	int size=0;
	read(new_socket,&size,sizeof(int));
	arr=malloc(size*sizeof(int));
	for(i=0;i<size;i++)
		read(new_socket,&arr[i],sizeof(int));
	qsort(arr, size, sizeof(int), cmpfunc);
	for(i=0;i<size;i++)
		write(new_socket,&arr[i],sizeof(int));
	return 0; 
} 
