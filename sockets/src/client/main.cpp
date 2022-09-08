#include <iostream>
#include <fstream>
#include "client.h"

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
  std::string storage = "recv";

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

    std::cout << "Response status: " << response.code << " " << response.status << std::endl;

    if (response.code == 200)
    {
      std::string path = storage + (url.route.compare("/") || url.route.compare("/sleep") ? "/index" : url.route) + ".html";

      std::ofstream file;
      file.open(path);

      file << response.body;

      file.close();
    }

    client.close();
  }

  return 0;
}