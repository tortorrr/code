#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#define PORT 12346
#define SWS 3

typedef struct{
	int seq_no;
	char data[20];
	int ack;
}Frame;

int main()
{
	int sock,connfd;
	struct sockaddr_in sadd;
	sock=socket(AF_INET,SOCK_STREAM,0);
	if(sock<0)
	{
		perror("error insocket");
		exit(-1);
	}
	printf("Socket created successfully\n");
	
	sadd.sin_family=AF_INET;
	sadd.sin_port=htons(PORT);
	sadd.sin_addr.s_addr=inet_addr("127.0.0.1");
	
	socklen_t len;
	len=sizeof(sadd);
	if(connect(sock,(struct sockaddr *)&sadd,len)!=0)
	{
		printf("Connection with the server failed");
		exit(0);
	}
	
	
	
	Frame frames[SWS];
	int LFS=-1,LAR=-1,i=LAR+1,flag=1;
	
	for(;;)
	{
		for(;i<SWS;i++)
		{
			if(flag==1)
				frames[i].seq_no=++LFS;
			printf("Enter message for frame %d:",i);
			scanf("%s",frames[i].data);
			send(sock,&frames[i],sizeof(Frame),0);
		}
		i=(LAR+1)%SWS;
		for(;i<SWS;i++)
		{
			recv(sock,&frames[i].ack,sizeof(int),0);
		}
		i=(LAR+1)%SWS;
		for(;i<SWS;i++)
		{
			
			if(frames[i].ack==0)
			{	
							
				flag=0;
				break;
			}
			LAR+=1;
			flag=1;	
		}
		i=(LAR+1)%SWS;
		if(flag==0){
			printf("Retransmit frame!\n");
			continue;
		}
		LAR+=SWS;
		i=(LAR+1)%SWS;
		printf("\n");
		
		
	}
	close(sock);
	return 0;
}
