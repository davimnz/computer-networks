#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "./common/request.h"
#include "./common/common.h"

void parse_url(char*, char**, char**, char**);

int main(int argc, char **argv) {
  char *hostname;
  char *ip;
  char *object;
  char *port;
  for (int i = 1; i < argc; i++) {
    parse_url(argv[i], &hostname, &port, &object);
    if (get_ip_from_hostname(hostname, &ip) != 0) {
      perror("get_ip_from_hostname");
      return 1;
    }

    /* Connect to server */
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(atoi(port));
    server_address.sin_addr.s_addr = inet_addr(ip);
    memset(server_address.sin_zero, '\0', sizeof(server_address.sin_zero));

    printf("Object: %s\n", object);
    printf("Hostname: %s\n", hostname);
    printf("Port: %d\n", atoi(port));
    printf("Ip: %s\n", ip);

    if (connect(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
      perror("connect");
      return 2;
    }

    printf("Sleeps for 20 seconds...\n");
    sleep(15);

    free(object);
    free(hostname);
    free(port);
    free(ip);
    close(socket_fd);
  }
  return 0;
}

void parse_url(char *url, char **hostname, char **port, char **object) {
  const char slash[2] = "/";
  char *token = strtok(url, slash);

  /* Find hostname */
  token = strtok(NULL, slash);
  size_t hostname_size = 0;
  for (size_t i = 0; i < strlen(token); i++) {
    if (token[i] == ':') {
      hostname_size = i;
      break;
    }
  }
  *hostname = (char*) malloc(sizeof(char)*(hostname_size + 1));
  for (int i = 0; i < hostname_size; i++)
    hostname[0][i] = token[i];
  hostname[0][hostname_size] = '\0';

  /* Find port */
  *port = (char*) malloc(sizeof(char)*(strlen(token) - hostname_size));
  for (int i = hostname_size + 1, j = 0; i < strlen(token); i++, j++)
    port[0][j] = token[i];
  port[0][strlen(token) - hostname_size - 1] = '\0';

  /* Find index */
  token = strtok(NULL, slash);
  *object = (char*) malloc(sizeof(char)*(strlen(token) + 1));
  strcpy(*object, token);
}
