#include "client.h"

void Client::configure(char *hostname, int port)
{
  std::string ip = hostToIp(hostname);

  // Create a socket (IPv4, TCP)
  socket = ::socket(AF_INET, SOCK_STREAM, 0);
  if (socket == -1)
  {
    std::cout << "Failed to create socket. errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }

  sockaddr.sin_family = AF_INET;   // IPv4
  sockaddr.sin_port = htons(port); // htons is necessary to convert a number to network byte order

  if (inet_pton(AF_INET, ip.c_str(), &sockaddr.sin_addr) <= 0)
  {
    std::cout << "Invalid address/Address not supported. errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }
}

void Client::connect()
{
  if (::connect(socket, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0)
  {
    std::cout << "Connection failed. errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }
}

void Client::request()
{
  char hello[] = "GET / HTTP/1.1";

  send(socket, hello, strlen(hello), 0);

  char buffer[1024];
  memset(buffer, '\0', sizeof(buffer));

  if (recv(socket, buffer, 1024, 0) == -1)
  {
    perror("recv");
  }

  std::cout << buffer << std::endl;
}

void Client::close()
{
  ::close(socket);
}