#include<iostream>
#include<stdio.h>		//fputs
#include<sys/types.h>	//socket, connect
#include<sys/socket.h>	//socket, connect
#include<stdlib.h>	//exit
#include<string.h>	//explicit_bzero
#include<arpa/inet.h>	//inet_pton
#include<unistd.h>	//read
#include<time.h>		//time
#include<pthread.h>
#define MAXCLIENTQ 10	//backlog
#define MAXLEN 4
#include <unordered_map> 
int servport;
#include <limits.h> 
int graph[4][4];
#define arr graph
#define V 4
//unsigned long timee=clock();
using namespace std;
typedef struct neighbour
{
	int port;
	int dist;
}neighbour;
int count=0;
int minDistance(int dist[], bool sptSet[]) 
{ 
   // Initialize min value 
   int min = INT_MAX, min_index; 
   
   for (int v = 0; v < V; v++) 
     if (sptSet[v] == false && dist[v] <= min) 
         min = dist[v], min_index = v; 
   
   return min_index; 
} 
   
// A utility function to print the constructed distance array 
int printSolution(int dist[], int n) 
{ 
   printf("Right now, best estimate is:\nVertex   Distance from Source\n"); 
   for (int i = 0; i < V; i++) 
   {
	   if (dist[i]>100)
		   printf("%d ....Infinite\n",(i+8000));
	   else
      	   printf("%d ... %d\n", (i+8000), dist[i]); 
   }
} 
   
// Function that implements Dijkstra's single source shortest path algorithm 
// for a graph represented using adjacency matrix representation 
void dijkstra(int graph[V][V], int src) 
{ 
	 //vector<string> updates;
     int dist[V];     // The output array.  dist[i] will hold the shortest 
                      // distance from src to i 
   
     bool sptSet[V]; // sptSet[i] will be true if vertex i is included in shortest 
                     // path tree or shortest distance from src to i is finalized 
   
     // Initialize all distances as INFINITE and stpSet[] as false 
     for (int i = 0; i < V; i++) 
        dist[i] = INT_MAX, sptSet[i] = false; 
   
     // Distance of source vertex from itself is always 0 
     dist[src] = 0; 
   
     // Find shortest path for all vertices 
     for (int count = 0; count < V-1; count++) 
     { 
       // Pick the minimum distance vertex from the set of vertices not 
       // yet processed. u is always equal to src in the first iteration. 
       int u = minDistance(dist, sptSet); 
   
       // Mark the picked vertex as processed 
       sptSet[u] = true; 
   
       // Update dist value of the adjacent vertices of the picked vertex. 
       for (int v = 0; v < V; v++) 
   
         // Update dist[v] only if is not in sptSet, there is an edge from  
         // u to v, and total weight of path from src to  v through u is  
         // smaller than current value of dist[v] 
         if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX  && dist[u]+graph[u][v] < dist[v])
		 {
            dist[v] = dist[u] + graph[u][v];
			//printf("Update: Best way to reach router %d is through router %d\n",u,v);
			//updates.push_back("Update: Best way to reach router %d");
		 }
     } 
   
     // print the constructed distance array 
     printSolution(dist, V); 
} 

void handle(int graph[4][4])
{
	char x[4];
	int l=servport;
	sprintf(x,"%d",servport);
	FILE *fp=fopen(x,"r");
	char line[8];
	int i,j;
	bzero(line,sizeof(line));
	while (fgets ( line, sizeof(line), fp ) != NULL )
	{
		char *token=strtok(line,",");
		char key[5];
		char dist[2];
		dist[0]=line[5];
		dist[1]='\0';
		for(i=0;i<4;i++)
			key[i]=line[i];
		//printf("%d\n",atoi(key));
		int val=atoi(dist);
		//printf("%d\n",val);
		printf("Values to insert in graph: %d,%d,%d\n",l,atoi(key),val);
		graph[l-8000][atoi(key)-8000]=val;
		bzero(line,sizeof(line));
	}
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			if (i==j)
			{
				arr[i][j]=0;
				continue;
			}
			if(arr[i][j]==arr[j][i])
				continue;
			if(arr[i][j]!=-9999)
				arr[j][i]=arr[i][j];
			else if(arr[j][i]!=-9999)
				arr[i][j]=arr[j][i];
		}
	}
	/*printf("Djikstra time! Graph I got is!\n");
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			printf("%d ",graph[i][j]);
		}
		printf("\n");
	}*/
	count++;
	//if (count==3)
	dijkstra(graph,l-8000);
}
void updater(int p)
{
	sleep(10);
	char x[4];
	int l=p;
	printf("Data of router %d...\n",p);
	sprintf(x,"%d",p);
	FILE *fp=fopen(x,"r");
	char line[8];
	bzero(line,sizeof(line));
	while (fgets ( line, sizeof(line), fp ) != NULL )
	{
		char *token=strtok(line,",");
		char key[5];
		char dist[2];
		dist[0]=line[5];
		dist[1]='\0';
		bzero(key,sizeof(key));
		int i,j;
		for(i=0;i<4;i++)
			key[i]=line[i];
		//printf("%d\n",atoi(key));
		int val=atoi(dist);
		//printf("%d\n",val);
		bzero(line,sizeof(line));
		printf("Values to insert in graph: %d,%d,%d\n",l,atoi(key),val);
		graph[l-8000][atoi(key)-8000]=val;
		//unsigned long r=clock()-timee;
		//printf("Elapsed time is %lu\n",r);
		//if (r/CLOCKS_PER_SEC>=30&&servport==8002)
		//{
			/*printf("Djikstra time! Graph I got is!\n");
			for(i=0;i<4;i++)
			{
				for(j=0;j<4;j++)
				{
					printf("%d ",graph[i][j]);
				}
				printf("\n");
			}*/
		//}
	}
	handle(graph);
	fclose(fp);
}
void *server(void *v)
{
	char **x=(char **)v;
	int listenfd, connfd;
	struct sockaddr_in servaddr;
	/*char fname[10];
	sprintf(fname,"%s.txt",argv[1]);*/
	//unordered_map<int, int> umap; 
	char buff[MAXLEN];
	if((listenfd=socket(AF_INET, SOCK_STREAM, 0))==-1)
	{
		perror("socket");
		exit(1);
	}
	explicit_bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servaddr.sin_port=htons(atoi(x[1]));
	if(bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr))==-1)
	{
		perror("bind");
		exit(2);
	}
	if(listen(listenfd, MAXCLIENTQ)==-1)
	{
		perror("listen");
		exit(3);
	}
	while(1)
	{
		if((connfd=accept(listenfd, (struct sockaddr *)NULL, NULL))==-1)
		{
			exit(4);
		}
		if(fork()==0)
		{
			close(listenfd);
			bzero(buff,sizeof(buff));
			read(connfd,buff,sizeof(buff));
			int p,d;
			p=atoi(buff);
			bzero(buff,sizeof(buff));
			read(connfd,buff,sizeof(buff));
			d=atoi(buff);
			printf("I now know my distance from node at %d is %d units\n",p,d);
			//graph[servport-8000][p-8000]=d;
			FILE *fp=fopen(x[1],"a+");
			//printf("Fname is %s\n",fname);
			fprintf(fp,"%d,%d\n",p,d);
			fclose(fp);
			//updater(connfd,x[1]);
		}
		close(connfd);
	}
}
void *client(void *v)
{
	sleep(10);
	neighbour *x=(neighbour *)v;
	int sock = 0, PORT = x->port, DIST=x->dist; 
	struct sockaddr_in serv_addr; 
	//char buff[1024];
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		printf("\n Socket creation error \n"); 
		exit(3); 
	} 
	memset(&serv_addr, '0', sizeof(serv_addr)); 
	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
	{ 
		printf("\nInvalid address/ Address not supported \n"); 
		exit(1); 
	} 
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{ 
		printf("\nConnection Failed \n"); 
		exit(2); 
	} 
	//bzero(buff,sizeof(buff));
	char intval[4];
	bzero(intval,sizeof(intval));
	sprintf(intval,"%d",servport);
	write(sock,intval,sizeof(intval));
	bzero(intval,sizeof(intval));
	sprintf(intval,"%d",DIST);
	write(sock,intval,sizeof(intval));
	//printf("I'm %d, and I know now that...\n",servport);
	updater(PORT);
}
int main(int argc, char **argv)
{
	int rc,i,j,s=atoi(argv[2]),k,total=atoi(argv[argc-1]);
	k=3;
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			graph[i][j]=9999;
		}
		//printf("\n");
	}
	servport=atoi(argv[1]);
	pthread_t tclient[s],tserver,tupdater;
	for(i=0;i<s;i++)
	{
		neighbour *n1=(neighbour *)malloc(sizeof(neighbour));
		n1->port=atoi(argv[k]);
		n1->dist=atoi(argv[k+1]);
		rc=pthread_create(&tclient[i], NULL, client, n1);
		printf("Told client to connect at port %d\n",atoi(argv[k]));
		k=k+2;
		if(rc<0)
		   exit(0);
	}
   	rc=pthread_create(&tserver, NULL, server, (void *)argv);
    if(rc<0)
       exit(0);
	for(i=0;i<s;i++)
    	pthread_join(tclient[i],NULL);
    pthread_join(tserver,NULL);
	return 0;
}