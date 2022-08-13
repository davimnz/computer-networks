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

#include "./common/common.h"


struct server {
  struct sockaddr_in addr;
  int socket;
};
typedef struct server server;

void register_server(server *server, char *ip, int port);

void *handle_connection(void *socket_fd);

int main(int argc, char **argv)
{
  if (argc != 4)
  {
    fprintf(stderr, "Usage: web-server [hostname] [port] [path-to-files]\n");
    return 1;
  }

  char *hostname = argv[1];
  char *port = argv[2];
  char *path = argv[3];

  char *ip;
  if (get_ip_from_hostname(hostname, &ip) != 0)
  {
    perror("get_ip_from_hostname");
    return 2;
  }

  server server;
  register_server(&server, ip, atoi(port));

  /* Allow reuse address */
  int yes = 1;
  if (setsockopt(server.socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
  {
    perror("setsockopt");
    free(ip);
    return 3;
  }

  /* Bind socket */
  if (bind(server.socket, (struct sockaddr *)&server.addr, sizeof(server.addr)) == -1)
  {
    perror("bind");
    free(ip);
    return 4;
  }

  /* Listen to incoming connections */
  if (listen(server.socket, 1) == -1)
  {
    perror("listen");
    free(ip);
    return 5;
  }

  struct sockaddr_in client_address;
  socklen_t client_address_size = sizeof(client_address);
  int client_socket_fd;

  while (true)
  {
    client_socket_fd = accept(server.socket, (struct sockaddr *)&client_address, &client_address_size);
    if (client_socket_fd == -1)
    {
      perror("accept");
      continue;
    }

    printf("Client %s connected\n", inet_ntoa(client_address.sin_addr));

    handle_connection(&client_socket_fd);
  }

  close(client_socket_fd);
  free(ip);
  return 0;
}

void register_server(server *server, char *ip, int port) {
  (server->addr).sin_family = AF_INET;
  (server->addr).sin_addr.s_addr = inet_addr(ip);
  (server->addr).sin_port = htons(port);
  memset((server->addr).sin_zero, '\0', sizeof((server->addr).sin_zero)); // Not used when family is IPV4

  server->socket = socket(
    AF_INET,     // IPV4
    SOCK_STREAM, // TCP
    0);
}

void *handle_connection(void *socket_fd)
{
  int *fd = (int *) socket_fd;

  char buffer[1024] = {'\0'};

  while (true)
  {
    memset(buffer, '\0', sizeof(buffer));

    if (recv(*fd, buffer, 1024, 0) == -1)
    {
      perror("recv");
      // return;
    }

    printf("%s", buffer);

    if (send(*fd, buffer, 1024, 0) == -1)
    {
      perror("send");
      // return;
    }
  }
}