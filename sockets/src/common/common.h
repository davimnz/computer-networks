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

#define DEFAULT_PORT 8080
#define DEFAULT_HOSTNAME "localhost"
#define HTTP_PROTOCOL "HTTP/1.0"

#define OK_STATUS "OK"
#define OK_CODE 200

#define NOT_FOUND_STATUS "NOT FOUND"
#define NOT_FOUND_CODE 404

#define BAD_REQUEST_STATUS "BAD REQUEST"
#define BAD_REQUEST_CODE 400

#define GET_METHOD "GET"

#define CONTENT_LENGTH_HEADER "Content-Length"
#define CONTENT_DISPOSITION_HEADER "Content-Disposition"
#define CONTENT_TYPE_HEADER "Content-Type"

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
  std::string contentDisposition;
  std::string contentType;
  std::string body;
} HTTPResponse;

std::string getHeader(std::string, std::string &, size_t);
std::string getFileNameFromContentDisposition(std::string &);
std::string httpRequestToString(HTTPRequest);
HTTPRequest parseRequest(std::string &);
std::string httpResponseToString(HTTPResponse);
HTTPResponse parseResponse(std::string &);

#endif
