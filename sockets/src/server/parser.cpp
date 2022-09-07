#include "parser.h"

struct Arguments parseArguments(int argc, char **argv)
{
  struct Arguments args;

  args.hostname = argc > 1 ? argv[1] : (char *)"localhost";
  args.port = argc > 2 ? atoi(argv[2]) : 8080;
  args.filesPath = argc > 3 ? argv[3] : (char *)"views";

  return args;
}