#include "server.h"

Server::Server(char *hostname, int port, char *filesPath)
{
  char *ip;
  getIpFromHostname(hostname, &ip);

  this->hostname = hostname;
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

  std::cout << "Listening on " << this->hostname << ":" << this->port << std::endl;
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

void Server::handleConnection(int connection, std::string filesPath)
{
  char buffer[1024];

  while (true)
  {
    memset(buffer, '\0', sizeof(buffer));

    if (recv(connection, buffer, 1024, 0) == -1)
    {
      perror("recv");
    }

    std::string request = buffer;

    std::string filename = parseRequest(request);

    std::string file;
    std::string status;

    if (handleRequest(filesPath, filename, &file, &status) != 0)
    {
      break;
    }

    std::string response = composeResponse(status, file, file.length());

    if (send(connection, response.c_str(), response.size(), 0) == -1)
    {
      perror("send");
    }
  }
}

std::string parseRequest(std::string req)
{
  size_t startPosition = req.find("/") + 1;
  size_t endPosition = req.find("HTTP") - 1;

  std::string name = req.substr(startPosition, endPosition - startPosition);

  if (name.empty())
  {
    name = "index";
  }

  std::stringstream filename;
  filename << name << ".html";

  return filename.str();
}

int handleRequest(std::string filesPath, std::string filename, std::string *file, std::string *status)
{
  if (filename.compare("sleep.html") == 0)
  {
    filename = "index.html";
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
  }

  std::ifstream ifs(filesPath + "/" + filename);

  if (filename.compare("404.html") == 0 || !ifs.good())
  {
    ifs.close();
    ifs.open(filesPath + "/" + "404.html");

    *status = "HTTP/1.1 404 NOT FOUND";
  }
  else
  {
    *status = "HTTP/1.1 200 OK";
  }

  std::stringstream content;
  content << ifs.rdbuf();

  *file = content.str();
  ifs.close();

  return 0;
}

std::string composeResponse(std::string status, std::string file, int file_length)
{
  std::stringstream response;
  response << status << "\r\nContent-Length: " << file_length << "\r\n\r\n"
           << file;
  return response.str();
}

void Server::close()
{
  ::close(socket);
}