#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
  int sock = 0;
  struct sockaddr_in serv_addr;
  char buffer[1024] = {0};

  // Create socket
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    perror("Socket creation error");
    return -1;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  // Convert address and connect
  if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
    perror("Invalid address / Address not supported");
    return -1;
  }

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    perror("Connection failed");
    return -1;
  }

  printf("Connected to server. Type 'exit' to quit.\n");

  while (1) {
    // Get user input
    printf("Client: ");
    fgets(buffer, sizeof(buffer), stdin);

    // Send message
    send(sock, buffer, strlen(buffer), 0);

    // Exit if user types "exit"
    if (strncmp(buffer, "exit", 4) == 0) {
      printf("Closing connection.\n");
      break;
    }

    // Receive response
    memset(buffer, 0, sizeof(buffer));
    int valread = read(sock, buffer, sizeof(buffer));
    if (valread > 0) {
      printf("Server: %s", buffer);
    }

    // Exit if server sends "exit"
    if (strncmp(buffer, "exit", 4) == 0) {
      printf("Server closed the connection.\n");
      break;
    }
  }

  close(sock);
  return 0;
}
