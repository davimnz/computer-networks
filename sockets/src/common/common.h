#ifndef COMMON_H
#define COMMON_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sstream>
#include <iostream>

int getIpFromHostname(char *, char **);
std::string hostToIp(const std::string &);

typedef struct Url
{
  std::string hostname;
  int port;
  std::string route;
} Url;

Url parseUrl(std::string &);

typedef struct HTTPRequest
{
  std::string method;
  std::string route;
  std::string protocol;
} HTTPRequest;

typedef struct HTTPResponse
{
  std::string protocol;
  int code;
  std::string status;
  int contentLength;
  std::string body;
} HTTPResponse;

std::string httpRequestToString(HTTPRequest);
HTTPRequest parseRequest(std::string &);
std::string httpResponseToString(HTTPResponse);
HTTPResponse parseResponse(std::string &);

#endif
