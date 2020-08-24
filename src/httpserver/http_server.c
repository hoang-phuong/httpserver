#include "std_types.h"
#include "tcp_server.h"
#include "http_server.h"
#include <stdio.h>

int HttpServer_Handler(void* pArgs)
{
  TcpServer_ThreadArgsT *pClient = (TcpServer_ThreadArgsT*)pArgs;
  char                   receiveBuffer[HTTP_SERVER_MAX_REQ_LENGTH];
  unsigned int           readBytes = 0;

  fprintf(stdout, "CLIENT CONNECTED\r\n");

  memset(receiveBuffer, 0, sizeof(receiveBuffer));

  readBytes = read(pClient->connection, receiveBuffer, HTTP_SERVER_MAX_REQ_LENGTH);

  char sendBuffer[10] = "hello12345";
  
  write(pClient->connection, sendBuffer, 10);

  fprintf(stdout, "%s\r\n", receiveBuffer);

  return 0;
}

int HttpServer_Start(int port)
{
  return 0;
}