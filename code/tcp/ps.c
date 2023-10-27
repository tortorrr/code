

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define BUFFER_SIZE 1024

int main()
{
	int serverSocket, clientSocket;
	struct sockaddr_in serverAddress, clientAddress;
	char buffer[BUFFER_SIZE];

	serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(8888);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	bind(serverSocket , (struct sockaddr *) &serverAddress, sizeof(serverAddress));

			// ithumela no serverAddress

	listen(serverSocket , 1);

	

	socklen_t clientAddressLength = sizeof(clientAddress);

	

	clientSocket = accept(serverSocket , (struct sockaddr *)&clientAddress , &clientAddressLength);
	 

	printf("Server started listening at address %s , and port %d " , inet_ntoa(clientAddress.sin_addr),ntohs(clientAddress.sin_port));


	while(1)
	{
		
		memset(buffer, 0, BUFFER_SIZE);
		ssize_t bytesRec = recv(clientSocket, buffer , BUFFER_SIZE - 1, 0);
		if(bytesRec <= 0)
		{
			break;
		}
		printf("message from the client %s \n" , buffer);

		printf("enter the response message: \n" );
		fgets(buffer, BUFFER_SIZE, stdin);
		ssize_t bytesSent = send(clientSocket, buffer,strlen(buffer), 0) ;
		
	}

	close(clientSocket);
	close(serverSocket);

	return 0;

}
	
		














































