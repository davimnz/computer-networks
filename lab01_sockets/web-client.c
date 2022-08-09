#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "./common/request.h"

void parse_url(char *, char **, char **, char **);

int main(int argc, char **argv) {
  struct HTTPRequest *requests = (struct HTTPRequest*) malloc((argc - 1)*sizeof(struct HTTPRequest));

  char *hostname;
  char *object;
  char *port;
  for (int i = 1; i < argc; i++) {
    parse_url(argv[i], &hostname, &port, &object);

    printf("Object: %s\n", object);
    printf("Hostname: %s\n", hostname);
    printf("Port: %s\n", port);
    free(object);
    free(hostname);
    free(port);
  }

  free(requests);
  return 0;
}

void parse_url(char *url, char **hostname, char **port, char **object) {
  const char slash[2] = "/";
  char *token = strtok(url, slash);

  // Find hostname
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

  // Find port
  *port = (char*) malloc(sizeof(char)*(strlen(token) - hostname_size));
  for (int i = hostname_size + 1, j = 0; i < strlen(token); i++, j++)
    port[0][j] = token[i];
  port[0][strlen(token) - hostname_size - 1] = '\0';

  // Find index
  token = strtok(NULL, slash);
  *object = (char*) malloc(sizeof(char)*(strlen(token) + 1));
  strcpy(*object, token);
}
