#ifndef CLIENT_H
#define CLIENT_H

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

class Client
{
public:
  struct sockaddr_in sockaddr;
  int socket;

  void configure(std::string, int);
  void connect();
  HTTPResponse makeRequest(HTTPRequest);
  void close();
};

#endif
