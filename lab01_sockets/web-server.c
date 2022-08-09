#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "./common/common.h"

int main(int argc, char **argv) {
  if (argc != 4) {
    fprintf(stderr, "Usage: web-server [hostname] [port] [path-to-files]\n");
    return 1;
  }

  char *ip;
  if (get_ip_from_hostname(argv[1], &ip) != 0) {
    perror("get_ip_from_hostname");
    return 2;
  }

  /* Welcome socket */
  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

  /* Allow reuse address */
  int yes = 1;
  if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
    perror("setsockopt");
    return 3;
  }

  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(atoi(argv[2]));
  server_address.sin_addr.s_addr = inet_addr(ip);
  memset(server_address.sin_zero, '\0', sizeof(server_address.sin_zero));

  if (bind(socket_fd, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
    perror("bind");
    return 4;
  }

  if (listen(socket_fd, 1) == -1) {
    perror("listen");
    return 5;
  }

  /* Waits for client */
  struct sockaddr_in client_address;
  socklen_t client_address_size = sizeof(client_address_size);
  int client_socket_fd = accept(socket_fd, (struct sockaddr*)&client_address, &client_address_size);
  if (client_socket_fd == -1) {
    perror("accept");
    return 6;
  }

  printf("Client ipv4: %s\n", inet_ntoa(client_address.sin_addr));
  printf("Sleeps for 10 seconds...\n");
  sleep(10);

  close(client_socket_fd);
  free(ip);
  return 0;
}
