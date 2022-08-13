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

#include "./common/common.h"

int handle_connection(int* socket_fd);

int main(int argc, char **argv) {
  if (argc != 4) {
    fprintf(stderr, "Usage: web-server [hostname] [port] [path-to-files]\n");
    return 1;
  }

  char *hostname = argv[1];
  char *port = argv[2];
  char *path = argv[3];

  char *ip;
  if (get_ip_from_hostname(hostname, &ip) != 0) {
    perror("get_ip_from_hostname");
    return 2;
  }

  /* Welcome socket */
  int socket_fd = socket(
    AF_INET, // IPV4
    SOCK_STREAM, // TCP
    0);

  // /* Set welcome socket as non-blocking */
  // int flags;
  // if ((flags = fcntl(socket_fd, F_GETFL, 0)) == -1)
  //   flags = 0;
  // fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK);

  /* Allow reuse address */
  int yes = 1;
  if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
    perror("setsockopt");
    free(ip);
    return 3;
  }

  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(atoi(port));
  server_address.sin_addr.s_addr = inet_addr(ip);
  memset(server_address.sin_zero, '\0', sizeof(server_address.sin_zero)); // Not used when family is IPV4

  if (bind(socket_fd, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
    perror("bind");
    free(ip);
    return 4;
  }

  if (listen(socket_fd, 1) == -1) {
    perror("listen");
    free(ip);
    return 5;
  }

  struct sockaddr_in client_address;
  socklen_t client_address_size = sizeof(client_address);
  int client_socket_fd;
  
  while(1) {
    client_socket_fd = accept(socket_fd, (struct sockaddr*)&client_address, &client_address_size);
    if (client_socket_fd != -1) {
      printf("Client IPv4: %s ; Client fd: %d\n", inet_ntoa(client_address.sin_addr), client_socket_fd);

      handle_connection(&client_socket_fd);
    }
  }


  close(client_socket_fd);
  free(ip);
  return 0;
}


int handle_connection(int* socket_fd) {
  char buffer[1024] = {'\0'};

  while (true) {
    memset(buffer, '\0', sizeof(buffer));

    // recebe ate 20 bytes do cliente remoto
    if (recv(*socket_fd, buffer, 1024, 0) == -1) {
      perror("recv");
      return 5;
    }

    printf("%s", buffer);

    // envia de volta o buffer recebido como um echo
    if (send(*socket_fd, buffer, 1024, 0) == -1) {
      perror("send");
      return 6;
    }
  }
}