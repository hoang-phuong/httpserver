#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

/* ** Defines ****************************************************************/
#define HTTP_SERVER_DEFAULT_PORT            80
#define HTTP_SERVER_MAX_REQ_LENGTH          8192
#define HTTP_SERVER_MAX_URI_LENGTH          2000
#define HTTP_SERVER_MAX_LINE_LENGTH         4096

typedef enum
{
    HTTP_METHOD_GET       = 0,
    HTTP_METHOD_HEAD,
    HTTP_METHOD_POST,
    HTTP_METHOD_PUT,
    HTTP_METHOD_DELETE,
    HTTP_METHOD_CONNECT,
    HTTP_METHOD_OPTIONS,
    HTTP_METHOD_TRACE,
    HTTP_METHOD_PATCH,

    HTTP_METHOD_NUM,
    HTTP_METHOD_UNDEFINED
} HttpMethodT;

typedef struct
{
    char         *pRequestBuffer;
    long          requestLength;
    HttpMethodT   method;
    long          pathOffset;
    long          pathLength;
    long          headersOffset;
    long          headersLength;
    long          versionOffset;
    long          versionLength;
    long          bodyOffset;
    long          bodyLength;
} HttpRequestT;

int HttpServer_Start(int port);

int HttpServer_Handler(void* pArgs);

#endif /* #ifndef HTTP_SERVER_H */