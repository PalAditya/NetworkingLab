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
int main()
{
	int sockfd1=connectAndReturn(8080);
	int i,frameno=-1;
	char ch;
	char intbuf[4];
	for(i=0;i<12;)
	{
		bzero(intbuf,sizeof(intbuf));
		read(sockfd1,intbuf,sizeof(intbuf));
		printf("Got frameno:%d\n",atoi(intbuf));
		while(1)
		{
			read(sockfd1,&ch,sizeof(char));
			if(frameno<atoi(intbuf))
			{
				int k=0;
				k=rand()%10;
				if(k<8)//set probability
				{
					printf("NO ACK\n");
					continue;
				}
				printf("%c\n",ch);
				i++;
				//break;
				frameno=atoi(intbuf)+1;
				printf("Sending frameno:%d\n",frameno);
				bzero(intbuf,sizeof(intbuf));
				sprintf(intbuf,"%d",frameno);
				write(sockfd1,intbuf,sizeof(intbuf));
				break;
			}
		}
				
		/*while(1)
		{
			if(frameno==!atoi(intbuf))
			{
				printf("%c\n",ch);
				frameno=!frameno;
				bzero(intbuf,sizeof(intbuf));
				sprintf(intbuf,"%d",frameno);
				write(sockfd1,intbuf,sizeof(intbuf));
				break;
			}
			bzero(intbuf,sizeof(intbuf));
			sprintf(intbuf,"%d",frameno);
			write(sockfd1,intbuf,sizeof(intbuf));
			bzero(intbuf,sizeof(intbuf));
			read(sockfd1,intbuf,sizeof(intbuf));
			printf("Got frameno:%d\n",atoi(intbuf));
			read(sockfd1,&ch,sizeof(char));
		}*/
	}
	return 0;
}