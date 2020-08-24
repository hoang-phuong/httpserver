#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <netinet/in.h>

typedef struct sockaddr    TcpServer_GenericAddressT;
typedef struct sockaddr_in TcpServer_IPv4AddressT;

typedef struct
{
  int                    connection;
  TcpServer_IPv4AddressT clientAddr;
  
} TcpServer_ThreadArgsT;

int TcpServer_Start(int port, int maxQueue);

#endif /* #ifndef TCP_SERVER_H */