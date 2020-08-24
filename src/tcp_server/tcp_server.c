#include "std_types.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include "tcp_server.h"
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "macros.h"

extern int HttpServer_Handler(void* pArgs);

static int server;

void TcpServer_SignalHandler(int signalNumber)
{
  UNUSED_PARAM(signalNumber);

  fprintf(stdout, "Received SIGTERM\r\n");

  close(server);

  fprintf(stdout, "Socket has been closed\r\n");

  exit(0);
}

static void * TcpServer_Handler(void* pArgs)
{
  fprintf(stdout, "CLIENT CONNECTED\r\n");

  (void)HttpServer_Handler(pArgs);

  free(pArgs);

  return NULL_PTR;
}

int TcpServer_Start(int port, int maxQueue)
{
  TcpServer_IPv4AddressT serverAddr;

  server = socket(AF_INET, SOCK_STREAM, 0);

  if (server < 0)
  {
    fprintf(stderr, "Error while creating TCP socket");
    return -1;
  }

  serverAddr.sin_family      = AF_INET;
  serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serverAddr.sin_port        = htons(port);


  if (signal(SIGTERM, TcpServer_SignalHandler) == SIG_ERR) {
      return -1;
  }
  if (signal(SIGINT, TcpServer_SignalHandler) == SIG_ERR) {
      return -1;
  }

  if(bind(server, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) != 0)
  {
    fprintf(stderr, "Error while binding TCP socket");
    return -1;
  }

  if(listen(server, maxQueue) != 0)
  {
    fprintf(stderr, "Error while listening TCP socket");
    return -1;
  }

  unsigned int socketLen = sizeof(TcpServer_IPv4AddressT);
  
  TcpServer_ThreadArgsT *pArgs;
  
  pthread_t threadId;
  pthread_attr_t threadAttr;

  if (pthread_attr_init(&threadAttr) != 0) {
    fprintf(stderr, "Error while setting thread attribute");
    return -1;
  }

  if (pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_DETACHED) != 0) {
    fprintf(stderr, "Error while setting thread attribute");
    return -1;
  }

  while(1)
  {
    pArgs = (TcpServer_ThreadArgsT*)malloc(sizeof(TcpServer_ThreadArgsT));

    pArgs->connection = accept(server, (struct sockaddr *)&pArgs->clientAddr, &socketLen);

    if (-1 == pArgs->connection)
    {
      fprintf(stderr, "Error occured while accepting new client connection");
      free(pArgs);
      continue;
    }

    
    if(0 != pthread_create(&threadId, &threadAttr, TcpServer_Handler, (void*)pArgs))
    {
      fprintf(stderr, "Error occured while creating new handling thread");
      free(pArgs);
      continue;
    }
  }

  return 0;
}