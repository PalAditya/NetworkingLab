#include<stdio.h>		//fputs
#include<sys/types.h>	//socket, connect
#include<sys/socket.h>	//socket, connect
#include<stdlib.h>	//exit
#include<string.h>	//explicit_bzero
#include<arpa/inet.h>	//inet_pton
#include<unistd.h>	//read
#include<netdb.h>	//getaddrinfo

#define MAXSIZE 1024 //1048576

int main(int argc, char **argv)
{
	if(argc!=4)
	{
		printf("Usage: httpClient <host-name> <port_no.> <relative-path-of-resource-to-be-fetched>\n");
		exit(99);
	}
	
	int sockfd, n;
	char buff[MAXSIZE];
	//uint16_t port;
	
	struct addrinfo hints, *servinfo, *p;
	int rv;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // can use AF_INET6 to force IPv6
	hints.ai_socktype = SOCK_STREAM;

	if((rv = getaddrinfo(argv[1], "http", &hints, &servinfo)) != 0)
	{
	    	printf("getaddrinfo: %s\n", gai_strerror(rv));
	    	exit(1);
	}

	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next)
	{
		if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
		{
			perror("socket");
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
		{
			perror("connect");
			close(sockfd);
			continue;
		}

		break; // if we get here, we must have connected successfully
	}

	if(p==NULL)
	{
		// looped off the end of the list with no connection
		printf("failed to connect\n");
		exit(2);
	}
	
	//char getRequestFormat[1024]="GET /%s HTTP/1.1\r\nHost: %s:%s\r\nAccept: text/html,application/xhtml+xml,application/xml\r\n\r\n";
	char getRequest[1024];
	sprintf(getRequest, "GET /%s HTTP/1.1\r\nHost: %s:%s\r\nAccept: text/html,application/xhtml+xml,application/xml\r\n\r\n", argv[3], argv[1], argv[2]);
	
	write(sockfd, getRequest, strlen(getRequest));
	
	/*char ch;
	while(1)
	{
		if(read(sockfd, &ch, sizeof(char))==-1)
		{
			perror("read");
			break;//exit(7);
		}
		if(ch==EOF)
			break;
		printf("%c", ch);
	}*/
	
	/*while((n=read(sockfd, buff, MAXSIZE))==MAXSIZE)
	{
		printf("%s", buff);
		explicit_bzero(buff, sizeof(buff));
	}
	if(n>0 && n<MAXSIZE)
	{
		buff[n]=0;
		printf("\n\n\nIM HERE\n\n\n%s\n", buff);
	}*/
	
	while((n=read(sockfd, buff, MAXSIZE))>0)
	{
		printf("%s", buff);
		explicit_bzero(buff, sizeof(buff));
	}
	
	close(sockfd);
	freeaddrinfo(servinfo); // all done with this structure
	exit(0);
}
