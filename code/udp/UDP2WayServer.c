// first code server side and the client side 


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

void error(const char *message) {
    perror(message);
    exit(1);
}

int main() {
    int serverSocket;
    struct sockaddr_in serverAddress, clientAddress;
    char buffer[BUFFER_SIZE];

    // Create server socket
    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket < 0) {
        error("Error creating server socket");
    }

    // Set up server address
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY; // defining the IP Address
    serverAddress.sin_port = htons(8888); // defining the port number

    // Bind server socket to the specified address and port
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        error("Error binding server socket");
    }

    printf("Server started. Waiting for incoming messages...\n");

    socklen_t clientAddressLength = sizeof(clientAddress);

    while (1) {
        // Receive message from client
        // inorder to receive we need a memory 
        memset(buffer, 0, BUFFER_SIZE);
        ssize_t bytesRead = recvfrom(serverSocket, buffer, BUFFER_SIZE - 1, 0, (struct sockaddr *)&clientAddress, &clientAddressLength);
        if (bytesRead <= 0) {
            // Error or client closed the connection
            break;
        }
        printf("Received from client %s:%d: %s\n", inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port), buffer);

        // Send response to client
        printf("Enter response: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        ssize_t bytesSent = sendto(serverSocket, buffer, strlen(buffer), 0, (struct sockaddr *)&clientAddress, clientAddressLength);
        if (bytesSent <= 0) {
            error("Error sending response");
        }
    }

    // Close socket
    close(serverSocket);

    return 0;
}


// it uses only the bind() 
// receive pannrappo we ned in the size of buffer_size -1 
// send pannrappo we nneed in the size od strlen(buffer) 6 parameters in UDP and 4 in TCP 

