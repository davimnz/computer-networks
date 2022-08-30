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
  static void handleConnection(int, std::string);
  void close();
};

std::string parseRequest(std::string);
int handleRequest(std::string, std::string, std::string *, std::string *);
std::string composeResponse(std::string, std::string, int);

#endif
