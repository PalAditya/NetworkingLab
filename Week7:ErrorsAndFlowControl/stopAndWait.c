#include<stdio.h>
#include<stdlib.h>
#include <unistd.h> 
#include <sys/socket.h> 
#include <netinet/in.h>
#include<fcntl.h>
#include<time.h>
#include<string.h>
#include<sys/time.h>
char getRandomCharacter()
{
	return (char)(rand()%26+65);
}
char getFileChar(FILE *fp)
{
	char ch;
	fscanf(fp,"%c",&ch);
	return ch;
}
int main(int argc,char* argv[])
{
	srand(time(0));
	int port,i;
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
	/*struct timeval tv;
	tv.tv_usec=100;
	tv.tv_sec=0;
	if (setsockopt(server_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv))) 
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	}*/
	printf("Enter portno\n");
	scanf("%d",&port);
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons(port); 
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
	int flags = fcntl(new_socket, F_GETFL, 0);
	fcntl(new_socket, F_SETFL, flags | O_NONBLOCK);
	char ch;
	int frameno=0;
	char intbuf[4];
	FILE *fp=fopen("test.txt","r");
	for(i=0;i<12;i++)
	{
		ch=getFileChar(fp);
		printf("Random: %c\n",ch);
		bzero(intbuf,sizeof(intbuf));
		sprintf(intbuf,"%d",frameno);
		write(new_socket,intbuf,sizeof(intbuf));
		write(new_socket,&ch,sizeof(char));
		while(1)
		{
			bzero(intbuf,sizeof(intbuf));
			sleep(1);
			read(new_socket,intbuf,sizeof(intbuf));
			printf("Frameno:%d,%d\n",atoi(intbuf),frameno+1);
			if(atoi(intbuf)>frameno)
				break;
			/*bzero(intbuf,sizeof(intbuf));
			sprintf(intbuf,"%d",frameno);
			write(new_socket,intbuf,sizeof(intbuf));*/
			write(new_socket,&ch,sizeof(char));//Write the character again
		}
		frameno=atoi(intbuf)+1;
	}
	fclose(fp);
	return 0;
}