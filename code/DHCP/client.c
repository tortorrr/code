#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#define BUFFSIZE 512

int main() {
  int client_socket;
  struct sockaddr_in server_address;

  client_socket = socket(AF_INET, SOCK_DGRAM, 0);

  server_address.sin_family = AF_INET;
  inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr);
  server_address.sin_port = htons(8080);

  connect(client_socket, (struct sockaddr *)&server_address,
          sizeof(server_address));

  printf("Connected to server\n");

  char yiaddr[BUFFSIZE] = "0.0.0.0";
  int transID = 0, lifetime;

  printf("DHCP 4 way HandShake:\n");
  socklen_t client_address_size = sizeof(server_address);
  printf("Discover: %s, transID:%d\n", yiaddr, transID);
  sendto(client_socket, &yiaddr, sizeof(yiaddr), 0,
         (struct sockaddr *)&server_address, sizeof(server_address));
  sendto(client_socket, &transID, sizeof(transID), 0,
         (struct sockaddr *)&server_address, sizeof(server_address));

  printf("\nDHCP offer:\n");
  recvfrom(client_socket, &yiaddr, sizeof(yiaddr), 0,
           (struct sockaddr *)&server_address, &client_address_size);
  recvfrom(client_socket, &transID, sizeof(transID), 0,
           (struct sockaddr *)&server_address, &client_address_size);
  recvfrom(client_socket, &lifetime, sizeof(lifetime), 0,
           (struct sockaddr *)&server_address, &client_address_size);
  printf("Address assigned: %s, transID: %d, lifetime: %d\n", yiaddr, transID,
         lifetime);

  printf("\nDHCP request:\n");
  sendto(client_socket, &yiaddr, sizeof(yiaddr), 0,
         (struct sockaddr *)&server_address, sizeof(server_address));
  sendto(client_socket, &transID, sizeof(transID), 0,
         (struct sockaddr *)&server_address, sizeof(server_address));
  sendto(client_socket, &lifetime, sizeof(lifetime), 0,
         (struct sockaddr *)&server_address, sizeof(server_address));
  printf("Address request : %s, transID: %d, lifetime: %d\n", yiaddr, transID,
         lifetime);

  printf("\nDHCP Acknowledge:\n");
  recvfrom(client_socket, &yiaddr, sizeof(yiaddr), 0,
           (struct sockaddr *)&server_address, &client_address_size);
  recvfrom(client_socket, &transID, sizeof(transID), 0,
           (struct sockaddr *)&server_address, &client_address_size);
  recvfrom(client_socket, &lifetime, sizeof(lifetime), 0,
           (struct sockaddr *)&server_address, &client_address_size);
  printf("Acknowledgement Address : %s, transID: %d, lifetime: %d\n", yiaddr,
         transID, lifetime);

  time_t curr;
  time(&curr);
  while (1) {
    time_t next;
    time(&next);
    double elapsed = difftime(next, curr);

    if (elapsed >= (lifetime / 2)) {
      int choice = 0;
      printf("\nContinue Same Packet? 1:Yes, 2:No\n");
      scanf("%d", &choice);
      sendto(client_socket, &choice, sizeof(choice), 0,
             (struct sockaddr *)&server_address, sizeof(server_address));
      if (choice != 1) {
        break;
      }
      // reset timer
      time(&curr);
    }
  }
  close(client_socket);
  return 0;
}
