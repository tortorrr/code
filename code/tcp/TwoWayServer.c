#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main()
{
    int serverSocket, clientSocket; // socket descriptor
    struct sockaddr_in serverAddress, clientAddress;
    char buffer[BUFFER_SIZE];

    // Create server socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    // Set up server address
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8888);       
    serverAddress.sin_addr.s_addr = INADDR_ANY; 

    // Bind server socket to the specified address and port
    bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

    // Listen for incoming client connections
    listen(serverSocket, 1);

    printf("Server started. Waiting for incoming connection...\n");

    socklen_t clientAddressLength = sizeof(clientAddress);

    // Accept client connection
    clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);

    printf("Client connected. IP: %s, Port: %d\n",
           inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port));

    while(1)
    {
        // Receive message from client
                // pillar of buffer
        memset(buffer, 0, BUFFER_SIZE);
        ssize_t bytesRead = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
        // sszie_t is used to represent the size of the integer or the array
        if (bytesRead <= 0)
        {
            // Error or client closed the connection
            break;
        }
        printf("Received from client: %s\n", buffer);

        // Send response to client
        printf("Enter response: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        ssize_t bytesSent = send(clientSocket, buffer, strlen(buffer), 0);
    }

    // Close sockets
    close(clientSocket);
    close(serverSocket);

    return 0;
}
