#include "parser.h"
#include "server.h"

int main(int argc, char **argv)
{
  struct Arguments args = parseArguments(argc, argv);

  Server server(args.hostname, args.port, args.filesPath);

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