#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h>
#define SIZE 250
#define SA struct sockaddr
int arr1[SIZE],arr2[SIZE],arr3[SIZE],arr4[SIZE];
int sockfd1,sockfd2,sockfd3,sockfd4; 
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
	char buffer[100];
	bzero(buffer,sizeof(buffer));
	read(sockfd,buffer,100);
	printf("%s\n",buffer);
	return sockfd;
}
void sendAndSort(int arr[],int sockfd)
{
	int i;
	for(i=0;i<SIZE;i++)
		write(sockfd,&arr[i],sizeof(int));
	for(i=0;i<SIZE;i++)
		read(sockfd,&arr[i],sizeof(int));
	//printf("%d,%d,%d,%d,%d\n",arr[0],arr[1],arr[2],arr[3],arr[4]);
}
void *fun1(void *vargp)
{
	sendAndSort(arr1,sockfd1);
    pthread_exit(NULL);
}
void *fun2(void *vargp)
{
	sendAndSort(arr2,sockfd2);
    pthread_exit(NULL);
}
void *fun3(void *vargp)
{
	sendAndSort(arr3,sockfd3);
    pthread_exit(NULL);
}
void *fun4(void *vargp)
{
	sendAndSort(arr4,sockfd4);
    pthread_exit(NULL);
}
void merge(int arr[], int L[], int R[], int x) 
{ 
    int i, j, k; 
    int n1 = x; 
    int n2 =  x;  
    i = 0; // Initial index of first subarray 
    j = 0; // Initial index of second subarray 
    k = 0; // Initial index of merged subarray 
    while (i < n1 && j < n2) 
    { 
        if (L[i] <= R[j]) 
        { 
            arr[k] = L[i]; 
            i++; 
        } 
        else
        { 
            arr[k] = R[j]; 
            j++; 
        } 
        k++; 
    } 
    while (i < n1) 
    { 
        arr[k] = L[i]; 
        i++; 
        k++; 
    } 
    while (j < n2) 
    { 
        arr[k] = R[j]; 
        j++; 
        k++; 
    } 
} 
void merge2(int arr[], int L[], int R[], int x,int y)//Use if size not equal
{ 
    int i, j, k; 
    int n1 = x; 
    int n2 = y;  
    i = 0; // Initial index of first subarray 
    j = 0; // Initial index of second subarray 
    k = 0; // Initial index of merged subarray 
    while (i < n1 && j < n2) 
    { 
        if (L[i] <= R[j]) 
        { 
            arr[k] = L[i]; 
            i++; 
        } 
        else
        { 
            arr[k] = R[j]; 
            j++; 
        } 
        k++; 
    } 
    while (i < n1) 
    { 
        arr[k] = L[i]; 
        i++; 
        k++; 
    } 
    while (j < n2) 
    { 
        arr[k] = R[j]; 
        j++; 
        k++; 
    } 
} 
/*void fillStruct(struct desc desc_o,int a,int arr[])
{
	int i;
	for(i=0;i<250;i++)
		desc_o._arr[i]=arr[i];
	desc_o._sockfd=a;
}*/
int main() 
{ 
    
  	sockfd1=connectAndReturn(8080);
	sockfd2=connectAndReturn(8081);
	sockfd3=connectAndReturn(8082);
	sockfd4=connectAndReturn(8083);
	FILE *fp=fopen("random.txt","r");
	int i,num;
	for(i=0;i<1000;i++)
	{
		fscanf(fp,"%d",&num);
		if(i<250)
			arr1[i]=num;
		else if(i<500)
			arr2[i-250]=num;
		else if(i<750)
			arr3[i-500]=num;
		else
			arr4[i-750]=num;
	}
	/*sendAndSort(arr1,sockfd1);
	sendAndSort(arr2,sockfd2);
	sendAndSort(arr3,sockfd3);
	sendAndSort(arr4,sockfd4);
    close(sockfd1);
	close(sockfd2);
	close(sockfd3);
	close(sockfd4);
	int t1[SIZE*2];
	int t2[SIZE*2];
	int t3[SIZE*4];
	merge(t1,arr1,arr2,SIZE);
	merge(t2,arr3,arr4,SIZE);
	merge(t3,t1,t2,SIZE*2);
	for(i=0;i<1000;i++)
		printf("%d\n",t3[i]);
	struct desc 
	{ 
	   int _sockfd, _arr[250]; 
	};
	struct desc desc_o;
	desc_o=malloc(sizeof(desc));
	fillStruct(desc_o,sockfd1,arr1);*/
	int rc;
	pthread_t one,two,three,four;
    rc=pthread_create(&one, NULL, fun1,NULL);
    if(rc<0)
   	   perror("Thread");
	pthread_join(one,NULL);
   	rc=pthread_create(&two, NULL, fun2, NULL);
    if(rc<0)
      exit(0);	
	rc=pthread_create(&three, NULL, fun3, NULL);
    if(rc<0)
      exit(0);	
	rc=pthread_create(&four, NULL, fun4, NULL);
    if(rc<0)
      exit(0);	
    pthread_join(one,NULL);
    pthread_join(two,NULL);
	pthread_join(three,NULL);
	pthread_join(four,NULL);
	int t1[SIZE*2];
	int t2[SIZE*2];
	int t3[SIZE*4];
	merge(t1,arr1,arr2,SIZE);
	merge(t2,arr3,arr4,SIZE);
	merge(t3,t1,t2,SIZE*2);
	for(i=0;i<1000;i++)
		printf("%d\n",t3[i]);
	return 0;
} 
