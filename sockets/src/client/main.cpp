#include "./client.h"

int main(int argc, char **argv)
{
  char hostname[] = "localhost";
  int port = 8080;
  char route[] = "/";
  
  Client client;
  
  client.configure(hostname, port);

  client.connect();

  client.request();

  client.close();

  return 0;
}