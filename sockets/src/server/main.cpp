#include "server.h"

#define DEFAULT_ASSETS_FOLDER "views"

int main(int argc, char **argv)
{
  auto hostname = argc > 1 ? argv[1] : (char *)DEFAULT_HOSTNAME;
  auto port = argc > 2 ? atoi(argv[2]) : DEFAULT_PORT;
  auto filesPath = argc > 3 ? argv[3] : (char *)DEFAULT_ASSETS_FOLDER;

  Server server(hostname, port, filesPath);

  server.configure();
  server.listen();

  while (true)
  {
    int connection = server.acceptConnection();

    std::thread connectionThread(Server::handleConnection, connection, server);

    connectionThread.detach();
  }

  server.close();

  return 0;
}