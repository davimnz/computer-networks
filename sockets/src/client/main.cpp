#include <iostream>
#include <fstream>
#include "client.h"

#define DEFAULT_ASSETS_FOLDER "recv"

int main(int argc, char **argv)
{
  int numUrls = argc - 1;
  Url *urls = new Url[numUrls];

  for (int i = 0; i < numUrls; ++i)
  {
    std::string argument = argv[i + 1];
    urls[i] = parseUrl(argument);
  }

  Client client;
  std::string storage = DEFAULT_ASSETS_FOLDER;

  for (int i = 0; i < numUrls; i++)
  {
    Url url = urls[i];

    client.configure(url.hostname, url.port);

    client.connect();

    HTTPRequest req = {
        GET_METHOD,
        url.route,
        HTTP_PROTOCOL,
        url.hostname,
        DEFAULT_CONNECTION_HEADER,
    };

    auto response = client.makeRequest(req);

    std::string savePath = client.getSavePath(url.route, storage);

    client.handleResponse(response, savePath);

    client.close();
  }

  return 0;
}