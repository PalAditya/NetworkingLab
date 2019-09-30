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
    int portno = 80,opt=1;
    char *host = argv[1];
    char *base="GET %s HTTP/1.1\r\n\r\n";
    struct hostent *server;
    struct sockaddr_in serv_addr;
    int sockfd, bytes, sent, total;
    char message[256];
    if (argc < 3) { puts("Parameters: Hostname Subindex"); exit(0); }
    /* fill in the parameters */
    sprintf(message,base,argv[2]);
    /* create the socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR opening socket");
    /* lookup the ip address */
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
    server = gethostbyname(host);
    if (server == NULL) error("ERROR, no such host");
    /* fill in the structure */
    memset(&serv_addr,0,sizeof(serv_addr));
    /* send the request */
	if(fork()==0)
	{
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(portno);
		memcpy(&serv_addr.sin_addr.s_addr,server->h_addr,server->h_length);
		if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting HTTP");
		total = strlen(message);
		sent = 0;
		do {
			bytes = write(sockfd,message+sent,total-sent);
			if (bytes < 0)
				error("ERROR writing message to socket");
			if (bytes == 0)
				break;
			sent+=bytes;
		} while (sent < total);
		char ch=' ';
		int count=0,print=0,str_index=0,extract=0,build=0;
		char *str="Content-Length:";
		char *detect="href=";
		int detect_index=0;
		char links[256];
		while((bytes=read(sockfd,&ch,1))==1)
		{
			if(ch=='<')
				print=1;
			if(print)
			printf("%c",ch);
			if(ch=='<')
				extract=3;
			if(extract==3)
				count++;
			if(ch==detect[detect_index])
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
							if(links[detect_index-1]=='s'&&links[detect_index-2]=='s')
							{
								bzero(message,sizeof(message));
								sprintf(message,"GET /%s HTTP/1.0\r\n\r\n",links);
								total = strlen(message);
								sent = 0;
								do {
									bytes = write(sockfd,message+sent,total-sent);
									if (bytes < 0)
										error("ERROR writing message to socket");
									if (bytes == 0)
										break;
									sent+=bytes;
								} while (sent < total);
								printf("<style></br>");
								while((bytes=read(sockfd,&ch,1))==1)
									printf("%c",ch);
								printf("</style></br>");
							}
							detect_index=0;
							break;
						}
						else
						links[detect_index++]=ch;						
					}
				}
			}
			else
			{
				detect_index=0;
			}
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
			}
		}
		close(sockfd);
	}
	else
	{
		wait(NULL);
		exit(0);
		printf("HTTPS...\n");
		serv_addr.sin_family = AF_INET;
		portno=443;
		serv_addr.sin_port = htons(portno);
		memcpy(&serv_addr.sin_addr.s_addr,server->h_addr,server->h_length);
		/* connect the socket */
		if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting HTTPS");
		SSL_load_error_strings ();
		SSL_library_init ();
		SSL_CTX *ssl_ctx = SSL_CTX_new (SSLv23_client_method ());

		// create an SSL connection and attach it to the socket
		SSL *conn = SSL_new(ssl_ctx);
		SSL_set_fd(conn, sockfd);

		// perform the SSL/TLS handshake with the server - when on the
		// server side, this would use SSL_accept()
		int err = SSL_connect(conn);
		if (err != 1)
		   abort();
		
		total = strlen(message);
		sent = 0;
		do {
			bytes = SSL_write(conn,message+sent,total-sent);
			if (bytes < 0)
				error("ERROR writing message to socket");
			if (bytes == 0)
				break;
			sent+=bytes;
		} while (sent < total);

		char ch=' ';
		int count=0,str_index=0,extract=0,build=0;
		char *str="Content-Length:";
		while((bytes=SSL_read(conn,&ch,1))==1)
		{
			printf("%c",ch);
			if(ch=='<')
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
			}
		}
		/* close the socket */
		SSL_shutdown(conn);
		close(sockfd);
		
	}  
    return 0;
}
