#include "common.h"

int getIpFromHostname(char *hostname, char **ip)
{
  struct addrinfo hints;
  struct addrinfo *res;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  int status = 0;
  if ((status = getaddrinfo(hostname, "80", &hints, &res)) != 0)
  {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    return 1;
  }

  struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr;
  *ip = (char *)malloc(sizeof(char) * (strlen(inet_ntoa(ipv4->sin_addr)) + 1));
  strcpy(ip[0], inet_ntoa(ipv4->sin_addr));

  freeaddrinfo(res);
  return 0;
}

std::string hostToIp(const std::string &host)
{
  hostent *hostname = gethostbyname(host.c_str());
  if (!hostname)
  {
    exit(EXIT_FAILURE);
  }
  
  return std::string(inet_ntoa(**(in_addr **)hostname->h_addr_list));
}