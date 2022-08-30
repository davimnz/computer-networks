#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>
#include <thread>

class Server
{
public:
  char *ip;
  int port;
  char *filesPath;

  struct sockaddr_in sockaddr;
  socklen_t addressLength;
  int socket;

  Server(char *ip, int port, char *filesPath);
  void configure();
  void listen();
  int acceptConnection();
  static void handleConnection(int);
  void close();
};

#endif
