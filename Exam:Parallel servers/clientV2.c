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
int *arr1,*arr2,*arr3,*arr4;
int sockfd1,sockfd2,sockfd3,sockfd4; 
int size1,size2,size3,size4;
int getLines()
{
	FILE *fp = fopen("random.txt", "r"); 
    if (fp == NULL) 
    { 
        printf("Could not open file\n"); 
        return 0; 
    } 
	int count;
	char c;
    for (c = getc(fp); c != EOF; c = getc(fp)) 
        if (c == '\n') // Increment count if this character is newline 
            count = count + 1; 
  
    // Close the file 
    fclose(fp);
	return count;
}
int connectAndReturn(int port)//Accepts a port number and returns connection to a server running on localhost
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
void sendAndSort(int arr[],int sockfd,int size)//Called by a thread,simply writes a array to server and reads the sorted array back
{
	int i;
	write(sockfd,&size,sizeof(int));
	for(i=0;i<size;i++)
		write(sockfd,&arr[i],sizeof(int));
	for(i=0;i<size;i++)
		read(sockfd,&arr[i],sizeof(int));
	//printf("%d,%d,%d,%d,%d\n",arr[0],arr[1],arr[2],arr[3],arr[4]);
}
void *fun1(void *vargp)
{
	sendAndSort(arr1,sockfd1,size1);
    pthread_exit(NULL);
}
void *fun2(void *vargp)
{
	sendAndSort(arr2,sockfd2,size2);
    pthread_exit(NULL);
}
void *fun3(void *vargp)
{
	sendAndSort(arr3,sockfd3,size3);
    pthread_exit(NULL);
}
void *fun4(void *vargp)
{
	sendAndSort(arr4,sockfd4,size4);
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
    int size=0;
  	sockfd1=connectAndReturn(8080);
	sockfd2=connectAndReturn(8081);
	sockfd3=connectAndReturn(8082);
	sockfd4=connectAndReturn(8083);
	/*printf("Enter file size\n"); //Not necessary after getLines()
	scanf("%d",&size);*/
	size=getLines();
	arr1=malloc((size/4)*sizeof(int));
	arr2=malloc((size/4)*sizeof(int));
	arr3=malloc((size/4)*sizeof(int));
	arr4=malloc((size-(size/4)*3)*sizeof(int));
	FILE *fp=fopen("random.txt","r");
	int i,num;
	for(i=0;i<size;i++)
	{
		fscanf(fp,"%d",&num);
		if(i<size/4)
			arr1[i]=num;
		else if(i<2*(size/4))
			arr2[i-(size/4)]=num;
		else if(i<3*(size/4))
			arr3[i-2*(size/4)]=num;
		else
			arr4[i-3*(size/4)]=num;
	}
	size1=size2=size3=size/4;
	size4=size-3*(size/4);
	printf("%d,%d,%d,%d\n",size1,size2,size3,size4);
	//Creating a struct would eliminate the need of global variables in thread. Couldn't implement due to lack of time.
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
	int t1[size/4+size/4];
	int t2[size/4+size-3*(size/4)];
	int t3[size];
	merge2(t1,arr1,arr2,size1,size2);
	merge2(t2,arr3,arr4,size3,size4);
	merge2(t3,t1,t2,size/4+size/4,size/4+size-3*(size/4));
	for(i=0;i<size;i++)
		printf("%d\n",t3[i]);
	return 0;
} 
