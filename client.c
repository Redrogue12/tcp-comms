#include "common.h"

int main() {
  int socket;
  struct sockaddr_in server_address;
  char buffer[1024] = {0};

  socket = create_socket();
  setup_address(&server_address, "127.0.0.1");

  if (connect(socket, (struct sockaddr *)&server_address,
              sizeof(server_address)) < 0) {
    perror("Connection Failed");
    exit(EXIT_FAILURE);
  }

  printf(GREEN "Connected to server. Type 'exit' to quit.\n" RESET_COLOR);

  fd_set read_fds;
  while (1) {
    printf(CYAN "You: " RESET_COLOR);
    fflush(stdout);

    FD_ZERO(&read_fds);
    FD_SET(STDIN_FILENO, &read_fds);
    FD_SET(socket, &read_fds);

    select(socket + 1, &read_fds, NULL, NULL, NULL);

    if (FD_ISSET(STDIN_FILENO, &read_fds)) {
      memset(buffer, 0, sizeof(buffer));
      fgets(buffer, sizeof(buffer), stdin);
      send(socket, buffer, strlen(buffer), 0);

      if (strncmp(buffer, "exit", 4) == 0) {
        printf(GREEN "Closing connection.\n" RESET_COLOR);
        break;
      }
    }

    if (FD_ISSET(socket, &read_fds)) {
      memset(buffer, 0, sizeof(buffer));
      int valread = read(socket, buffer, sizeof(buffer));
      if (valread > 0) {
        printf("\n" YELLOW "Server: %s" RESET_COLOR, buffer);
        printf(CYAN "You: " RESET_COLOR);
        fflush(stdout);
      }

      if (strncmp(buffer, "exit", 4) == 0) {
        printf(GREEN "\nServer closed the connection.\n" RESET_COLOR);
        break;
      }
    }
  }

  close(socket);
  return 0;
}
