#include<stdio.h>		//fputs
#include<sys/types.h>	//socket, connect
#include<sys/socket.h>	//socket, connect
#include<stdlib.h>	//exit
#include<string.h>	//explicit_bzero
#include<arpa/inet.h>	//inet_pton
#include<unistd.h>	//read
#include<netdb.h>	//getaddrinfo

#define MAXSIZE 1024 //1048576
void error(const char *msg) { perror(msg); exit(0); }
int main(int argc, char **argv)
{
	if(argc!=4)
	{
		printf("Usage: httpClient <host-name> <port_no.> <relative-path-of-resource-to-be-fetched>\n");
		exit(99);
	}
	
	int sockfd, n,bytes,sent=0,total=0;
	char links[MAXSIZE],message[MAXSIZE];
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
	char _buf[256];
	int print=0;
	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next)
	{		
		start:bzero(_buf,sizeof(_buf));
		inet_ntop(p->ai_family,p->ai_addr,_buf,p->ai_addrlen);
		//printf("%s\n",_buf);
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
		char getRequest[1024];
		sprintf(getRequest, "GET /%s HTTP/1.1\r\nHost: %s:%s\r\nAccept: text/html,application/xhtml+xml,application/xml\r\n\r\n", argv[3], argv[1], argv[2]);
		write(sockfd, getRequest, strlen(getRequest));
		/*char *detect="href=";
		int detect_index=0;
		char links[256];
		char ch;*/
		int counter=0,code=0;
		while(1)
		{
			counter++;
			if(ch=='<')
				print=1;
			if(print)
				printf("%c", ch);
			if(read(sockfd, &ch, sizeof(char))!=1)
			{
				break;//exit(7);
			}
			/*if(ch==detect[detect_index])
			{
				detect_index++;
				if(detect_index==5)
				{
					detect_index=0;
					bzero(links,sizeof(links));
					while(1)
					{
						bytes=read(sockfd,&ch,1);
						printf("%c",ch);
						if(ch=='>')
						{
							detect_index--;
							links[detect_index]='\0';
							//printf("\n\nLinks are: %s \n\n",links);
							//sleep(2);
							if(links[detect_index-1]=='s'&&links[detect_index-2]=='s')
							{
								bzero(message,sizeof(message));
								sprintf(message,"GET /%s HTTP/1.1\r\nHost: %s:%s\r\nAccept: text/html,application/xhtml+xml,application/xml\r\n\r\n",links,argv[1],argv[2]);
								total = strlen(message);
								sent = 0;
								//printf("%s\n",message);
								do {
									bytes = write(sockfd,message+sent,total-sent);
									if (bytes < 0)
										error("ERROR writing message to socket");
									if (bytes == 0)
										break;
									sent+=bytes;
								} while (sent < total);*/
								/*FILE *fp=fopen("testing.html","w");
								fprintf(fp,"%s","Sent GET request!");
								//sleep(2);
								fprintf(fp,"%s","<style></br>");
								while((bytes=read(sockfd,&ch,1))==1)
									fprintf(fp,"%c",ch);
								fprintf(fp,"%s","</style></br>");*/
							/*}
							//printf("\nOne link is: %s\n",links);
							detect_index=0;
							break;
							goto outer;
						}
						else
						links[detect_index++]=ch;						
					}
				}
			}
			else
			{
				detect_index=0;
			}*/
			if(counter>=10&&counter<=12)
				code=code*10+(int)ch-48;
			if(counter==12&&code==301)
			{
				printf("\nClosed 1\n");
				p=p->ai_next;
				close(sockfd);
				if(p!=NULL)
					goto start;
				else
					break;
			}	
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
	
	/*while((n=read(sockfd, buff, MAXSIZE))>0)
	{
		printf("%s", buff);
		explicit_bzero(buff, sizeof(buff));
	}*/
	outer:close(sockfd);
	freeaddrinfo(servinfo); // all done with this structure
	exit(0);
}

