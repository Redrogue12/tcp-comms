#include "common.h"

int prompt = 0;

int main() {
  int server_fd, new_socket;
  struct sockaddr_in address;
  int addrlen = sizeof(address);
  char buffer[1024] = {0};
  int exit_p = 0; // flag for exiting program from inner while loop.

  server_fd = create_socket();

  int opt = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  setup_address(&address, NULL);

  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("Bind failed");
    exit(EXIT_FAILURE);
  }

  if (listen(server_fd, 3) < 0) {
    perror("Listen failed");
    exit(EXIT_FAILURE);
  }
  print_message(GREEN, "Server listening on port ", "8080...\n");

  while (!exit_p) {
    new_socket =
        accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
    if (new_socket < 0) {
      perror("Accept failed");
      exit(EXIT_FAILURE);
    }
    print_message(GREEN, "\nClient connected.\n", "");
    print_message(CYAN, "You: ", "");

    fd_set read_fds;
    while (1) {
      FD_ZERO(&read_fds);
      FD_SET(STDIN_FILENO, &read_fds);
      FD_SET(new_socket, &read_fds);

      select(new_socket + 1, &read_fds, NULL, NULL, NULL);

      if (FD_ISSET(STDIN_FILENO, &read_fds)) {
        print_message(CYAN, "You: ", "");

        memset(buffer, 0, sizeof(buffer));
        fgets(buffer, sizeof(buffer), stdin);
        send(new_socket, buffer, strlen(buffer), 0);

        if (strncmp(buffer, "exit", 4) == 0) {
          print_message(GREEN, "Closing connection.", "");
          exit_p = 1;
          break;
        }
      }

      if (FD_ISSET(new_socket, &read_fds)) {
        memset(buffer, 0, sizeof(buffer));
        int valread = read(new_socket, buffer, sizeof(buffer));
        if (valread > 0) {
          print_message(YELLOW, "\nClient: ", buffer);
          print_message(CYAN, "You: ", "");
        } else if (valread == 0) {
          print_message(GREEN, "\nClient disconnected.", "");
          break;
        }
      }
    }
    close(new_socket);
  }

  close(server_fd);
  return 0;
}