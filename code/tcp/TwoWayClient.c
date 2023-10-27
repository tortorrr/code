#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main()
{
    int clientSocket;
    struct sockaddr_in serverAddress;
    char buffer[BUFFER_SIZE];

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8888);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    
    // it is just like bind
    connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

    while(1)
    {
        // Send message to server
        printf("Enter message: ");
        fgets(buffer, BUFFER_SIZE, stdin); 
        ssize_t bytesSent = send(clientSocket, buffer, strlen(buffer), 0);

        // Receive response from server
        memset(buffer, 0, BUFFER_SIZE);
        ssize_t bytesRead = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
        if (bytesRead <= 0)
        {
            // Error or server closed the connection
            break;
        }
        printf("Received from server: %s\n", buffer);
    }

    // Close client socket
    close(clientSocket);

    return 0;
}
