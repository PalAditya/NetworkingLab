#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include<arpa/inet.h>
#include<unistd.h>
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
void check(int arr[],char crc[],int len)
{
	int i,j,i_pos,a;
	for(i=1;i<8;)
	{
		a=0;
		i_pos=-1;
		for(j=i;j<len+i;j++)
		{
			//printf("CRC: %d\n",(int)crc[a]);
			arr[j]=arr[j]^(int)(crc[a++]-48);
			if(arr[j]==1&&i_pos==-1)
				i_pos=j;
		}
		i=i_pos;
		if(i_pos<0)
			break;
	}
	for(i=0;i<7+len;i++)
		if(arr[i]==1)
			break;
	if(i!=7+len)
		printf("Error at position %d\n",(i+1));
	else
		printf("Ok\n");
}
int main()
{
	int sockfd1=connectAndReturn(8080);
	int len,lo,val,i;
	char intbuf[4];
	read(sockfd1,intbuf,sizeof(intbuf));
	len=atoi(intbuf);
	printf("%d\n",len);
	//sleep(10);
	//len=4;
	char crc[len];
	bzero(crc,sizeof(crc));
	read(sockfd1,crc,sizeof(crc));
	printf("%c%c%c%c\n",crc[0],crc[1],crc[2],crc[3]);
	bzero(intbuf,sizeof(intbuf));
	read(sockfd1,intbuf,sizeof(intbuf));
	lo=atoi(intbuf);
	printf("Got size:%d\n",lo);
	while(lo-->0)
	{
		int arr[7+len];
		for(i=0;i<7+len;i++)
		{
			read(sockfd1,&val,sizeof(int));
			printf("%d",val);
			arr[i]=val;
		}
		printf("\n");
		check(arr,crc,len);
	}
	return 0;
}