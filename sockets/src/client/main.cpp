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
        "GET",
        url.route,
        "HTTP/1.1",
    };

    auto response = client.makeRequest(req);

    client.handleResponse(response, storage);

    client.close();
  }

  return 0;
}