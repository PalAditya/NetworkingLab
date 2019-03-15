#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include<arpa/inet.h>
#include<unistd.h>
#include<time.h>
#define SA struct sockaddr
#define N 4
int connectAndReturn(int port)
{
	struct sockaddr_in servaddr; 
	int sockfd;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    bzero(&servaddr, sizeof(servaddr)); 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    servaddr.sin_port = htons(port); 
    // connect the client socket to server socket 
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
        printf("connection with the server failed...\n"); 
        exit(0); 
    } 
	return sockfd;
}
int main(int argc,char *argv[])
{
	int sockfd1=connectAndReturn(8080);
	int i,j,frameno=-1;
	char ch;
	char intbuf[4];
	int arr[N+1];
	for(i=0;i<N+1;i++)
		arr[i]=0;
	for(i=0;i<20;i+=4)
	{
		while(1)
		{
			for(j=0;j<N;j++)
			{
				bzero(intbuf,sizeof(intbuf));
				read(sockfd1,intbuf,sizeof(intbuf));
				if(argc==1)
				printf("Got frameno:%d\n",atoi(intbuf));
				read(sockfd1,&ch,sizeof(char));
				if(arr[atoi(intbuf)]==0)
				{
					arr[atoi(intbuf)]=1;			
					printf("%c\n",ch);
				}
				frameno=(atoi(intbuf)+1)%(N+1);
			}
			int k=0;
			k=rand()%10;
			if(k<5)//set probability
			{
				if(argc==1)
				printf("NO ACK\n");
				continue;
			}
			else
			{
				if(argc==1)
				printf("Sending frameno:%d\n",frameno);
				bzero(intbuf,sizeof(intbuf));
				sprintf(intbuf,"%d",frameno);
				write(sockfd1,intbuf,sizeof(intbuf));
				arr[(frameno)%(N+1)]=0;//Clean array to accept future packets
				arr[(frameno+1)%(N+1)]=0;
				arr[(frameno+2)%(N+1)]=0;
				arr[(frameno+3)%(N+1)]=0;
				break;
			}
		}
	}
	return 0;
}