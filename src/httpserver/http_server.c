#include "std_types.h"
#include "tcp_server.h"
#include "http_server.h"
#include <stdio.h>
#include "string.h"
#include <unistd.h>

#define   SPACE_CHAR    ' '
#define   CR_CHAR       '\r'
#define   LF_CHAR       '\n'
#define   CRLF          "\r\n"
#define   COLON_CHAR    ':'

#define   GET_PUT_CHAR_LENGTH       3
#define   POST_HEAD_CHAR_LENGTH     4
#define   TRACE_PATCH_CHAR_LENGTH   5
#define   DELETE_CHAR_LENGTH        6
#define   OPTIONS_CHAR_LENGTH       7

#define   PROTOCOL_VERSION_LENGTH   8

static const char* methodGetString      = "GET";
static const char* methodHeadString     = "HEAD";
static const char* methodPostString     = "POST";
static const char* methodPutString      = "PUT";
static const char* methodDeleteString   = "DELETE";
static const char* methodOptionsString  = "OPTIONS";
static const char* methodConnectString  = "CONNECT";
static const char* methodTraceString    = "TRACE";
static const char* methodPatchString    = "PATCH";

static const char* version10String      = "HTTP/1.0";
static const char* version11String      = "HTTP/1.1";
static const char* version20String      = "HTTP/2.0";

static const char response[] = "HTTP/1.1 200 OK\r\n"
                              "Date: Mon, 27 Jul 2009 12:28:53 GMT\r\n"
                              "Server: Apache/2.2.14 (Win32)\r\n"
                              "Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT\r\n"
                              "Content-Length: 54\r\n"
                              "Content-Type: text/html\r\n"
                              "Connection: Closed\r\n"
                              "\r\n"
                              "<html>\r\n"
                              "<body>\r\n"
                              "<h1>Hello, World!</h1>\r\n"
                              "</body>\r\n"
                              "</html>";

/**
 * @brief Parse the request buffer and fill in request structure
 * 
 * @return      0        success
 * @return      1        malformed request
 * @return      2        protocol version is not supported
 */
static int parseRequestBuffer(HttpRequestT *pRequest);

/**
 * @brief Parse the request buffer and write output to request structure
 * 
 * Retrieve request method, path, protocol version, header and body offset
 * 
 * @param[in]   pBuffer       Pointer to buffer that stores request message
 * @param[out]  pRequest      Pointer to structure that stores info parsed from buffer
 * 
 * @return  1 if error, 0 if ok
 */
static int parseRequestLine(HttpRequestT *pRequest);

static int parseRequestHeaders(HttpRequestT *pRequest);

/**
 * @brief Read line from a buffer with an offset
 * 
 * @param pBuffer           buffer to read from
 * @param offset            offset of buffer to start reading
 * @param lastCharOffset    offset of last character before CRLF
 * 
 * @return  1        Error occured
 * @return  0        Success
 */
static int getEndOfLineOffset(char *pBuffer, long length, long offset, long* lastCharOffset);

/**
 * @brief Get the offset of next SPACE character in the buffer, starting from specified offset
 * 
 * @param[in]   pBuffer     Pointer to buffer
 * @param[in]   offset      Offset inside buffer from which search starts
 * @param[out]  spaceOffset Offset of next space character
 * 
 * @return      1           Error occured
 * @returnb     0           Success
 */
static int getNextSpaceOffset(char* pBuffer, long length, long offset, long* spaceOffset);

static int getNextNonSpaceOffset(char* pBuffer, long length, long offset, long* nonSpaceOffset);

static int getIndexOf(char ch, char* pBuffer, long length, long offset, long* index);

static boolean isVersionLengthValid(long length);

static boolean isPathLengthValid(long length);

static boolean isVersionSupported(HttpRequestT *pRequest);

static boolean isEmptyLine(char* pLine);

/**
 * @brief Function check if given string is equal to an HTTP request method
 * 
 * @param[in]   pString         Buffer to string
 * @param[in]   length          Length of pString buffer
 * @param[in]   expectedMethod  Expected method to be checked with given string
 * 
 * @return      TRUE        Given string is equal to an expected HTTP method
 * @return      FALSE       Given string is not equal to an expected HTTP method
 */
static boolean isMethodEqualToExpected(char* pString, long length, HttpMethodT expectedMethod);

/**
 * @brief Get HTTP Method as enumerator from string form
 * 
 * @param[in]       pString     Buffer to given string
 * @param[in]       length      Length of pString buffer
 * 
 * @return          HTTP Method as enumerator
 */
static HttpMethodT getHttpMethodFromString(char* pString, long length);

static HttpVersionT getHttpVersionFromString(char* pString, long length);

static int parseHeaderLine(char* pBuffer, long length, long offset, long* nextLineOffset);

static boolean isMethodEqualToExpected(char* pString, long length, HttpMethodT expectedMethod)
{
    boolean     result;
    const char* expectedMethodString;

    switch (expectedMethod)
    {
        case HTTP_METHOD_GET:
        {
            expectedMethodString = methodGetString;
            break;
        }
        case HTTP_METHOD_PUT:
        {
            expectedMethodString = methodPutString;
            break;
        }
        case HTTP_METHOD_POST:
        {
            expectedMethodString = methodPostString;
            break;
        }
        case HTTP_METHOD_DELETE:
        {
            expectedMethodString = methodDeleteString;
            break;
        }
        case HTTP_METHOD_CONNECT:
        {
            expectedMethodString = methodConnectString;
            break;
        }
        case HTTP_METHOD_OPTIONS:
        {
            expectedMethodString = methodOptionsString;
            break;
        }
        case HTTP_METHOD_TRACE:
        {
            expectedMethodString = methodTraceString;
            break;
        }
        case HTTP_METHOD_PATCH:
        {
            expectedMethodString = methodPatchString;
            break;
        }
        case HTTP_METHOD_UNDEFINED:
        default:
        {
            expectedMethodString = NULL_PTR;
            break;
        }
    }

    if (NULL_PTR == expectedMethodString){
        result = FALSE;
    }
    else
    {
        int expectedLength = strlen(expectedMethodString);

        if(length == expectedLength)
        {
            int diff = memcmp(expectedMethodString, pString, length);
            result = (0 == diff) ? TRUE : FALSE;
        }
        else
        {
            result = FALSE;
        }
    }

    return result;
}

static boolean isVersionLengthValid(long length)
{
    return (PROTOCOL_VERSION_LENGTH == length) ? TRUE : FALSE;
}

static boolean isPathLengthValid(long length)
{
    return (length <= HTTP_SERVER_MAX_URI_LENGTH) ? TRUE : FALSE;
}

static boolean isVersionSupported(HttpRequestT *pRequest)
{
    return (pRequest->version == HTTP_VERSION_1_1) ? TRUE : FALSE;
}

static boolean isEmptyLine(char* pLine)
{
    if (CR_CHAR == pLine[0] && LF_CHAR == pLine[1])
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static int parseRequestLine(HttpRequestT *pRequest)
{
    int error;
    char* pBuffer = pRequest->pRequestBuffer;
    long  bufferLength = pRequest->requestLength;
    HttpMethodT method;
    HttpVersionT version; 
    long offsetBegin = 0;
    long offsetEnd;
    long spaceOffset;
    long pathOffset;
    long pathLength;
    long versionOffset;
    long versionLength;

    error = getEndOfLineOffset(pBuffer, bufferLength, offsetBegin, offsetEnd);

    if (0 == error)
    {
        error = getNextSpaceOffset(pBuffer, bufferLength, offsetBegin, spaceOffset);
    }

    if (0 == error)
    {
        /* Extract HTTP method */
        long stringLength = spaceOffset - offsetBegin;
        method = getHttpMethodFromString(pBuffer, stringLength);
        if (HTTP_METHOD_UNDEFINED == method)
        {
            error = 1;
        }
    }

    if (0 == error)
    {
        pRequest->method = method;

        /* Extract path */
        pathOffset  = spaceOffset + 1;
        error = getNextSpaceOffset(pBuffer, bufferLength, pathOffset, spaceOffset);
    }

    if (0 == error)
    {
        pathLength = spaceOffset - pathOffset;
        error = isPathLengthValid(pathLength) ? 0 : 1;
    }

    if (0 == error)
    {
        pRequest->pathOffset = pathOffset;
        pRequest->pathLength = spaceOffset - pathOffset;

        /* Extract protocol version */
        versionOffset = spaceOffset + 1;
        versionLength = offsetEnd - versionOffset + 1;
        error = isVersionLengthValid(versionLength) ? 0 : 1;
    }

    if (0 == error)
    {
        char* pVersion = pBuffer[versionOffset];
        version = getHttpVersionFromString(pVersion, versionLength);

        if(HTTP_VERSION_UNDEFINED == version)
        {
            error = 1;
        }
        else
        {
            pRequest->version       = version;
            pRequest->versionOffset = versionOffset;
            pRequest->versionLength = versionLength;
        }

    }

    return error;
}

static int parseHostHeader(char* pBuffer, long length, long offset)
{
    int error;
    long endOfLineOffset;
    long lineLength;

    if(offset >= length )
    {
        error = 1;
    }

    if (0 == error)
    {
        error = getEndOfLineOffset(pBuffer, length, offset, &endOfLineOffset);
    }

    if (0 == error)
    {
        lineLength = endOfLineOffset - offset + 1;
        
    }

    return error;
}

static int parseRequestHeaders(HttpRequestT *pRequest)
{
    int     error;
    char   *pBuffer = pRequest->pRequestBuffer;
    long    bufferLength = pRequest->requestLength;
    long    headerOffset;
    long    headerLength;
    long    nextLineOffset;
    boolean isEmptyLineEncountered = FALSE;
    char* headerHostString = "Host";

    /* Calculate header offset, skip CRLF of request line */
    headerOffset = pRequest->versionOffset + pRequest->versionLength + 2;
    pRequest->headersOffset = headerOffset;

    if(headerOffset >= bufferLength)
    {
        error = 1;
    }

    if (0 == error)
    {
        do
        {
            if( isEmptyLine(pBuffer[headerOffset]) ) break;
            error = parseHeaderLine(pBuffer, bufferLength, headerOffset, &headerOffset);
        } while(0 == error && headerOffset < bufferLength);
    }
    
    return error;
}

static int parseHeaderLine(char* pBuffer, long length, long offset, long* nextLineOffset)
{
    int error;
    long keyEndOffset;
    long keyLength;
    long endLineOffset;
    long keyStartOffset = offset;

    error = getEndOfLineOffset(pBuffer, length, offset, &endLineOffset);

    if (0 == error)
    {
        error = getIndexOf(COLON_CHAR, pBuffer, length, keyStartOffset, &keyEndOffset);
    }

    if (0 == error)
    {
        keyLength = keyEndOffset - keyStartOffset;
    }
}

static int parseRequestBuffer(HttpRequestT *pRequest)
{
    int error;

    error = parseRequestLine(pRequest);

    if (0 == error)
    {
        if(isVersionSupported(pRequest))
        {
            error = parseRequestHeaders(pRequest);
        }
        else
        {
            error = 2;
        }
    }

    return error;
}

static HttpMethodT getHttpMethodFromString(char* pString, long length)
{
    HttpMethodT method = HTTP_METHOD_UNDEFINED;
    
    int httpMethodEnum;

    for (httpMethodEnum = 0; httpMethodEnum < HTTP_METHOD_NUM; httpMethodEnum++)
    {
        if(isMethodEqualToExpected(pString, length, (HttpMethodT)httpMethodEnum))
        {
            method = (HttpMethodT)httpMethodEnum;
            break;
        }
    }
    
    return method;
}

static HttpVersionT getHttpVersionFromString(char* pString, long length)
{
    HttpVersionT version = HTTP_VERSION_UNDEFINED;
    int diff;

    diff = memcmp(version10String, pString, length);
    if(0 == diff)
    {
        version = HTTP_VERSION_1_0;
    }

    if(HTTP_VERSION_UNDEFINED == version)
    {
        diff = memcmp(version11String, pString, length);
        if(0 == diff)
        {
            version = HTTP_VERSION_1_1;
        }
    }

    if(HTTP_VERSION_UNDEFINED == version)
    {
        diff = memcmp(version20String, pString, length);
        if(0 == diff)
        {
            version = HTTP_VERSION_2_0;
        }
    }

    return version;
}

static int getNextNonSpaceOffset(char* pBuffer, long length, long offset, long* nonSpaceOffset)
{
    long idx = 0;
    while(pBuffer[idx + offset] == SPACE_CHAR)
    {
        idx++;
        if((idx >= HTTP_SERVER_MAX_URI_LENGTH) || ((idx + offset) >= length))
        {
            return 1;
        }
    }
    *nonSpaceOffset = idx + offset;

    return 0;
}

static int getIndexOf(char ch, char* pBuffer, long length, long offset, long* index)
{
    long idx = 0;
    while(pBuffer[idx + offset] == ch)
    {
        idx++;
        if((idx >= HTTP_SERVER_MAX_URI_LENGTH) || ((idx + offset) >= length))
        {
            return 1;
        }
    }
    *index = idx + offset;

    return 0;
}

static int getNextSpaceOffset(char* pBuffer, long length, long offset, long* spaceOffset)
{
    long idx = 0;
    while(pBuffer[idx + offset] != SPACE_CHAR)
    {
        idx++;
        if((idx >= HTTP_SERVER_MAX_URI_LENGTH) || ((idx + offset) >= length))
        {
            return 1;
        }
    }
    *spaceOffset = idx + offset;

    return 0;
}

static int getEndOfLineOffset(char *pBuffer, long length, long offset, long* lastCharOffset)
{
    long idx = 0;

    while (pBuffer[idx + offset] != CR_CHAR || pBuffer[idx + offset + 1] != LF_CHAR)
    {
        idx++;
        if((idx > HTTP_SERVER_MAX_LINE_LENGTH) || ((idx + offset) >= length))
        {
            return 1;
        }
    }
    *lastCharOffset = idx + offset - 1;

    return 0;
}

int HttpServer_Handler(void *pArgs)
{
    TcpServer_ThreadArgsT *pClient = (TcpServer_ThreadArgsT *)pArgs;
    char receiveBuffer[HTTP_SERVER_MAX_REQ_LENGTH];
    long readBytes = 0;

    memset(receiveBuffer, 0, sizeof(receiveBuffer));

    readBytes = read(pClient->connection, receiveBuffer, HTTP_SERVER_MAX_REQ_LENGTH);

    char sendBuffer[1024] = "hello12345";
    long index;

    

    //memcpy(sendBuffer, receiveBuffer, index);

    write(pClient->connection, response, sizeof(response));
    close(pClient->connection);
    fprintf(stdout, "%s\r\n%s\r\n", receiveBuffer, response);

    //HttpRequestT request;

    //request.pRequestBuffer = receiveBuffer;
    //request.requestLength = readBytes;

    //parseRequestLine(&request);

    return 0;
}

int HttpServer_Start(int port)
{
    return 0;
}