#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>

#define BUFFER_SIZE 1024

int main()
{
	int clientSocket;
	struct sockaddr_in serverAddress;
	char buffer[BUFFER_SIZE];

	clientSocket = socket(AF_INET,SOCK_STREAM,0); // tcp naala sock_stream

	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port=htons(8888);
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

	connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

	while(1)
	{
	
		// ithuku ullara eppovume client socket 	
	
		printf("Enter the message that has to be sent");
		fgets(buffer, BUFFER_SIZE, stdin);
		ssize_t byteSent = send(clientSocket, buffer, strlen(buffer),0);

		memset(buffer, 0, BUFFER_SIZE);
		ssize_t byteRecv = recv(clientSocket , buffer, BUFFER_SIZE - 1, 0);

		if(byteRecv <= 0)
		{
			break;
	
		}
	
		printf("Received from server %s \n", buffer);
    }
		close(clientSocket);

		return 0 ;
} 