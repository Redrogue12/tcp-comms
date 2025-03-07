#include "common.h"

int prompt = 0;

int main() {
  int server_fd, new_socket;
  struct sockaddr_in address;
  int addrlen = sizeof(address);
  char buffer[1024] = {0};
  int exit_p = 0;

  server_fd = create_socket();
  setup_address(&address, NULL);

  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("Bind failed");
    exit(EXIT_FAILURE);
  }
  if (listen(server_fd, 3) < 0) {
    perror("Listen failed");
    exit(EXIT_FAILURE);
  }
  printf(GREEN "Server listening on port %d...\n" RESET_COLOR, PORT);

  while (!exit_p)
  {
    new_socket =
        accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
    if (new_socket < 0) {
      perror("Accept failed");
      exit(EXIT_FAILURE);
    }
    printf(GREEN "Client connected.\n" RESET_COLOR);
    printf(CYAN "You: " RESET_COLOR);
    fflush(stdout);

    fd_set read_fds;
    while (1) {
      FD_ZERO(&read_fds);
      FD_SET(STDIN_FILENO, &read_fds);
      FD_SET(new_socket, &read_fds);

      select(new_socket + 1, &read_fds, NULL, NULL, NULL);

      if (FD_ISSET(STDIN_FILENO, &read_fds)) {
        printf(CYAN "You: " RESET_COLOR);
        fflush(stdout);

        memset(buffer, 0, sizeof(buffer));
        fgets(buffer, sizeof(buffer), stdin);
        send(new_socket, buffer, strlen(buffer), 0);

        if (strncmp(buffer, "exit", 4) == 0) {
          printf(GREEN "Closing connection.\n" RESET_COLOR);
          fflush(stdout);
          exit_p = 1;
          break;
        }
      }

      if (FD_ISSET(new_socket, &read_fds)) {
        memset(buffer, 0, sizeof(buffer));
        int valread = read(new_socket, buffer, sizeof(buffer));
        if (valread > 0) {
          printf("\n" YELLOW "Client: %s" RESET_COLOR, buffer);
          fflush(stdout);
        } else if (valread == 0) {
          printf(GREEN "\nClient disconnected.\n" RESET_COLOR);
          fflush(stdout);
          break;
        }
      }
    }
    close(new_socket);
  }

  close(new_socket);
  close(server_fd);
  return 0;
}
