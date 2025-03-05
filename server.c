#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
  int server_fd, new_socket;
  struct sockaddr_in address;
  int addrlen = sizeof(address);
  char buffer[1024] = {0};

  // Create socket
  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  
  if (server_fd == 0) {
    perror("Socket failed");
    exit(EXIT_FAILURE);
  }

  // Bind to port
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY; // Accept connections on any interface
  address.sin_port = htons(PORT);

  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("Bind failed");
    exit(EXIT_FAILURE);
  }

  // Start listening
  if (listen(server_fd, 3) < 0) {
    perror("Listen failed");
    exit(EXIT_FAILURE);
  }

  printf("Server listening on port %d...\n", PORT);

  // Accept a connection
  new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
  if (new_socket < 0) {
    perror("Accept failed");
    exit(EXIT_FAILURE);
  }

  while(1) {
    // Receive message
    memset(buffer, 0, sizeof(buffer));
    int valread = read(new_socket, buffer, sizeof(buffer));
    if (valread > 0) {
      printf("Client: %s", buffer);
    }

    // Check for exit condition
    if (strncmp(buffer, "exit", 4) == 0) {
      printf("Client disconnected.\n");
      break;
    }

    // Get user input
    printf("Server: ");
    fgets(buffer, sizeof(buffer), stdin);

    // Send message
    send(new_socket, buffer, strlen(buffer), 0);

    // Exit if user types "exit"
    if (strncmp(buffer, "exit", 4) == 0) {
      printf("Closing connection.\n");
      break;
    }
  }

  close(new_socket);
  close(server_fd);

  return 0;
}
