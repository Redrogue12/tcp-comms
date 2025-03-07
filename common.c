#include "common.h"

int create_socket() {
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == 0) {
    perror("Socket failed");
    exit(EXIT_FAILURE);
  }
  return sockfd;
}

void setup_address(struct sockaddr_in *address, const char *ip) {
  address->sin_family = AF_INET;
  address->sin_port = htons(PORT);

  if (ip == NULL) {
    address->sin_addr.s_addr = INADDR_ANY;
  } else {
    inet_pton(AF_INET, ip, &address->sin_addr);
  }
}