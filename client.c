#include "common.h"

int main(int argc, char *argv[]) {
  int socket;
  struct sockaddr_in server_address;
  char buffer[1024] = {0};
  const char *server_ip = "127.0.0.1";

  if (argc > 1) {
    server_ip = argv[1];
  }

  socket = create_socket();
  setup_address(&server_address, server_ip);

  if (connect(socket, (struct sockaddr *)&server_address,
              sizeof(server_address)) < 0) {
    perror("Connection Failed");
    exit(EXIT_FAILURE);
  }

  printf(GREEN "Connected to server. Type 'exit' to quit.\n" RESET_COLOR);

  fd_set read_fds;
  while (1) {
    FD_ZERO(&read_fds);
    FD_SET(STDIN_FILENO, &read_fds);
    FD_SET(socket, &read_fds);

    select(socket + 1, &read_fds, NULL, NULL, NULL);

    if (FD_ISSET(STDIN_FILENO, &read_fds)) {
      printf(CYAN "You: " RESET_COLOR);
      fflush(stdout);
      
      memset(buffer, 0, sizeof(buffer));
      fgets(buffer, sizeof(buffer), stdin);
      send(socket, buffer, strlen(buffer), 0);

      if (strncmp(buffer, "exit", 4) == 0) {
        printf(GREEN "Closing connection.\n" RESET_COLOR);
        break;
      }
    }

    if (FD_ISSET(socket, &read_fds)) {
      printf(CYAN "You: " RESET_COLOR);
      fflush(stdout);

      memset(buffer, 0, sizeof(buffer));
      int valread = read(socket, buffer, sizeof(buffer));
      if (valread > 0) {
        printf("\n" YELLOW "Server: %s" RESET_COLOR, buffer);
      } else if (valread == 0) {
        printf(GREEN "\nServer closed the connection.\n" RESET_COLOR);
        break;
      }
    }
  }

  close(socket);
  return 0;
}
