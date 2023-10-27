#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#define PORT 12346
typedef struct{
	int seq_no;
	char data[20];
	int ack;
}Frame;
int main()
{
	int sock,connfd;
	struct sockaddr_in sadd,cadd;
	sock=socket(AF_INET,SOCK_STREAM,0);
	if(sock<0)
	{
		perror("error insocket");
		exit(-1);
 	}
	printf("Socket created successfully\n");
	sadd.sin_family=AF_INET;
	sadd.sin_addr.s_addr=htonl(INADDR_ANY);
	sadd.sin_port=htons(PORT);	
	if(bind(sock,(struct sockaddr*)&sadd,sizeof(sadd))<0)
	{
		perror("Error in binding");
		exit(-1);
	}
	if(listen(sock,1)!=0)
	{
		printf("Listen Failed");
		exit(-1);
	}
	socklen_t len;
	len=sizeof(cadd);
	connfd=accept(sock,(struct sockaddr *)&cadd,&len);
	if(connfd<0)
	{
		printf("Server accept failed");
		exit(0);
	}
	int bit=0;
	Frame frames[2];
	for(;;)
	{
		//read(connfd,buffer,sizeof(buffer));
		//read(connfd,&bit,sizeof(bit));
		recv(connfd,&frames[bit],sizeof(Frame),0);
		printf("From sender: %s frame: %d\n",frames[bit].data,bit);
		if(strncmp(frames[bit].data,"exit",4)==0)
		{	
			printf("Sender closed");
			break;
		}
		printf("Send Ack:1\nNAck:0\n");
		scanf("%d",&frames[bit].ack);
		send(connfd,&frames[bit].ack,sizeof(int),0);
		if(frames[bit].ack==1)
		{
			bit=bit^1;
			//write(connfd,&bit,sizeof(bit));
		}
		
		
	}
	close(sock);
	return 0;
}
