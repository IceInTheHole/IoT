#ifndef _SPDIoT_HTTP_H_
#define _SPDIoT_HTTP_H_

#include "../string/sstring.h"
#include "../list/list.h"
#include "../net/uri.h"
#include "../net/socket.h"
#include "../net/interface.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SPDIoT_HTTP_READLINE_BUFSIZE 512
#define SPDIoT_HTTP_SEVERNAME_MAXLEN 64
#define SPDIoT_HTTP_DATE_MAXLEN 128

#define SPDIoT_HTTP_REQUESTLINE_DELIM "\r\n "
#define SPDIoT_HTTP_STATUSLINE_DELIM "\r\n "
#define SPDIoT_HTTP_HEADERLINE_DELIM "\r\n :"

#define SPDIoT_HTTP_CR "\r"
#define SPDIoT_HTTP_LF "\n"
#define SPDIoT_HTTP_CRLF "\r\n"
#define SPDIoT_HTTP_SP " "
#define SPDIoT_HTTP_COLON ":"

#define SPDIoT_HTTP_VER10 "HTTP/1.0"
#define SPDIoT_HTTP_VER11 "HTTP/1.1"

#if !defined(SPDIoT_HTTP_CURL)
#define SPDIoT_HTTP_USERAGENT_DEFAULT "SPDIoT-HTTP/1.0"
#else
#define SPDIoT_HTTP_USERAGENT_DEFAULT "SPDIoT-HTTP(libcurl)/"
#endif

#define SPDIoT_HTTP_DEFAULT_PORT 80
#define SPDIoT_HTTS_DEFAULT_PORT 443

#define SPDIoT_HTTP_STATUS_CONTINUE 100
#define SPDIoT_HTTP_STATUS_OK 200
#define SPDIoT_HTTP_STATUS_PARTIAL_CONTENT 206
#define SPDIoT_HTTP_STATUS_BAD_REQUEST 400
#define SPDIoT_HTTP_STATUS_NOT_FOUND 404
#define SPDIoT_HTTP_STATUS_METHOD_NOT_ALLOWED 405
#define SPDIoT_HTTP_STATUS_PRECONDITION_FAILED 412
#define SPDIoT_HTTP_STATUS_INVALID_RANGE 416
#define SPDIoT_HTTP_STATUS_INTERNAL_SERVER_ERROR 500

#define SPDIoT_HTTP_POST "POST"
#define SPDIoT_HTTP_GET "GET"
#define SPDIoT_HTTP_HEAD "HEAD"

#define SPDIoT_HTTP_HOST "HOST"
#define SPDIoT_HTTP_DATE "Date"
#define SPDIoT_HTTP_CACHE_CONTROL "Cache-Control"
#define SPDIoT_HTTP_NO_CACHE "no-cache"
#define SPDIoT_HTTP_MAX_AGE "max-age"
#define SPDIoT_HTTP_CONNECTION "Connection"
#define SPDIoT_HTTP_CLOSE "close"
#define SPDIoT_HTTP_KEEP_ALIVE "Keep-Alive"
#define SPDIoT_HTTP_CONTENT_TYPE "Content-Type"
#define SPDIoT_HTTP_CONTENT_LENGTH "Content-Length"
#define SPDIoT_HTTP_CONTENT_RANGE "Content-Range"
#define SPDIoT_HTTP_CONTENT_RANGE_BYTES "bytes"
#define SPDIoT_HTTP_LOCATION "Location"
#define SPDIoT_HTTP_SERVER "Server"
#define SPDIoT_HTTP_RANGE "Range"
#define SPDIoT_HTTP_TRANSFER_ENCODING "Transfer-Encoding"
#define SPDIoT_HTTP_CHUNKED "Chunked"
#define SPDIoT_HTTP_USERAGENT "User-Agent"

/**** SOAP Extention ****/
#define SPDIoT_HTTP_SOAP_ACTION "SOAPACTION"
#define SPDIoT_HTTP_SOAP_ACTION_WITH_NS "01-SOAPACTION"
#define SPDIoT_HTTP_SOAP_MAN_VALUE "\"http://schemas.xmlsoap.org/soap/envelope/\"; ns=01"

/**** UPnP Extention ****/
#define SPDIoT_HTTP_MPOST "M-POST"
#define SPDIoT_HTTP_MSEARCH "M-SEARCH"
#define SPDIoT_HTTP_NOTIFY "NOTIFY"
#define SPDIoT_HTTP_SUBSCRIBE "SUBSCRIBE"
#define SPDIoT_HTTP_UNSUBSCRIBE "UNSUBSCRIBE"

#define SPDIoT_HTTP_ST "ST"
#define SPDIoT_HTTP_MX "MX"
#define SPDIoT_HTTP_MAN "MAN"
#define SPDIoT_HTTP_NT "NT"
#define SPDIoT_HTTP_NTS "NTS"
#define SPDIoT_HTTP_USN "USN"
#define SPDIoT_HTTP_EXT "EXT"
#define SPDIoT_HTTP_SID "SID"
#define SPDIoT_HTTP_SEQ "SEQ"
#define SPDIoT_HTTP_CALLBACK "CALLBACK"
#define SPDIoT_HTTP_TIMEOUT "TIMEOUT"

/**** CURL connection timeout: 2 seconds ****/
#define SPDIoT_HTTP_CURL_CONNECTTIMEOUT 2

/**** CURL total timeout: 30 seconds (UPnP DA: whole transfer: 30sec ****/
#define SPDIoT_HTTP_CONN_TIMEOUT 30

/* HTTP server - client thread blocking timeout */
#define SPDIoT_HTTP_SERVER_READ_TIMEOUT 120

/**** HTTP Status code reason phrases ****/
#define SPDIoT_HTTP_REASON_100 "Continue"
#define SPDIoT_HTTP_REASON_101 "Switching Protocols"
#define SPDIoT_HTTP_REASON_200 "OK"
#define SPDIoT_HTTP_REASON_201 "Created"
#define SPDIoT_HTTP_REASON_202 "Accepted"
#define SPDIoT_HTTP_REASON_203 "Non-Authoritative Information"
#define SPDIoT_HTTP_REASON_204 "No Content"
#define SPDIoT_HTTP_REASON_205 "Reset Content"
#define SPDIoT_HTTP_REASON_206 "Partial Content"
#define SPDIoT_HTTP_REASON_300 "Multiple Choices"
#define SPDIoT_HTTP_REASON_301 "Moved Permanently"
#define SPDIoT_HTTP_REASON_302 "Found"
#define SPDIoT_HTTP_REASON_303 "See Other"
#define SPDIoT_HTTP_REASON_304 "Not Modified"
#define SPDIoT_HTTP_REASON_305 "Use Proxy"
#define SPDIoT_HTTP_REASON_307 "Temporary Redirect"
#define SPDIoT_HTTP_REASON_400 "Bad Request"
#define SPDIoT_HTTP_REASON_401 "Unauthorized"
#define SPDIoT_HTTP_REASON_402 "Payment Required"
#define SPDIoT_HTTP_REASON_403 "Forbidden"
#define SPDIoT_HTTP_REASON_404 "Not Found"
#define SPDIoT_HTTP_REASON_405 "Method Not Allowed"
#define SPDIoT_HTTP_REASON_406 "Not Acceptable"
#define SPDIoT_HTTP_REASON_407 "Proxy Authentication Required"
#define SPDIoT_HTTP_REASON_408 "Request Time-out"
#define SPDIoT_HTTP_REASON_409 "Conflict"
#define SPDIoT_HTTP_REASON_410 "Gone"
#define SPDIoT_HTTP_REASON_411 "Length Required"
#define SPDIoT_HTTP_REASON_412 "Precondition Failed"
#define SPDIoT_HTTP_REASON_413 "Request Entity Too Large"
#define SPDIoT_HTTP_REASON_414 "Request-URI Too Large"
#define SPDIoT_HTTP_REASON_415 "Unsupported Media Type"
#define SPDIoT_HTTP_REASON_416 "Requested range not satisfiable"
#define SPDIoT_HTTP_REASON_417 "Expectation Failed"
#define SPDIoT_HTTP_REASON_500 "Internal Server Error"
#define SPDIoT_HTTP_REASON_501 "Not Implemented"
#define SPDIoT_HTTP_REASON_502 "Bad Gateway"
#define SPDIoT_HTTP_REASON_503 "Service Unavailable"
#define SPDIoT_HTTP_REASON_504 "Gateway Time-out"
#define SPDIoT_HTTP_REASON_505 "HTTP Version not supported"

typedef struct _spdIoTHttpHeader {
    spdIoTListHead list;
    spdIoTString *name;
    spdIoTString *value;
}spdIoTHttpHeader, spdIoTHttpHeaderList;

typedef struct _spdIoTHttpPacket {
    spdIoTHttpHeaderList headerList;
    spdIoTString *content;
}spdIoTHttpPacket;

typedef struct _spdIoTHttpResponse {
    spdIoTHttpHeaderList headerList;
    spdIoTString *content;
    spdIoTString *version;
    int statusCode;
    spdIoTString *reasonPhrase;
    void *userData;
    int timeout;
}spdIoTHttpResponse;

typedef struct _spdIoTHttpRequest {
    spdIoTHttpHeaderList *headerList;
    spdIoTString *content;
    spdIoTString *method;
    spdIoTString *uri;
    spdIoTString *version;
    spdIoTString *userAgent;
    spdIoTHttpResponse *httpRes;
    spdIoTNetURI *postURL;
    void *userData;
    int timeout;
}spdIoTHttpRequest;

spdIoTHttpHeader *spdIoT_http_header_new();
void spdIoT_http_header_delete(spdIoTHttpHeader *header);

void spdIoT_http_header_setname(spdIoTHttpHeader *header, const char *name);
const char *spdIoT_http_header_getname(spdIoTHttpHeader *header);
void spdIoT_http_header_setvalue(spdIoTHttpHeader *header, const char *value);
const char *spdIoT_http_header_getvalue(spdIoTHttpHeader *header);


/**********************************
 * Function Header list
 **********************************/
spdIoTHttpHeaderList *spdIoT_http_headerlist_new();
void spdIoT_http_headerlist_delete(spdIoTHttpHeaderList *headerList);

void spdIoT_http_headerlist_clear(spdIoTHttpHeaderList *headerList);
int spdIoT_http_headerlist_size(spdIoTHttpHeaderList *headerList);
void spdIoT_http_headerlist_add(spdIoTHttpHeaderList *headerList, spdIoTHttpHeader *newHeader);
spdIoTHttpHeader *spdIoT_http_headerlist_get(spdIoTHttpHeaderList *headerList, const char *name);
void spdIoT_http_headerlist_set(spdIoTHttpHeaderList *headerList, const char *name, const char *value);
const char *spdIoT_http_headerlist_getvalue(spdIoTHttpHeaderList *headerList, const char *name);

/*********************************
 * Function http packet
 *********************************/
spdIoTHttpPacket *spdIoT_http_packet_new();
void spdIoT_http_packet_delete(spdIoTHttpPacket *httpPkt);
void spdIoT_http_packet_clear(spdIoTHttpPacket *httpPkt);

#define spdIoT_http_packet_getheadersize(httpPkt) spdIoT_http_headerlist_size(httpPkt->headerList)
#define spdIoT_http_packet_getheaders(httpPkt) (httpPkt->headerList)
#define spdIoT_http_packet_getheader(httpPkt, name) spdIoT_http_headerlist_get(httpPkt->headerList, name)
#define spdIoT_http_packet_hasheader(httpPkt, name) (spdIoT_http_headerlist_get(httpPkt->headerList, name) != NULL ? 1 : 0)

#define spdIoT_http_packet_addheader(httpPkt, header) spdIoT_http_headerlist_add(httpPkt->headerList, header)

void spdIoT_http_packet_setheadervalue(spdIoTHttpPacket *httpPkt, const char *name, const char *value);
void spdIoT_http_packet_setheaderinteger(spdIoTHttpPacket *httpPkt, const char *name, int value);
void spdIoT_http_packet_setheaderlong(spdIoTHttpPacket *httpPkt, const char *name, long value);
void spdIoT_http_packet_setheadersizet(spdIoTHttpPacket *httpPkt, const char *name, size_t value);
void spdIoT_http_packet_setheaderssize(spdIoTHttpPacket *httpPkt, const char *name, ssize_t value);

const char *spdIoT_http_packet_getheadervalue(spdIoTHttpPacket *httpPkt, const char *name);
int spdIoT_http_packet_getheaderinteger(spdIoTHttpPacket *httpPkt, const char *name);
long spdIoT_http_packet_getheaderlong(spdIoTHttpPacket *httpPkt, const char *name);
size_t spdIoT_http_packet_getheadersizet(spdIoTHttpPacket *httpPkt, const char *name);
ssize_t spdIoT_http_packet_getheaderssizet(spdIoTHttpPacket *httpPkt, const char *name);

#define spdIoT_http_packet_setcontent(httpPkt, value) spdIoT_string_setvalue(httpPkt->content, value)
#define spdIoT_http_packet_getcontent(httpPkt) spdIoT_string_getvalue(httpPkt->content)

void spdIoT_http_packet_post(spdIoTHttpPacket *httpPkt, spdIoTSocket *sock);
void spdIoT_http_packet_read_headers(spdIoTHttpPacket *httpPkt, spdIoTSocket *sock, char *lineBuf, size_t lineBufSize);

#define spdIoT_http_packet_setcontentlength(httpPkt, value) spdIoT_http_packet_setheaderssizet(httpPkt, SPDIoT_HTTP_CONTENT_LENGTH, value)
#define spdIoT_http_packet_getcontentlength(httpPkt) spdIoT_http_packet_getheaderssizet(httpPkt, SPDIoT_HTTP_CONTENT_LENGTH)

#define spdIoT_http_packet_setconnection(httpPkt, value) spdIoT_http_packet_setheadervalue(httpPkt, SPDIoT_HTTP_CONNECTION, value)
#define spdIoT_http_packet_getconnection(httpPkt) spdIoT_http_packet_getheadervalue(httpPkt, SPDIoT_HTTP_CONNECTION)
#define spdIoT_http_packet_iskeepaliveconnection(httpPkt) (strcmp(spdIoT_http_packet_getconnection(httpPkt), SPDIoT_HTTP_CLOSE) != 0)

#define spdIoT_http_packet_settransferencoding(httpPkt, value) spdIoT_http_packet_setheadervalue(httPkt, SPDIoT_HTTP_TRANSFER_ENCODING, value)
#define spdIoT_http_packet_gettransferencoding(httpPkt) spdIoT_http_packet_getheadervalue(httpPkt, SPDIoT_HTTP_TRANSFER_ENCODING)
#define spdIoT_http_packet_ischunked(httpPkt) (strcmp(spdIoT_http_packet_gettransferencoding(httpPkt), SPDIoT_HTTP_CHUNKED) == 0)

void spdIoT_http_packet_sethost(spdIoTHttpPacket *httpPkt, const char *addr, int port);
#define spdIoT_http_packet_gethost(httpPkt) spdIoT_http_packet_getheaderlong(httpPkt, SPDIoT_HTTP_HOST)

/****************************
 * http request function
 ****************************/
spdIoTHttpRequest *spdIoT_http_request_new();
void spdIoT_http_request_delete(spdIoTHttpRequest *httpReq);
void spdIoT_http_request_clear(spdIoTHttpRequest *httpReq);

#define spdIoT_http_request_setmethod(httpReq, value) spdIoT_string_setvalue(httpReq->method, value)
#define spdIoT_http_request_getmethod(httpReq) spdIoT_string_getvalue(httpReq->method)
#define spdIoT_http_request_setversion(httpReq, value) spdIoT_string_setvalue(httpReq->version, value)
#define spdIoT_http_request_getversion(httpReq) spdIoT_string_getvalue(httpReq->version)
#define spdIoT_http_request_setuseragent(httpReq, value) spdIoT_string_setvalue(httpReq->userAgent, value)
#define spdIoT_http_request_getuseragent(httpReq) spdIoT_string_getvalue(httpReq->userAgent)
#define spdIoT_http_request_addtouseragent(httpReq) spdIoT_string_addvalue(httpReq->userAgent, value)
#define spdIoT_http_request_seturi(httpReq, value) spdIoT_string_setvalue(httpReq->uri, value)
#define spdIoT_http_request_geturi(httpReq) spdIoT_string_getvalue(httpReq->uri)
#define spdIoT_http_request_setsocket(httpReq, sock) (httpReq->sock = sock)
#define spdIoT_http_request_getsocket(httpReq) (httpReq->sock)
#ifdef __cplusplus
}
#endif
#endif
