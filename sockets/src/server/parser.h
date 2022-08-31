#ifndef SERVER_CONFIG_H
#define SERVER_CONFIG_H

#include <stdlib.h>
#include <stdio.h>
#include <string>

#include "../common/common.h"

struct Arguments
{
  char *hostname;
  int port;
  char *filesPath;
};

struct Arguments parseArguments(int argc, char **argv);

#endif
