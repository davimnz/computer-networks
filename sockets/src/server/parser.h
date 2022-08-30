#ifndef SERVER_CONFIG_H
#define SERVER_CONFIG_H

#include <stdlib.h>
#include <stdio.h>

#include "../common/common.h"

struct Arguments
{
  char *hostname;
  int port;
  char *filesPath;
  char *ip;
};

int parseArguments(int argc, char **argv, struct Arguments *args);

#endif
