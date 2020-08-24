#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

/* ** Defines ****************************************************************/
#define HTTP_DEFAULT_PORT                   80
#define HTTP_SERVER_MAX_REQ_LENGTH      8192

int HttpServer_Start(int port);

int HttpServer_Handler(void* pArgs);

#endif /* #ifndef HTTP_SERVER_H */