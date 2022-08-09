#ifndef REQUEST_H
#define REQUEST_H

#include <stdint.h>

struct HTTPRequest {
  uint8_t ip[4];     /* 32 bits */
  uint8_t port[2];   /* 16 bits */
  char *object;
};

#endif
