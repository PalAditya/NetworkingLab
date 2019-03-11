#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include<arpa/inet.h>
#include<unistd.h>
#define SA struct sockaddr
int isSet(int num,int bit)
{
	return 1 == ( (num >> bit) & 1);
}
int getCount(int pos,int size,int arr[])
{
	int i,count=0;
	for(i=0;i<size;i++)
	{
		if(isSet(size-i,pos)&&arr[i]==1)
			count++;
	}
	//printf("Found that with a=%d, we got %d\n",pos,count);
	return count;
}
void check(int arr[],int len,int test)
{
	int i,a=0,num=0;
	int build[test];
	for(i=0;i<test;i++)
	{
		if(getCount(a++,len,arr)%2==0)
			build[test-i-1]=0;
		else
			build[test-i-1]=1;
		//printf("%d",build[test-i-1]);
	}	
	//printf("\n");
	for(i=0;i<test;i++)
		num=num*2+build[i];
	if(num!=0)
		printf("Error at %d\n",len-num);
	else
		printf("No error\n");
}
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
	int len,test,i;
	char intbuf[4];
	read(sockfd1,intbuf,sizeof(intbuf));
	test=atoi(intbuf);
	printf("%d\n",test);
	bzero(intbuf,sizeof(intbuf));
	read(sockfd1,intbuf,sizeof(intbuf));
	len=atoi(intbuf);
	printf("%d\n",len);
	int arr[len];
	for(i=0;i<len;i++)
	{
		bzero(intbuf,sizeof(intbuf));
		read(sockfd1,intbuf,sizeof(intbuf));
		arr[i]=atoi(intbuf);
		printf("%d",arr[i]);
	}
	printf("\n");
	check(arr,len,test);
	return 0;
}