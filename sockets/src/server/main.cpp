#include "parser.h"
#include "server.h"

int main(int argc, char **argv)
{
  struct Arguments args;
  parseArguments(argc, argv, &args);

  Server server(args.ip, args.port, args.filesPath);

  server.configure();
  server.listen();

  while (true)
  {
    int connection = server.acceptConnection();

    std::thread connection_thread(Server::handleConnection, connection);

    connection_thread.detach();
  }

  // Close the connections
  server.close();

  return 0;
}