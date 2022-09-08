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

Url parseUrl(std::string &url)
{
  size_t doubleSlashPosition = url.find("//");
  bool hasHttpLead = doubleSlashPosition != std::string::npos;

  size_t hostStartPosition = hasHttpLead ? doubleSlashPosition + 2 : 0;
  size_t hostEndPosition = url.find("/", hostStartPosition);
  bool isRootRoute = hostEndPosition == std::string::npos;
  std::string host = isRootRoute ? url.substr(hostStartPosition) : url.substr(hostStartPosition, hostEndPosition - hostStartPosition);

  size_t colonPosition = host.find(":");
  bool hasPortSpecified = colonPosition != std::string::npos;

  std::string hostname = hasPortSpecified ? host.substr(0, colonPosition) : host;

  std::string port = hasPortSpecified ? host.substr(colonPosition + 1) : "80";

  std::string route = isRootRoute ? "/" : url.substr(hostEndPosition);

  return Url{
      hostname,
      atoi(port.c_str()),
      route,
  };
}

std::string httpRequestToString(HTTPRequest request)
{
  std::stringstream requestSS;
  requestSS << request.method + " " << request.route + " " << request.protocol;

  return requestSS.str();
}

HTTPRequest parseRequest(std::string &req)
{
  size_t methodStartPosition = 0;
  size_t methodEndPosition = req.find(" ", methodStartPosition);
  std::string method = req.substr(methodStartPosition, methodEndPosition - methodStartPosition);

  size_t routeStartPosition = methodEndPosition + 1;
  size_t routeEndPosition = req.find(" ", routeStartPosition);
  std::string route = req.substr(routeStartPosition, routeEndPosition - routeStartPosition);

  size_t protocolStartPosition = routeEndPosition + 1;
  size_t protocolEndPosition = req.find("\r\n", protocolStartPosition);
  std::string protocol = req.substr(protocolStartPosition, protocolEndPosition - protocolStartPosition);

  return HTTPRequest{
      method,
      route,
      protocol,
  };
}

std::string httpResponseToString(HTTPResponse response)
{
  std::stringstream responseSS;
  responseSS << response.protocol + " " << response.code << " " << response.status + "\r\n"
             << "Content-Length: " << response.contentLength << "\r\n\r\n"
             << response.body;

  return responseSS.str();
}

HTTPResponse parseResponse(std::string &res)
{
  size_t protocolStartPosition = 0;
  size_t protocolEndPosition = res.find(" ", protocolStartPosition);
  std::string protocol = res.substr(protocolStartPosition, protocolEndPosition - protocolStartPosition);

  size_t codeStartPosition = protocolEndPosition + 1;
  size_t codeEndPosition = res.find(" ", codeStartPosition);
  std::string code = res.substr(codeStartPosition, codeEndPosition - codeStartPosition);

  size_t statusStartPosition = codeEndPosition + 1;
  size_t statusEndPosition = res.find("\r\n", statusStartPosition);
  std::string status = res.substr(statusStartPosition, statusEndPosition - statusStartPosition);

  size_t contentLengthStartPosition = res.find(":", statusEndPosition) + 1;
  size_t contentLengthEndPosition = res.find("\r\n", contentLengthStartPosition);
  std::string contentLength = res.substr(contentLengthStartPosition, contentLengthEndPosition - contentLengthStartPosition);

  size_t contentStartPosition = contentLengthEndPosition + 4;
  std::string content = res.substr(contentStartPosition);

  return HTTPResponse{
      protocol,
      atoi(code.c_str()),
      status,
      atoi(contentLength.c_str()),
      content,
  };
}