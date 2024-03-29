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
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>

#include "../common/common.h"

class Server
{
public:
  std::string hostname;
  std::string ip;
  int port;
  std::string filesPath;

  struct sockaddr_in sockaddr;
  socklen_t addressLength;
  int socket;

  Server(char *, int, char *);
  void configure();
  void listen();
  int acceptConnection();
  static void handleConnection(int, Server);
  static HTTPResponse handleRequest(HTTPRequest &, std::string);
  void close();
};


#endif
