#include "server.h"

int main(int argc, char **argv)
{
  auto hostname = argc > 1 ? argv[1] : (char *)"localhost";
  auto port = argc > 2 ? atoi(argv[2]) : 8080;
  auto filesPath = argc > 3 ? argv[3] : (char *)"views";

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