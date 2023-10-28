#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

#define BUFFER 1024
#define PORT 8080
#define FILE_NAME "IPaddress.txt"

typedef struct Packet {
    char ipAddr[BUFFER];
    int transID, lifeTime;
} Packet;

void removeIPFromFile(char* ipToRemove) {
    FILE* tempFile = fopen("temp_ip.txt", "w");
    FILE* file = fopen(FILE_NAME, "r");

    char buffer[BUFFER];
    int found = 0;

    while (fscanf(file, "%s", buffer) != EOF) {
        if (strcmp(buffer, ipToRemove) != 0) {
            fprintf(tempFile, "%s\n", buffer);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove(FILE_NAME);
    rename("temp_ip.txt", FILE_NAME);
}

int main() {
    int server_socket;
    struct sockaddr_in server_address, client_address;
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed\n");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        perror("Setsockopt failed");
        exit(EXIT_FAILURE);
    }

    // Set server details
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address)) == -1) {
        perror("Bind failed\n");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);
    
    printf("DHCP 4-way handshake begins\n");
    socklen_t client_address_length = sizeof(client_address);
    
    // Receive the DHCP Discover and check for errors
    printf("\nDHCP Discover:\n");
    Packet *pck = (Packet*)malloc(sizeof(Packet));
    if (recvfrom(server_socket, pck, sizeof(Packet), 0, (struct sockaddr *)&client_address, &client_address_length) == -1) {
        perror("Error receiving DHCP Discover");
        exit(EXIT_FAILURE);
    }
    printf("Discover: %s\ttransID: %d\n", pck -> ipAddr, pck -> transID);

    // Read IP address from the file
    FILE *fptr = fopen(FILE_NAME, "r");
    if (fscanf(fptr, "%s", pck -> ipAddr) == EOF) {
        perror("Failed to read IP address from file");
        exit(EXIT_FAILURE);
    }
    fclose(fptr);
    removeIPFromFile(pck -> ipAddr);

    // Send the DHCP Offer and check for errors
    printf("\nDHCP Offer:\n");
    pck -> lifeTime = 10;
    if (sendto(server_socket, pck, sizeof(Packet), 0, (struct sockaddr *)&client_address, sizeof(client_address)) == -1) {
        perror("Error sending DHCP Offer");
        exit(EXIT_FAILURE);
    }
    printf("Address assigned: %s\ttransID: %d\tLifetime: %d\n", pck -> ipAddr, pck -> transID, pck -> lifeTime);

    // Receive the DHCP Request and check for errors
    printf("\nDHCP Request:\n");
    if (recvfrom(server_socket, pck, sizeof(Packet), 0, (struct sockaddr *)&client_address, &client_address_length) == -1) {
        perror("Error receiving DHCP Request");
        exit(EXIT_FAILURE);
    }
    printf("Address request: %s\ttransID: %d\tLifetime: %d\n", pck -> ipAddr, pck -> transID, pck -> lifeTime);

    // Send the DHCP Acknowledge and check for errors
    printf("DHCP Acknowledge:\n");
    if (sendto(server_socket, pck, sizeof(Packet), 0, (struct sockaddr *)&client_address, sizeof(client_address)) == -1) {
        perror("Error sending DHCP Acknowledge");
        exit(EXIT_FAILURE);
    }
    printf("Acknowledgement Address: %s\ttransID: %d\tLifetime: %d\n", pck -> ipAddr, pck -> transID, pck -> lifeTime);

    time_t current = time(NULL);
    while (1) {
        time_t next = time(NULL);
        double elapsed = difftime(next, current);

        if (elapsed >= (pck -> lifeTime / 2)) {
            int choice = -1;
            if (recvfrom(server_socket, &choice, sizeof(choice), 0, (struct sockaddr *)&client_address, &client_address_length) == -1) {
                perror("Error receiving DHCP Discover");
                exit(EXIT_FAILURE);
            }

            if (choice != 1) { break; }
            else {
                printf("\nClient chose to continue with same IP %s\n", pck -> ipAddr);
                current = time(NULL); // reset timer
            }
        }
    }
    // After the client exits, append the same IP back to the file
    FILE* appendFile = fopen(FILE_NAME, "a");
    fprintf(appendFile, "%s", pck -> ipAddr);

    fclose(appendFile);
    close(server_socket);
    free(pck);
    pck = NULL;
    return 0;
}