#include "parser.h"
#include "server.h"

int main(int argc, char **argv)
{
  struct Arguments args;
  parseArguments(argc, argv, &args);

  Server server(args.hostname, args.ip, args.port, args.filesPath);

  server.configure();
  server.listen();

  while (true)
  {
    int connection = server.acceptConnection();

    std::thread connectionThread(Server::handleConnection, connection, server.filesPath);

    connectionThread.detach();
  }

  // Close the connections
  server.close();

  return 0;
}