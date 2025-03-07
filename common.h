#ifndef COMMON_H
#define COMMON_H

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>

#define PORT 8080

#define RESET_COLOR "\033[0m"
#define CYAN "\033[96m"
#define YELLOW "\033[93m"
#define GREEN "\033[92m"

int create_socket();
void setup_address(struct sockaddr_in *address, const char *ip);

extern int prompt;

#endif // COMMON_H