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
	
	int server_socket,client_socket,ret;
	struct sockaddr_in server_addr;
	int addrlen = sizeof(server_addr);

	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(server_socket == 0)
	{
		perror("socker failed\n");
		exit(EXIT_FAILURE);
	}

	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_family = AF_INET;

	ret = bind(server_socket, (struct sockaddr *)&server_addr, 
								sizeof(server_addr));
	if(ret<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	ret = listen(server_socket, 3);
	if(ret<0)
	{
		perror("listen failed");
		exit(EXIT_FAILURE);
	}

	client_socket = accept(server_socket, 
						(struct sockaddr *)&server_addr,
								(socklen_t*)&addrlen);

	int status = fcntl(client_socket, F_SETFL, fcntl(client_socket, F_GETFL, 0) | O_NONBLOCK);

	if (status == -1){
	  perror("calling fcntl");
	}

	struct frame f[N];
	int ack[N];
	int sent=0;
	int start=0,end=2;

	int i,j;
	for(i=0;i<N;i++)
	{
		f[i].data=i;
		f[i].fid=i;
		ack[i]=0;
	}

	while(sent<N)
	{
		for(j=start;j<=end;j++)
		{
			if(!ack[j])
			{
				printf("Sent %d\n",f[j].fid);
				write(client_socket, (void*)&f[j], sizeof(struct frame));
			}
		}

		time_t begin, close;
		double elapsed;  // seconds
		begin = time(NULL);
		int terminate=1, valread=0;

		int *seq = malloc(sizeof(int));
		while (terminate) 
		{
			close = time(NULL);
			elapsed = difftime(close, begin);
			if(elapsed >= 3) terminate = 0;
		 	else 
		 	{
		 		valread=read(client_socket, seq, sizeof(seq));
		 		if(valread>0)
		 		{
		 			printf("Recieved %d\n",*seq);
		 			ack[*seq]=1;
		 			sent++;
		 		}
		 		valread=0;
		 		*seq=0;
		 	}
		}

		printf("Timeout\n");
		for(i=0;i<N;i++)
			printf("%d ",ack[i]);
		printf("\n");

		start=0;
		while(ack[start]) start++;
		end=(start+W-1>N-1)?N-1:start+W-1;
	}

	close(client_socket);
	close(server_socket);

	return 0;
}
