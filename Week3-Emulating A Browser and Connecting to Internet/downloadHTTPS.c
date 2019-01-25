#include <stdio.h> /* printf, sprintf */
#include <stdlib.h> /* exit */
#include <unistd.h> /* read, write, close */
#include <string.h> /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h> /* struct hostent, gethostbyname */
#include <openssl/ssl.h>
#include <sys/types.h>
#include <sys/wait.h>
void error(const char *msg) { perror(msg); exit(0); }

int main(int argc,char *argv[])
{
	char *base="GET %s HTTP/1.1\r\nConnection: Close\r\n\r\n";
    int sockfd, bytes, sent, total,index=0;
    char message[256];
    if (argc < 3) { puts("Parameters: Hostname Subindex"); exit(0); }
    sprintf(message,base,argv[2]); 
	struct addrinfo hints, *servinfo, *p;
	int rv;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // use AF_INET6 to force IPv6
	hints.ai_socktype = SOCK_STREAM;
	if ((rv = getaddrinfo(argv[1], "443", &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		exit(1);
	}
	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("socket");
			continue;
		}
		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			perror("connect");
			close(sockfd);
			continue;
		}
		break; // if we get here, we must have connected successfully
	}
	if (p == NULL) {
		// looped off the end of the list with no connection
		fprintf(stderr, "failed to connect\n");
		exit(2);
	}
	freeaddrinfo(servinfo);
	SSL_load_error_strings ();
	SSL_library_init ();
	SSL_CTX *ssl_ctx = SSL_CTX_new (SSLv23_client_method ());
	SSL *conn = SSL_new(ssl_ctx);
	SSL_set_fd(conn, sockfd);
	int err = SSL_connect(conn);
	if (err != 1)
	    abort();	
	total = strlen(message);
	sent = 0;
	do
	{
		bytes = SSL_write(conn,message+sent,total-sent);
		if (bytes < 0)
			error("ERROR writing message to socket");
		if (bytes == 0)
			break;
		sent+=bytes;
	} while (sent < total);
	char ch=' ';
	char fill_line[1000];
	//int count=0,str_index=0,extract=0,build=0;
	//char *str="Content-Length:";
	while((bytes=SSL_read(conn,&ch,1))==1)
	{
		printf("%c",ch);
		if(ch!='\r'&&ch!='\n')
			fill_line[index++]=ch;
		else
		{
			fill_line[index]='\0';
			printf("</br>Line: %s %d</br> ",fill_line,strncmp("Location:",fill_line,9));
			if(strncmp("Location:",fill_line,9)==0)
			{
				char new_url[60];
				bzero(new_url,sizeof(new_url));
				strncpy(new_url,fill_line+10,59);
				printf("</br>%s</br>",new_url);
				execl("downloadHTTPS.out","downloadHTTPS.out",new_url,"/",(char *)NULL);
				printf("Excevp failed :(\n");
			}
			bzero(fill_line,sizeof(fill_line));
			index=0;
		}
		/*if(ch=='<')
			extract=3;
		if(extract==3)
			count++;
		if(extract>=2)
		{
			continue;
		}
		if(ch==str[str_index])
		{
			str_index++;
			if(str_index==15)
				extract=1;
		}
		else
		{
			str_index=0;
		}
		while(extract==1)
		{
			bytes=read(sockfd,&ch,1);
			printf("%c",ch);
			if(ch=='\n')
			{
				extract=2;
				break;
			}
			if(ch<48||ch>57)
				continue;
			build=build*10+(int)ch-48;
			//printf("\nCha2: %d",(int)ch);
		}*/
	}
	/*if(count==build)
		printf("\nSuccess: Complete response consumed, %d bytes\n",build);
	else
	{
		printf("Failed to get complete response. Got %d bytes while content was %d bytes\n",count,build);
	}*/
	//printf("Reaching here\n");
	SSL_shutdown(conn);
	close(sockfd);	
    return 0;
}