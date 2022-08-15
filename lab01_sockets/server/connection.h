#ifndef CONNECTION_H
#define CONNECTION_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>
#include <pthread.h>

#include "../common/common.h"

struct connection {
  struct sockaddr_in addr;
  socklen_t addr_len;
  int socket;
};
typedef struct connection connection;

int create_server(connection *server, char *ip, int port);

connection *accept_client_connection(int server_socket);

void *handle_connection(void *client);

#endif
