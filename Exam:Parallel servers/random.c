#include<stdio.h>
#include<time.h>
#include<stdlib.h>
int main()
{
	srand(time(NULL));
	int i,x,val;
	printf("Enter the number of random numbers to generate\n");
	scanf("%d",&val);
	FILE *fp=fopen("random.txt","w");
	for(i=0;i<val;i++)
	{
		x=rand()%10000+1;
		fprintf(fp,"%d\n",x);
	}
	return 0;
}
