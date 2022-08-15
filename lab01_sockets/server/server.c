#include "../common/common.h"
#include "./connection.h"

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

  connection server;
  create_server(&server, ip, atoi(port));

  while (true)
  {
    connection *client = accept_client_connection(server.socket);

    if (client == NULL)
    {
      fprintf(stderr, "Refused connection\n");
      continue;
    }

    pthread_t thread_id;
    pthread_create(&thread_id, NULL, handle_connection, (void *)client);
  }

  free(ip);
  return 0;
}