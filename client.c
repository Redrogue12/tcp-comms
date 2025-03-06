#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define PORT 8080

#define RESET_COLOR "\033[0m"
#define CYAN "\033[96m"
#define YELLOW "\033[93m"
#define GREEN "\033[92m"

int main()
{
  int sock;
  struct sockaddr_in server_address;
  char buffer[1024] = {0};

  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0)
  {
    perror("Socket creation error");
    exit(EXIT_FAILURE);
  }

  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORT);
  inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr);

  if (connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
  {
    perror("Connection Failed");
    exit(EXIT_FAILURE);
  }

  printf(GREEN "Connected to server. Type 'exit' to quit.\n" RESET_COLOR);

  fd_set read_fds;
  while (1)
  {
    printf(CYAN "You: " RESET_COLOR);
    fflush(stdout);

    FD_ZERO(&read_fds);
    FD_SET(STDIN_FILENO, &read_fds);
    FD_SET(sock, &read_fds);

    select(sock + 1, &read_fds, NULL, NULL, NULL);

    if (FD_ISSET(STDIN_FILENO, &read_fds))
    {
      memset(buffer, 0, sizeof(buffer));
      fgets(buffer, sizeof(buffer), stdin);
      send(sock, buffer, strlen(buffer), 0);

      if (strncmp(buffer, "exit", 4) == 0)
      {
        printf(GREEN "Closing connection.\n" RESET_COLOR);
        break;
      }
    }

    if (FD_ISSET(sock, &read_fds))
    {
      memset(buffer, 0, sizeof(buffer));
      int valread = read(sock, buffer, sizeof(buffer));
      if (valread > 0)
      {
        printf("\n" YELLOW "Server: %s" RESET_COLOR, buffer);
        printf(CYAN "You: " RESET_COLOR);
        fflush(stdout);
      }

      if (strncmp(buffer, "exit", 4) == 0)
      {
        printf(GREEN "\nServer closed the connection.\n" RESET_COLOR);
        break;
      }
    }
  }

  close(sock);
  return 0;
}
