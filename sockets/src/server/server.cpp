#include "server.h"

Server::Server(char *hostname, int port, char *filesPath)
{
  this->hostname = hostname;
  this->ip = hostToIp(hostname);
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

void Server::handleConnection(int connection, Server server)
{
  char buffer[1024];

  memset(buffer, '\0', sizeof(buffer));

  if (recv(connection, buffer, 1024, 0) == -1)
  {
    perror("recv");
  }

  std::string buff = buffer;

  auto request = parseRequest(buff);

  std::cout << "Request: " << request.method << " " << request.route << std::endl;

  auto response = Server::handleRequest(request, server.filesPath);

  std::cout << "Response: " << response.code << " " << response.status << std::endl;

  std::string responseString = httpResponseToString(response);

  if (send(connection, responseString.c_str(), responseString.size(), 0) == -1)
  {
    perror("send");
  }
}

HTTPResponse Server::handleRequest(HTTPRequest &request, std::string filesPath)
{
  HTTPResponse response = {
      "HTTP/1.1",
      200,
      "OK",
  };

  std::string filePath;

  if (request.route.compare("/") == 0)
  {
    filePath = filesPath + "/index.html";
  }
  else if (request.route.compare("/sleep") == 0)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    filePath = filesPath + "/index.html";
  }
  else
  {
    filePath = filesPath + request.route + ".html";
  }

  std::ifstream ifs(filePath);

  if (!ifs.good())
  {
    ifs.close();
    ifs.open(filesPath + "/404.html");

    response.code = 404;
    response.status = "NOT FOUND";
  }

  std::stringstream content;
  content << ifs.rdbuf();

  response.body = content.str();
  response.contentLength = response.body.size();
  ifs.close();

  return response;
}

void Server::close()
{
  ::close(socket);
}