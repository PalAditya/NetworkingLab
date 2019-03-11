#include<stdio.h>
#include<stdlib.h>
#include <unistd.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include<string.h>
int insert_error=0;
void getCRC(char ch,char crc[],int len,int sockfd)
{
	int *buf=(int*)calloc((8+len-1),sizeof(int));
	int i=0,n=(int)ch,j,a,i_pos,temp=(int)ch;
	//printf("Inside getCRC\n");
	while (n > 0) 
	{ 
        n = n / 2; 
        i++; 
    }
	//printf("Character length is: %d\n",i);
	i=8-i;	
	n=temp;
	temp=i;
	i=7;
	while (n > 0) 
	{ 
		buf[i]=n%2;
        n = n / 2; 
        i--; 
    }
	int *buff=(int*)calloc((8+len-1),sizeof(int));
	for(i=0;i<7+len;i++)
		buff[i]=buf[i];
	/*temp=1;
	buf[1]=1;
	buf[2]=0;
	buf[3]=1;
	buf[4]=0;
	buf[5]=0;
	buf[6]=0;
	buf[7]=0;*/
	/*for(i=temp;i<8;i++)
		printf("%d",buff[i]);
	printf("\n");*/
	for(i=temp;i<8;)
	{
		a=0;
		i_pos=-1;
		for(j=i;j<len+i;j++)
		{
			//printf("CRC: %d\n",(int)crc[a]);
			buf[j]=buf[j]^(int)(crc[a++]-48);
			if(buf[j]==1&&i_pos==-1)
				i_pos=j;
		}
		i=i_pos;
		if(i_pos<0)
			break;
		//printf("I pos is:%d\n",i_pos);
	}
	/*printf("Remainder:\n");
	for(i=0;i<8+len-1;i++)
		printf("%d",buf[i]);
	printf("\n");*/
	for(i=0;i<8+len-1;i++)
		buf[i]=buf[i]+buff[i];
	for(i=0;i<8+len-1;i++)
	{
		/*if(insert_error==2&&i==3)
		{
			if(buf[i]==0)
				buf[i]=1;
			else
				buf[i]=0;
		}*/
		write(sockfd,&buf[i],sizeof(int));
	}
}
int main()
{
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
	int len;
	printf("Enter length of crc polynomial\n");
	scanf("%d",&len);
	char intbuf[4];
	sprintf(intbuf,"%d",len);
	char *crc=(char *)calloc(len,sizeof(char));
	printf("Enter the crc polynomial\n");
	scanf("%s",crc);
	write(new_socket,intbuf,sizeof(intbuf));
	write(new_socket,crc,4);
	char *message="Aditya\n";
	int s=7;
	bzero(intbuf,sizeof(intbuf));
	sprintf(intbuf,"%d",s);
	write(new_socket,intbuf,sizeof(int));
	for(i=0;i<strlen(message);i++)
	{
		insert_error++;
		getCRC(message[i],crc,len,new_socket);
	}
	return 0;
}
