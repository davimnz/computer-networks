#include "parser.h"

int parseArguments(int argc, char **argv, struct Arguments *args)
{
  if (argc != 4)
  {
    fprintf(stderr, "Usage: server [hostname] [port] [path-to-files]\n");
    return 1;
  }

  args->hostname = argv[1];
  args->port = atoi(argv[2]);
  args->filesPath = argv[3];

  char *ip;
  if (getIpFromHostname(args->hostname, &ip) != 0)
  {
    return 2;
  }

  args->ip = ip;

  return 0;
}