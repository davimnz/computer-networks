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

struct client {
  struct sockaddr_in addr;
  socklen_t addr_len;
  int socket;
};
typedef struct client client;


void register_server(server *server, char *ip, int port);

void *handle_connection(client *client);

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

  client client;

  while (true)
  {
    client.socket = accept(server.socket, (struct sockaddr *)&client.addr, &client.addr_len);
    if (client.socket == -1)
    {
      perror("accept");
      continue;
    }

    printf("Client %s connected\n", inet_ntoa(client.addr.sin_addr));

    handle_connection(&client);
  }

  close(client.socket);
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

void *handle_connection(client *client)
{
  char buffer[1024] = {'\0'};

  while (true)
  {
    memset(buffer, '\0', sizeof(buffer));

    if (recv(client->socket, buffer, 1024, 0) == -1)
    {
      perror("recv");
      // return;
    }

    printf("%s", buffer);

    if (send(client->socket, buffer, 1024, 0) == -1)
    {
      perror("send");
      // return;
    }
  }
}