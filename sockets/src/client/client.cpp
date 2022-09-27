#include "client.h"

void Client::configure(std::string hostname, int port)
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

HTTPResponse Client::makeRequest(HTTPRequest request)
{
  std::cout << "Request: " << request.method + " " << request.route << std::endl;

  std::string requestString = httpRequestToString(request);

  send(socket, requestString.c_str(), requestString.size(), 0);

  std::string responseString = readResponseFromSocket(socket);

  auto response = parseResponse(responseString);

  std::cout << "Response status: " << response.code << " " << response.status << std::endl;

  return response;
}

void Client::handleResponse(HTTPResponse response, std::string &savePath)
{
  if (response.code != OK_CODE)
  {
    return;
  }

  std::cout << "Saving file to " << savePath << std::endl;

  std::ofstream file;
  file.open(savePath);

  file << response.body;

  file.close();
}

std::string Client::getSavePath(std::string &route, std::string &storage)
{
  std::string fileName = route.substr(route.find_last_of("/") + 1);

  if (fileName.compare("") == 0)
  {
    fileName = "index.html";
  }

  return storage + "/" + fileName;
}

void Client::close()
{
  ::close(socket);
}