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
    int clientSocket;
    struct sockaddr_in serverAddress;
    char buffer[BUFFER_SIZE];

    // Create client socket
    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket < 0) {
        error("Error creating client socket");
    }

    // Set up server address
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddress.sin_port = htons(8888);
    socklen_t serverAddressLength = sizeof(serverAddress);
    while (1)
    {
        printf("Enter message: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        // Send message to server
        ssize_t bytesSent = sendto(clientSocket, buffer, strlen(buffer), 0, (struct sockaddr *)&serverAddress, serverAddressLength);
        if (bytesSent <= 0) {
            error("Error sending message");
        }

        // Receive response from server
        memset(buffer, 0, BUFFER_SIZE);

        ssize_t bytesRead = recvfrom(clientSocket, buffer, BUFFER_SIZE - 1, 0, (struct sockaddr *)&serverAddress, &serverAddressLength);
        if (bytesRead <= 0) {
            error("Error receiving response");
        }
        printf("Received from server: %s\n", buffer);
    }

    // Close socket
    close(clientSocket);

    return 0;
}

// it uses sendto() and recvfrom() function rather than send() and recv()

