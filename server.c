#include "common.h"

int main() {
  int server_fd, new_socket;
  struct sockaddr_in address;
  int addrlen = sizeof(address);
  char buffer[1024] = {0};

  server_fd = create_socket();
  setup_address(&address, NULL);

  bind(server_fd, (struct sockaddr *)&address, sizeof(address));
  listen(server_fd, 3);
  printf(GREEN "Server listening on port %d...\n" RESET_COLOR, PORT);

  new_socket =
      accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
  printf(GREEN "Client connected.\n" RESET_COLOR);

  fd_set read_fds;
  while (1) {
    printf(CYAN "You: " RESET_COLOR);
    fflush(stdout);

    FD_ZERO(&read_fds);
    FD_SET(STDIN_FILENO, &read_fds);
    FD_SET(new_socket, &read_fds);

    select(new_socket + 1, &read_fds, NULL, NULL, NULL);

    if (FD_ISSET(STDIN_FILENO, &read_fds)) {
      memset(buffer, 0, sizeof(buffer));
      fgets(buffer, sizeof(buffer), stdin);
      send(new_socket, buffer, strlen(buffer), 0);

      if (strncmp(buffer, "exit", 4) == 0) {
        printf(GREEN "Closing connection.\n" RESET_COLOR);
        break;
      }
    }

    if (FD_ISSET(new_socket, &read_fds)) {
      memset(buffer, 0, sizeof(buffer));
      int valread = read(new_socket, buffer, sizeof(buffer));
      if (valread > 0) {
        printf("\n" YELLOW "Client: %s" RESET_COLOR, buffer);
        printf(CYAN "You: " RESET_COLOR);
        fflush(stdout);
      }

      if (strncmp(buffer, "exit", 4) == 0) {
        printf(GREEN "\nClient disconnected.\n" RESET_COLOR);
        break;
      }
    }
  }

  close(new_socket);
  close(server_fd);
  return 0;
}
