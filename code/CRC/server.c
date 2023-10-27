// CRC_Server.c
#include <limits.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define PORT 8086
#define GEN_POLY "1011"
#define N strlen(GEN_POLY)

char *data;
char *checkValue;

void XOR() {
  for (int j = 0; j < N; j++)
    checkValue[j] = (checkValue[j] == GEN_POLY[j]) ? '0' : '1';
}

void crc(int dataLength) {
  int i;
  for (i = 0; i < N; i++)
    checkValue[i] = data[i];

  do {
    if (checkValue[0] == '1')
      XOR();

    int j;
    for (j = 0; j < N - 1; j++)
      checkValue[j] = checkValue[j + 1];

    checkValue[j] = data[i++];
  } while (i < dataLength);
}

void receiver(int dataLength) {
  printf("Data received: %s", data);
  crc(dataLength);
  int i;
  for (i = 0; (i < N) && (checkValue[i] == '0'); i++)
    ;

  if (i < N)
    printf("\nError detected");
  else
    printf("\nNo error detected");
}

void initialize() {
  data = (char *)(calloc(sizeof(char), INT_MAX));
  checkValue = (char *)(calloc(sizeof(char), N));
}

int main() {
  int server_socket, client_socket;
  struct sockaddr_in server_address, client_address;
  int client_address_length;

  // Create a socket
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1) {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }

  // Set SO_REUSEADDR option
  int opt = 1;
  if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) <
      0) {
    perror("Setsockopt failed");
    exit(EXIT_FAILURE);
  }

  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = INADDR_ANY;
  server_address.sin_port = htons(PORT);

  if (bind(server_socket, (struct sockaddr *)&server_address,
           sizeof(server_address)) < 0) {
    perror("Bind failed");
    exit(EXIT_FAILURE);
  }

  if (listen(server_socket, 1) < 0) {
    perror("Listen failed");
    exit(EXIT_FAILURE);
  }

  printf("Server listening on port %d\n", PORT);

  client_address_length = sizeof(client_address);
  client_socket = accept(server_socket, (struct sockaddr *)&client_address,
                         (socklen_t *)&client_address_length);
  if (client_socket < 0) {
    perror("Accept failed");
    exit(EXIT_FAILURE);
  }

  printf("Connected by client\n");

  // to initialize and allocate memory
  initialize();

  // Receive the data from the client
  if (recv(client_socket, data, INT_MAX, 0) < 0) {
    perror("Receive failed");
    exit(EXIT_FAILURE);
  }

  // the data is received with n - 1 0's, so no need to add 'N' at the CRC()
  int dataLength = strlen(data);
  printf("%d\n", dataLength);
  receiver(dataLength);

  // Close the sockets
  close(client_socket);
  close(server_socket);

  return 0;
}