#include <stdio.h>  			//printf, scanf
#include <unistd.h>				//for read()			
#include <sys/socket.h>			//for socket functions
#include <stdlib.h>       		//EXIT_FAILURE, memset
#include <string.h>				//strlen()
#include <netinet/in.h>			//for struct sockaddr
#include <pthread.h>			//for multi threading
#include <stdbool.h>			//for boolean var
#include <dirent.h>				//for directories
#include <sys/stat.h>			//for file size
#include <fcntl.h>


struct frame
{
	int data;
	int fid;
};

#define PORT 8080
#define W 3
#define N 5

int main()
{
	int client_socket,ret;
	struct sockaddr_in server_addr;
	int addrlen = sizeof(server_addr);

	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(client_socket == 0)
	{
		perror("socker failed\n");
		exit(EXIT_FAILURE);
	}

	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_family = AF_INET;

	ret = connect(client_socket, 
					(struct sockaddr *)&server_addr, 
						sizeof(server_addr));
	if(ret<0)
	{
		perror("Connection error");
		exit(EXIT_FAILURE);
	}

	int *seq = malloc(sizeof(int));
	*seq=0;
	struct frame temp;

	while(1)
	{
		read(client_socket, (void*)&temp, sizeof(struct frame));
		*seq = temp.fid;
		printf("Recieved %d\n",*seq);
		write(client_socket, seq, sizeof(seq));
	}

	close(client_socket);

	return 0;
}