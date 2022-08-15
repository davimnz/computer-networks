#include "connection.h"

int create_server(connection *server, char *ip, int port)
{
  (server->addr).sin_family = AF_INET;
  (server->addr).sin_addr.s_addr = inet_addr(ip);
  (server->addr).sin_port = htons(port);
  memset((server->addr).sin_zero, '\0', sizeof((server->addr).sin_zero)); // Not used when family is IPV4
  server->addr_len = sizeof(server->addr);

  server->socket = socket(
      AF_INET,     // IPV4
      SOCK_STREAM, // TCP
      0);

  /* Allow reuse address */
  int yes = 1;
  if (setsockopt(server->socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
  {
    perror("setsockopt");
    free(ip);
    return 3;
  }

  /* Bind socket */
  if (bind(server->socket, (struct sockaddr *)&server->addr, sizeof(server->addr)) == -1)
  {
    perror("bind");
    free(ip);
    return 4;
  }

  /* Listen to incoming connections */
  if (listen(server->socket, 1) == -1)
  {
    perror("listen");
    free(ip);
    return 5;
  }

  return 0;
}

connection *accept_client_connection(int server_socket)
{
  struct sockaddr_in address;
  socklen_t address_len;

  int client_socket = accept(server_socket, (struct sockaddr *)&address, &address_len);
  if (client_socket == -1)
  {
    perror("accept");
    return NULL;
  }

  connection *client = malloc(sizeof(connection));

  client->addr = address;
  client->addr_len = address_len;
  client->socket = client_socket;

  return client;
}

void *handle_connection(void *conn)
{
  connection *client = (connection *)conn;

  printf("Client %s connected\n", inet_ntoa(client->addr.sin_addr));

  char buffer[1024] = {'\0'};

  while (true)
  {
    memset(buffer, '\0', sizeof(buffer));

    if (recv(client->socket, buffer, 1024, 0) == -1)
    {
      perror("recv");
    }

    if (strncmp("close", buffer, 5) == 0)
    {
      close_connection(client);
      return NULL;
    }

    printf("%s", buffer);

    if (send(client->socket, buffer, 1024, 0) == -1)
    {
      perror("send");
    }
  }
}

int close_connection(connection *client)
{
  int status = close(client->socket);
  if (status != 0)
  {
    perror("close");
    return status;
  }

  free((void *)client);

  return 0;
}