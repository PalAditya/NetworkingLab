#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<ncurses.h>
#include<term.h>
#include<fcntl.h>
#include<dirent.h>
#include<signal.h>
#include <pwd.h>
#include <grp.h>
#define clearScreen() printf("\033[H\033[J")
#define die(e) do { fprintf(stderr, "%s\n", e); exit(EXIT_FAILURE); } while (0);
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#define PORT 8080 
char **buf;
int avail=0;
int implementLS(int tokens)
{
	int i=0;
	buf = malloc(20 * sizeof(char*));
	for(i=0;i<20;i++)
		buf[i]=malloc(1000*sizeof(char));
    char *curr_dir = NULL;
    DIR *dp = NULL;
    struct dirent *dptr = NULL;
    unsigned int count = 0;
    curr_dir = getenv("PWD");
    if(curr_dir == NULL)
    {
        perror("Could not get the working directory\n");
        return -1;
    }
    dp = opendir((const char*)curr_dir);
    if(dp == NULL)
    {
        perror("Could not open the working directory\n");
        return -1;
    }
    i=0;
    for(count = 0; (dptr = readdir(dp)) != NULL ; count++)
    {
		strcpy(buf[i],dptr->d_name);
		i++;
	}
	avail=i;
	return 0;
}
long int findSize(char file_name[]) 
{ 
    FILE* fp = fopen(file_name, "r");  
    if (fp == NULL) { 
        printf("File Not Found!\n"); 
        return -1; 
    } 
    fseek(fp, 0L, SEEK_END); 
    long int res = ftell(fp); 
    fclose(fp);
	printf("Found filsize: %ld\n",res);
	return res;
}
int readFile()
{
    int server_fd, new_socket; 
	struct sockaddr_in address; 
	int opt = 1; 
	int addrlen = sizeof(address);  
	char buffer[20];
	char buffer2[512]="";
	// Creating socket file descriptor 
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
	{ 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	// Forcefully attaching socket to the port 8080 
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
												&opt, sizeof(opt))) 
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( PORT ); 
	
	// Forcefully attaching socket to the port 8080 
	if (bind(server_fd, (struct sockaddr *)&address, 
								sizeof(address))<0) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	}
	if (listen(server_fd, 3) < 0) 
	{ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 
	while(1)
	{
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) 
		{ 
			perror("accept"); 
			exit(EXIT_FAILURE); 
		} 
		if(fork()==0)
		{
			close(server_fd);
			strcat(buffer2,buf[0]);
			strcat(buffer2,"\n");
			strcat(buffer2,buf[1]);
			strcat(buffer2,"\n");
			strcat(buffer2,buf[2]);
			strcat(buffer2,"\n");
			strcat(buffer2,buf[3]);
			strcat(buffer2,"\n");
			strcat(buffer2,buf[4]);
			strcat(buffer2,"\n");
			strcat(buffer2,buf[5]);
			strcat(buffer2,"\n");
			strcat(buffer2,buf[6]);
			strcat(buffer2,"\n");
			strcat(buffer2,buf[7]);
			strcat(buffer2,"\n");
			strcat(buffer2,buf[8]);
			strcat(buffer2,"\n");
			strcat(buffer2,buf[9]);
			/*strcat(buffer2,"\n");*/
			send(new_socket , buffer2 , strlen(buffer2) , 0 );
			read( new_socket , buffer, 20);
			printf("Received fileindex: %s\n",buffer);
			long int r=findSize(buf[atoi(buffer)]);
			char *saved=buf[atoi(buffer)];
			memset(&buffer, '0', sizeof(buffer));
			snprintf(buffer,20,"%ld",r);
			send(new_socket,buffer, strlen(buffer),0);
			printf("Sending filesize: %s\n",buffer);
			FILE *fp=fopen(saved,"r");
			if(fp==NULL)
				printf("Couldn't open file\n");
			char ch;
			printf("Sending data...\n");
			while(r-->0)
			{
				ch=fgetc(fp);
				//printf("%c",ch);
				send(new_socket,&ch,1,0);
			}
			fclose(fp);
			close(new_socket);
		}
	    close(new_socket);
	}	
    return 0;
}

int main()
{
	implementLS(1);
	readFile();
	return 0;
}
