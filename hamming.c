#include<stdio.h>
#include<stdlib.h>
#include <unistd.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include<string.h>
#include <math.h>
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
void getHamming(char buf[],int len,int sockfd)
{
	int test=0,a=0,i;
	while(pow(2,test)<len+test+1)
		test++;
	//printf("Test is:%d\n",test);
	int *arr=(int *)calloc((test+len),sizeof(int));
	for(i=0;i<test+len;i++)
	{
		if(ceil(log2(test+len-i)) == floor(log2(test+len-i)));
			//printf("Skipped: %d\n",(i+1));
		else
			arr[i]=buf[a++]-48;
	}
	a=0;
	for(i=0;i<test;i++)
	{
		if(getCount(a++,test+len,arr)%2==0)
			arr[test+len-(int)pow(2,i)]=0;
		else
			arr[test+len-(int)pow(2,i)]=1;
	}
	/*for(i=0;i<test+len;i++)
		printf("%d",arr[i]);
	printf("\n");*/
	char intbuf[4];
	bzero(intbuf,sizeof(intbuf));
	sprintf(intbuf,"%d",test);
	write(sockfd,intbuf,sizeof(intbuf));
	bzero(intbuf,sizeof(intbuf));
	sprintf(intbuf,"%d",test+len);
	write(sockfd,intbuf,sizeof(intbuf));
	int err_bit=200;
	for(i=0;i<test+len;i++)
	{
		bzero(intbuf,sizeof(intbuf));
		if(err_bit==i)
		{
			if(arr[i]==1)
				arr[i]=0;
			else
				arr[i]=1;
		}
		sprintf(intbuf,"%d",arr[i]);
		write(sockfd,intbuf,sizeof(intbuf));
	}
}
int main()
{
	int port;
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
	getHamming("000010001",9,new_socket);
	return 0;
}