// client.c
#include <arpa/inet.h>
#include <limits.h>
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
  for (int j = 1; j < N; j++)
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
  } while (i < dataLength + N);
}

void initialize() {
  data = (char *)(calloc(sizeof(char), INT_MAX));
  checkValue = (char *)(calloc(sizeof(char), N));
}

int main() {
  int client_socket;
  struct sockaddr_in server_address;
  char message[BUFFER_SIZE];

  // Create a socket
  client_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (client_socket == -1) {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }

  // Set server details
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORT);
  if (inet_pton(AF_INET, "127.0.0.1", &(server_address.sin_addr)) <= 0) {
    perror("Invalid address/Address not supported");
    exit(EXIT_FAILURE);
  }

  // Connect to the server
  if (connect(client_socket, (struct sockaddr *)&server_address,
              sizeof(server_address)) < 0) {
    perror("Connection failed");
    exit(EXIT_FAILURE);
  }

  // to initialize and allocate space
  initialize();

  // Get the message from the user
  printf("Enter message to be transmitted: ");
  scanf("%s", data);

  int dataLength = strlen(data);

  for (int i = dataLength; i < dataLength + N - 1; i++)
    data[i] = '0';

  printf("\nData padded with n-1 zeros: %s", data);

  crc(dataLength);

  printf("\nCRC or Check value is: %s", checkValue);

  for (int i = dataLength; i < dataLength + N - 1; i++)
    data[i] = checkValue[i - dataLength];

  printf("\nFinal data to be sent: %s\nPaste it here: ", data);
  scanf("%s", data);
  printf("%d\n", dataLength);

  // Send the data to the server
  if (send(client_socket, data, dataLength + N, 0) < 0) {
    perror("Send failed");
    exit(EXIT_FAILURE);
  } else {
    printf("Sent successfully!");
  }

  // Close the client socket
  close(client_socket);

  return 0;
}