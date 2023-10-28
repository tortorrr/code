#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>

#define BUFFER 1024
#define PORT 8080

typedef struct Packet {
    char ipAddr[BUFFER];
    int transID, lifeTime;
} Packet;

int main() {
    srand(time(NULL));
    struct sockaddr_in server_address;
    int client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(PORT);
    server_address.sin_family = AF_INET;

    printf("DHCP 4-way handshake begins\n");
    socklen_t server_address_length = sizeof(server_address);

    // Send the DHCP Discover and check for errors
    printf("\nDHCP Discover:\n");
    Packet *pck = (Packet*)malloc(sizeof(Packet));
    strcpy(pck -> ipAddr, "0.0.0.0");
    pck -> transID = rand();
    printf("Discover: %s\ttransID: %d\n", pck -> ipAddr, pck -> transID);

    if (sendto(client_socket, pck, sizeof(Packet), 0, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Error sending DHCP Discover");
        exit(EXIT_FAILURE);
    }

    // Receive the DHCP Offer and check for errors
    printf("\nDHCP Offer:\n");
    if (recvfrom(client_socket, pck, sizeof(Packet), 0, (struct sockaddr *)&server_address, &server_address_length) == -1) {
        perror("Error receiving DHCP Offer");
        exit(EXIT_FAILURE);
    }
    printf("Address assigned: %s\ttransID: %d\tLifetime: %d\n", pck -> ipAddr, pck -> transID, pck -> lifeTime);

    // Send the DHCP Request and check for errors    
    printf("\nDHCP Request:\n");
    if (sendto(client_socket, pck, sizeof(Packet), 0, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Error sending DHCP Request");
        exit(EXIT_FAILURE);
    }

    printf("Address request: %s\ttransID: %d\tLifetime: %d\n", pck -> ipAddr, pck -> transID, pck -> lifeTime);

    // Receive the DHCP Acknowledge and check for errors
    printf("\nDHCP Acknowledge:\n");
    if (recvfrom(client_socket, pck, sizeof(Packet), 0, (struct sockaddr *)&server_address, &server_address_length) == -1) {
        perror("Error receiving DHCP Acknowledge");
        exit(EXIT_FAILURE);
    }

    printf("Acknowledgement Address: %s\ttransID: %d\tLifetime: %d\n", pck -> ipAddr, pck -> transID, pck -> lifeTime);

    time_t current = time(NULL);
    while (1) {
        time_t next = time(NULL);
        double elapsed = difftime(next, current);

        if (elapsed >= (pck -> lifeTime / 2)) {
            int choice = -1;
            printf("\nContinue with the same IP? 1: Yes, else: No ->: ");
            scanf("%d", &choice);
            if (sendto(client_socket, &choice, sizeof(choice), 0, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
                perror("Error sending the choice");
                exit(EXIT_FAILURE);
            }
            printf("\nContinuing with same IP %s\n", pck -> ipAddr);
            if (choice != 1) { break; }
            current = time(NULL); // reset timer
        }
    }
    close(client_socket);
    free(pck);
    pck = NULL;
    return 0;
}