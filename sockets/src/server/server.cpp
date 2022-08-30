#include "server.h"

Server::Server(char *ip, int port, char *filesPath)
{
  this->ip = ip;
  this->port = port;
  this->filesPath = filesPath;
}

void Server::configure()
{
  // Create a socket (IPv4, TCP)
  socket = ::socket(AF_INET, SOCK_STREAM, 0);
  if (socket == -1)
  {
    std::cout << "Failed to create socket. errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }

  sockaddr.sin_family = AF_INET; // IPv4
  sockaddr.sin_addr.s_addr = INADDR_ANY;
  sockaddr.sin_port = htons(port); // htons is necessary to convert a number to network byte order

  // Allow reuse address
  int yes = 1;
  if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
  {
    std::cout << "Failed to set sockopt. errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }

  if (bind(this->socket, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0)
  {
    std::cout << "Failed to bind to port. errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }
}

void Server::listen()
{
  // Start listening. Hold at most 10 connections in the queue
  if (::listen(socket, 10) < 0)
  {
    std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }
}

int Server::acceptConnection()
{
  // Grab a connection from the queue
  auto addrlen = sizeof(sockaddr);
  int connection = accept(socket, (struct sockaddr *)&sockaddr, (socklen_t *)&addrlen);
  if (connection < 0)
  {
    std::cout << "Failed to grab connection. errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }

  return connection;
}

void Server::handleConnection(int connection)
{
  char buffer[1024];

  while (true)
  {
    memset(buffer, '\0', sizeof(buffer));

    if (recv(connection, buffer, 1024, 0) == -1)
    {
      perror("recv");
    }

    if (strncmp("close", buffer, 5) == 0)
    {
      ::close(connection);
      break;
    }

    std::cout << buffer;

    if (send(connection, buffer, 1024, 0) == -1)
    {
      perror("send");
    }
  }
}

void Server::close()
{
  ::close(socket);
}