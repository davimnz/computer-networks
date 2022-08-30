#include "parser.h"
#include "server.h"

int main(int argc, char **argv)
{
  struct Arguments args;
  parseArguments(argc, argv, &args);

  Server server(args.ip, args.port, args.filesPath);

  server.Configure();
  server.Listen();

  while (true)
  {
    int connection = server.AcceptConnection();

    std::thread connection_thread(Server::HandleConnection, connection);

    connection_thread.detach();
  }

  // Close the connections
  server.Close();

  return 0;
}