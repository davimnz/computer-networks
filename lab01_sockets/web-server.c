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


int register_server(server *server, char *ip, int port);

int register_client(client **client, struct sockaddr_in address, socklen_t address_len, int socket);

struct thread_pool {
  int size;
  pthread_t *threads;
  bool *busy_status;
};
typedef struct thread_pool thread_pool;

void new_thread_pool(thread_pool *pool, int pool_size);

int get_free_thread(thread_pool *pool);

int handle_task(thread_pool *pool, void *task, void *args);

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

  int thread_count = 3;
  thread_pool pool;
  new_thread_pool(&pool, thread_count);

  while (true)
  {
    struct sockaddr_in conn_addr;
    socklen_t conn_addr_len;

    int socket = accept(server.socket, (struct sockaddr *)&conn_addr, &conn_addr_len);
    if (socket == -1)
    {
      perror("accept");
      continue;
    }

    client *conn = malloc(sizeof(client));
    register_client(&conn, conn_addr, conn_addr_len, socket);

    if (handle_task(&pool, handle_connection, conn) == -1) {
      close(socket);
      free(conn);
      continue;
    }
  }

  free(ip);
  return 0;
}

int register_server(server *server, char *ip, int port) {
  (server->addr).sin_family = AF_INET;
  (server->addr).sin_addr.s_addr = inet_addr(ip);
  (server->addr).sin_port = htons(port);
  memset((server->addr).sin_zero, '\0', sizeof((server->addr).sin_zero)); // Not used when family is IPV4

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

int register_client(client **client, struct sockaddr_in address, socklen_t address_len, int socket) {
  (*client)->addr = address;
  (*client)->addr_len = address_len;
  (*client)->socket = socket;
}

void new_thread_pool(thread_pool *pool, int pool_size) {
  pool->size = pool_size;
  pool->threads = malloc(pool_size * sizeof(pthread_t));
  pool->busy_status = calloc(pool_size, sizeof(bool));
}

int get_free_thread(thread_pool *pool) {
  for (int i = 0; i < pool->size; i++) {
    if (pool->busy_status[i] == false) {
      return i;
    }
  }
  return -1;
}

int handle_task(thread_pool *pool, void *task, void *args) {
  int thread_num = get_free_thread(pool);
  if (thread_num == -1) {
    fprintf(stderr, "Refusing connection: maximum number of threads reached\n");
    return -1;
  }

  bool *busy_status = &(pool->busy_status[thread_num]);
  pthread_t *thread = &(pool->threads[thread_num]);

  *busy_status = true;

  // spawn thread
  // pass in task and args, probably need to pass the pool to make thread available after task completes?
  pthread_create(&(*thread), NULL, task, args);
}

void *handle_connection(client *client)
{
  printf("Client %s connected\n", inet_ntoa(client->addr.sin_addr));

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