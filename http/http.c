
#include <stdlib.h>
#include <string.h>
#include "http.h"


/* *
 * @brief spdIoT_http_header_new
 * */
spdIoTHttpHeader *spdIoT_http_header_new()
{
    spdIoTHttpHeader *header;

    header = (spdIoTHttpHeader *) calloc(1, sizeof(spdIoTHttpHeader));
    if (NULL != header) {
        spdIoT_list_init(&header->list);
        header->name = spdIoT_string_new();
        header->value = spdIoT_string_new();
    }

    return header;
}

/* *
 * @brief spdIoT_http_header_delete
 * */
void spdIoT_http_header_delete(spdIoTHttpHeader *header)
{
    if (NULL != header) {
        spdIoT_list_del(&header->list);
        spdIoT_string_delete(header->name);
        spdIoT_string_delete(header->value);
        free(header);
    }
}

/* *
 * @brief spdIoT_http_header_setname
 * */
void spdIoT_http_header_setname(spdIoTHttpHeader *header, const char *name)
{
    spdIoT_string_setvalue(header->name, name);
}

/* *
 * @brief spdIoT_http_header_getname
 * */
const char *spdIoT_http_header_getname(spdIoTHttpHeader *header)
{
    return spdIoT_string_getvalue(header->name);
}

/* *
 * @brief spdIoT_http_header_setvalue
 * */
void spdIoT_http_header_setvalue(spdIoTHttpHeader *header, const char *value)
{
    spdIoT_string_setvalue(header->value, value);
}

/* *
 * @brief spdIoT_http_header_getvalue
 * */
const char *spdIoT_http_header_getvalue(spdIoTHttpHeader *header)
{
    return spdIoT_string_getvalue(header->value);
}

/********************************************
 * spdIoT http header list function
 ********************************************/
/* *
 * @brief spdIoT_http_headerlist_new
 * */
spdIoTHttpHeaderList *spdIoT_http_headerlist_new()
{
    spdIoTHttpHeaderList *headerList;

    headerList = (spdIoTHttpHeaderList *) calloc(1, sizeof(spdIoTHttpHeaderList));
    if (NULL != headerList) {
        spdIoT_list_init(&headerList->list);
        headerList->name = NULL;
        headerList->value = NULL;
    }

    return headerList;
}

/* *
 * @brief spdIoT_http_headerlist_delete
 * */
void spdIoT_http_headerlist_delete(spdIoTHttpHeaderList *headerList)
{
    spdIoT_http_headerlist_clear(headerList);
    free(headerList);
}

/* *
 * @brief spdIoT_http_headerlist_clear
 * */
void spdIoT_http_headerlist_clear(spdIoTHttpHeaderList *headerList)
{
    spdIoTHttpHeader *pos, *next;

    spdIoT_list_for_each_entry_safe(pos, next, &headerList->list, list) {
        spdIoT_http_header_delete(pos);
        pos = NULL;
    }
}

/* *
 * @brief spdIoT_http_headerlist_size
 * */
int spdIoT_http_headerlist_size(spdIoTHttpHeaderList *headerList)
{
    spdIoTListHead *pos;
    int cnt = 0;

    spdIoT_list_for_each(pos, &headerList->list) {
        cnt++;
    }
}

/* *
 * @brief spdIoT_http_headerlist_add
 * */
void spdIoT_http_headerlist_add(spdIoTHttpHeaderList *headerList, spdIoTHttpHeader *newHeader)
{
    spdIoT_list_add_tail(&newHeader->list, &headerList->list);
}

/* *
 * @brief spdIoT_http_headerlist_get
 * */
spdIoTHttpHeader *spdIoT_http_headerlist_get(spdIoTHttpHeaderList *headerList, const char *name)
{
    spdIoTHttpHeader *pos;

    spdIoT_list_for_each_entry(pos, &headerList->list, list) {
        if (strcmp(name, spdIoT_http_header_getname(pos)) == 0) {
            return pos;
        }
    }

    return NULL;
}

/* *
 * @brief spdIoT_http_headerlist_set
 * */
void spdIoT_http_headerlist_set(spdIoTHttpHeaderList *headerList,
                                 const char *name, const char *value)
{
    spdIoTHttpHeader *header;

    header = spdIoT_http_headerlist_get(headerList, name);
    if (NULL == header) {
        header = spdIoT_http_header_new();
        spdIoT_http_headerlist_add(headerList, header);
        spdIoT_http_header_setname(header, name);
    }

    spdIoT_http_header_setvalue(header, value);
}

/* *
 * @brief spdIoT_http_headerlist_getvalue
 * */
const char *spdIoT_http_headerlist_getvalue(spdIoTHttpHeaderList *headerList, const char *name)
{
    spdIoTHttpHeader *header;

    header = spdIoT_http_headerlist_get(headerList, name);
    if (NULL != header) {
        return spdIoT_http_header_getvalue(header);
    }

    return NULL;
}

/*******************************************
 * spdIoT http packet function
 *******************************************/
/* *
 * @brief spdIoT_http_packet_new
 * */
spdIoTHttpPacket *spdIoT_http_packet_new()
{
    spdIoTHttpPacket *httpPkt;

    httpPkt = (spdIoTHttpPacket *) calloc(1, sizeof(spdIoTHttpPacket));
    if (NULL != httpPkt) {
        spdIoT_http_packet_init(httpPkt);
    }

    return httpPkt;
}

/* *
 * @brief spdIoT_http_packet_delete
 * */
void spdIoT_http_packet_delete(spdIoTHttpPacket *httpPkt)
{
    spdIoT_http_packet_clean(httpPkt);
    free(httpPkt);
}

/* *
 * @brief spdIoT_http_packet_clear
 * */
void spdIoT_http_packet_clear(spdIoTHttpPacket *httpPkt)
{
    spdIoT_http_headerlist_clear(httpPkt->headerList);
    spdIoT_string_clear(httpPkt->content);
}

void spdIoT_http_packet_init(spdIoTHttpPacket *httpPkt)
{
    httpPkt->headerList = spdIoT_http_headerlist_new();
    httpPkt->content = spdIoT_string_new();
}

void spdIoT_http_packet_clean(spdIoTHttpPacket *httpPkt)
{
    spdIoT_http_headerlist_delete(httpPkt->headerList);
    spdIoT_string_setvalue(httpPkt->content, NULL);
}

/* *
 * @brief spdIoT_http_packet_setheadervalue
 * */
void spdIoT_http_packet_setheadervalue(spdIoTHttpPacket *httpPkt,
                                       const char *name, const char *value)
{
    spdIoT_http_headerlist_set(httpPkt->headerList, name, value);
}

/* *
 * @brief spdIoT_http_setheaderinteger
 * */
void spdIoT_http_packet_setheaderinteger(spdIoTHttpPacket *httpPkt,
                                         const char *name, int value)
{
    char str[SPDIoT_STRING_INTEGER_BUFFLEN];

    spdIoT_http_packet_setheadervalue(
                httpPkt->headerList, name, spdIoT_int2str(value, str, sizeof(str)));
}

/* *
 * @brief spdIoT_http_setheaderlong
 * */
void spdIoT_http_packet_setheaderlong(spdIoTHttpPacket *httpPkt,
                                         const char *name, long value)
{
    char str[SPDIoT_STRING_LONG_BUFFLEN];

    spdIoT_http_packet_setheadervalue(
                httpPkt->headerList, name, spdIoT_long2str(value, str, sizeof(str)));
}

/* *
 * @brief spdIoT_http_setheadersizet
 * */
void spdIoT_http_packet_setheadersizet(spdIoTHttpPacket *httpPkt,
                                       const char *name, size_t value)
{
    char str[SPDIoT_STRING_LONG_BUFFLEN];

    spdIoT_http_packet_setheadervalue(
                httpPkt->headerList, name, spdIoT_sizet2str(value, str, sizeof(str)));
}

/* *
 * @brief spdIoT_http_setheaderssizet
 * */
void spdIoT_http_packet_setheaderssizet(spdIoTHttpPacket *httpPkt,
                                       const char *name, ssize_t value)
{
    char str[SPDIoT_STRING_LONG_BUFFLEN];

    spdIoT_http_packet_setheadervalue(
                httpPkt->headerList, name, spdIoT_ssizet2str(value, str, sizeof(str)));
}

/* *
 * @brief spdIoT_http_packet_getheadervalue
 * */
const char *spdIoT_http_packet_getheadervalue(spdIoTHttpPacket *httpPkt, const char *name)
{
    return spdIoT_http_header_getvalue(spdIoT_http_packet_getheader(httpPkt, name));
}

/* *
 * @brief spdIoT_http_packet_getheaderinteger
 * */
int spdIoT_http_packet_getheaderinteger(spdIoTHttpPacket *httpPkt, const char *name)
{
    const char *value;

    value = spdIoT_http_packet_getheadervalue(httpPkt, name);

    return value != NULL ? atoi(value) : 0;
}

/* *
 * @brief spdIoT_http_packet_getheaderlong
 * */
long spdIoT_http_packet_getheaderlong(spdIoTHttpPacket *httpPkt, const char *name)
{
    const char *value;

    value = spdIoT_http_packet_getheadervalue(httpPkt, name);

    return value != NULL ? atol(value) : 0;
}

/* *
 * @brief spdIoT_http_packet_getheadersizet
 * */
size_t spdIoT_http_packet_getheadersizet(spdIoTHttpPacket *httpPkt, const char *name)
{
    const char *value;

    value = spdIoT_http_packet_getheadervalue(httpPkt, name);

    return value != NULL ? atol(value) : 0;
}

/* *
 * @brief spdIoT_http_packet_getheaderssizet
 * */
ssize_t spdIoT_http_packet_getheaderssizet(spdIoTHttpPacket *httpPkt, const char *name)
{
    const char *value;

    value = spdIoT_http_packet_getheadervalue(httpPkt, name);

    return value != NULL ? atol(value) : 0;
}

/* *
 * @brief spdIoT_http_packet_sethost
 * */
void spdIoT_http_packet_sethost(spdIoTHttpPacket *httpPkt, const char *addr, int port)
{
    char *host;
    size_t hostMaxLen;

    if (NULL == addr) {
        return ;
    }

    hostMaxLen = strlen(addr) + SPDIoT_NET_IPV6_ADDRSTRING_MAXSIZE + SPDIoT_STRING_INTEGER_BUFFLEN;
    host = (char *) calloc(1, sizeof(char) * hostMaxLen);

    if (NULL == host) {
        return ;
    }

    if (0 < port && port != SPDIoT_HTTP_DEFAULT_PORT) {
        if (spdIoT_net_isipv6addr(addr)) {
            sprintf(host, "[%s]:%d", addr, port);
        } else {
            sprintf(host, "%s:%d", addr, port);
        }
    } else {
        if (spdIoT_net_isipv6addr(addr)) {
            sprintf(host, "[%s]", addr);
        } else {
            sprintf(host, "%s", addr);
        }
    }

    spdIoT_http_packet_setheadervalue(httpPkt, SPDIoT_HTTP_HOST, host);
    free(host);
}

/* *
 * @brief spdIoT_http_packet_post
 * */
void spdIoT_http_packet_post(spdIoTHttpPacket *httpPkt, spdIoTSocket *sock)
{
    spdIoTHttpHeader *header;
    const char *name, *value;
    char *content;
    size_t contentLen;

    /*** send header ***/
    spdIoT_list_for_each_entry(header, &httpPkt->headerList, list) {
        name = spdIoT_http_header_getname(header);
        if (NULL == name) {
            continue;
        }
        spdIoT_socket_write(sock, name, strlen(name));
        spdIoT_socket_write(sock, SPDIoT_HTTP_COLON, sizeof(SPDIoT_HTTP_COLON) - 1);
        spdIoT_socket_write(sock, SPDIoT_HTTP_SP, sizeof(SPDIoT_HTTP_SP) - 1);

        value = spdIoT_http_header_getvalue(header);
        if (NULL != value) {
            spdIoT_socket_write(sock, value, strlen(value));
        }
        spdIoT_socket_write(sock, SPDIoT_HTTP_CRLF, sizeof(SPDIoT_HTTP_CRLF) - 1);
    }
    spdIoT_socket_write(sock, SPDIoT_HTTP_CRLF, sizeof(SPDIoT_HTTP_CRLF) - 1);

    /*** send content ***/
    content = spdIoT_http_packet_getcontent(httpPkt);
    contentLen = spdIoT_http_packet_getcontentlength(httpPkt);
    if (NULL != content && 0 < contentLen) {
        spdIoT_socket_write(sock, content, contentLen);
    }
}

/* *
 * @brief spdIoT_http_packet_read_headers
 * */
void spdIoT_http_packet_read_headers(spdIoTHttpPacket *httpPkt,
                                     spdIoTSocket *sock, char *lineBuf, size_t lineBufSize)
{
    spdIoTStringTokenizer *strToken;
    spdIoTHttpHeader *header;
    ssize_t readLen;
    char *name, *value;

    while (1) {
        readLen = spdIoT_socket_readline(sock, lineBuf, lineBufSize);
        if (readLen <= 2) {
            break;
        }
        name = NULL;
        value = NULL;
        strToken = spdIoT_string_tokenizer_new(lineBuf, SPDIoT_HTTP_HEADERLINE_DELIM);
        if (spdIoT_string_tokenizer_hasmoretoken(strToken)) {
            name = spdIoT_string_tokenizer_nexttoken(strToken);
        }
        if (spdIoT_string_tokenizer_hasmoretoken(strToken)) {
            value = spdIoT_string_tokenizer_nexttoken(strToken);
        }

        if (0 < strlen(name)) {
            if (strlen(value) == 0) {
                value = "";
            }
            header = spdIoT_http_header_new();
            spdIoT_http_header_setname(header, name);
            spdIoT_http_header_setvalue(header, value);
            spdIoT_http_packet_addheader(httpPkt, header);
        }

        spdIoT_string_tokenizer_delete(strToken);
    }
}

/* *
 * @brief spdIoT_http_packet_read_chunk
 * */
size_t spdIoT_http_packet_read_chunk(spdIoTHttpPacket *httpPkt,
                                     spdIoTSocket *sock, char *lineBuf, size_t lineBufSize)
{
    ssize_t readLen = 0;
    ssize_t conLen = 0;
    int tries = 0;
    char *content = NULL;

    readLen = spdIoT_socket_readline(sock, lineBuf, lineBufSize);

    conLen = spdIoT_strhex2long(lineBuf);
    if (conLen < 1) {
        return 0;
    }

    content = (char *) calloc(1, conLen + 1);
    if (NULL == content) {
        return 0;
    }

    contentj[conLen] = '\0';

    readLen = 0;
    while (readLen < conLen && tries < 20) {
        readLen += spdIoT_socket_read(sock, (content + readLen), (conLen - readLen));
        tries++;
    }

    spdIoT_http_packet_appendncontent(httpPkt, content, readLen);
    free(content);
    content = NULL;

    if (readLen == conLen) {
        spdIoT_socket_readline(sock, lineBuf, lineBufSize);
    }

    return readLen;
}

/* *
 * @brief spdIoT_http_packet_read_body
 * */
int spdIoT_http_packet_read_body(spdIoTHttpPacket *httpPkt,
                                 spdIoTSocket *sock, char *lineBuf, size_t lineBufSize)
{
    ssize_t readLen;
    ssize_t conLen;
    char *content;
    char readBuf[1024 + 1];
    int tries = 0;

    conLen = spdIoT_http_packet_getcontentlength(httpPkt);
    content = NULL;
    if (0 < conLen) {
        content = (char *) calloc(1, conLen + 1);
        if (NULL == content) {
            return -1;
        }
        readLen = 0;

        while (readLen < conLen && tries < 20) {
            readLen += spdIoT_socket_read(sock, (content + readLen),  (conLen - readLen));
            if (readLen <= 0) {
                tries++;
            }
        }

        if (readLen <= 0) {
            return 0;
        }
        content[readLen] = '\0';
        spdIoT_http_packet_setcontent(httpPkt, content);
        free(content);
    } else if (spdIoT_http_packet_getheadervalue(httpPkt, SPDIoT_HTTP_CONTENT_LENGTH) == NULL) {
        /* check if we read chunked encoding */
        if (spdIoT_http_packet_ischunked(httpPkt)) {
            conLen = 0;
            do {
                readLen = spdIoT_http_packet_read_chunk(httpPkt, sock, lineBuf, lineBufSize);
                conLen += readLen;
            } while (readLen > 0);

            spdIoT_http_packet_setcontentlength(httpPkt, conLen);
        } else {
            readLen = 0;
            conLen = 0;
            while ((readLen = spdIoT_socket_read(sock, readBuf, 1024)) > 0) {
                spdIoT_http_packet_appendncontent(httpPkt, readBuf, readLen);
                conLen += readLen;
            }
            spdIoT_http_packet_setcontentlength(httpPkt, conLen);
        }
    }

    return 0;
}

/* *
 * @brief spdIoT_http_packet_read
 * */
int spdIoT_http_packet_read(spdIoTHttpPacket *httpPkt,
                            spdIoTSocket *sock, int onlyHeader, char *lineBuf, size_t lineBufSize)
{
    spdIoT_http_packet_clear(httpPkt);
    spdIoT_http_packet_read_headers(httpPkt, sock, lineBuf, lineBufSize);

    if (onlyHeader) {
        return 0;
    }

    return spdIoT_http_packet_read_body(httpPkt, sock, lineBuf, lineBufSize);
}

/* *
 * @brief spdIoT_http_packet_copy
 * */
void spdIoT_http_packet_copy(spdIoTHttpPacket *dstHttpPkt, spdIoTHttpPacket *srcHttpPkt)
{
    spdIoTHttpHeader *dstHeader;
    spdIoTHttpHeader *srcHeader;
    spdIoTHttpHeaderList *srcHeaderList;

    spdIoT_http_packet_clear(dstHttpPkt);

    srcHeaderList = spdIoT_http_packet_getheaders(srcHttpPkt);
    spdIoT_list_for_each_entry(srcHeader, &srcHeaderList->list, list) {
        dstHeader = spdIoT_http_header_new();
        spdIoT_http_header_setname(dstHeader, spdIoT_http_header_getname(srcHeader));
        spdIoT_http_header_setvalue(dstHeader, spdIoT_http_header_getvalue(srcHeader));
        spdIoT_http_packet_addheader(dstHttpPkt, dstHeader);
    }

    spdIoT_http_packet_setcontent(dstHttpPkt, spdIoT_http_packet_getcontent(srcHttpPkt));
}

/*****************************
 * http request function
 *****************************/
spdIoTHttpRequest *spdIoT_http_request_new()
{
    spdIoTHttpRequest *httpReq;

    httpReq = (spdIoTHttpRequest *) calloc(1, sizeof(spdIoTHttpRequest));
    if (NULL != httpReq) {
        spdIoT_http_packet_init((spdIoTHttpPacket *)httpReq);
        httpReq->method = spdIoT_string_new();
        httpReq->version = spdIoT_string_new();
        httpReq->uri = spdIoT_string_new();
        httpReq->userAgent = spdIoT_string_new();
        httpReq->httpRes = spdIoT_http_response_new();
        httpReq->postURL = spdIoT_net_url_new();

        spdIoT_http_request_setversion(httpReq, SPDIoT_HTTP_VER11);
        spdIoT_http_request_setuseragent(httpReq, SPDIoT_HTTP_USERAGENT_DEFAULT);

        spdIoT_http_request_setsocket(httpReq, NULL);
        spdIoT_http_request_setuserdata(httpReq, NULL);

        spdIoT_http_request_settimeout(httpReq, SPDIoT_HTTP_CONN_TIMEOUT);
    }

    return httpReq;
}

void spdIoT_http_request_delete(spdIoTHttpRequest *httpReq)
{
    spdIoT_http_packet_clean((spdIoTHttpPacket *)httpReq);
    spdIoT_string_delete(httpReq->method);
    spdIoT_string_delete(httpReq->version);
    spdIoT_string_delete(httpReq->uri);
    spdIoT_string_delete(httpReq->userAgent);
    spdIoT_http_response_delete(httpReq->httpRes);
    spdIoT_net_url_detele(httpReq->postURL);

    free(httpReq);
}

void spdIoT_http_request_clear(spdIoTHttpRequest *httpReq)
{
    spdIoT_http_packet_clear((spdIoTHttpPacket *)httpReq);
    spdIoT_http_request_setmethod(httpReq, NULL);
    spdIoT_http_request_seturi(httpReq, NULL);
    spdIoT_http_request_setversion(httpReq, NULL);
    spdIoT_net_url_clear(httpReq->postURL);
    spdIoT_http_request_setuserdata(httpReq, NULL);
}

spdIoTHttpResponse *spdIoT_http_request_post_main(spdIoTHttpRequest *httpReq,
                                          const char ipaddr, int port, int isSecure)
{
    spdIoTSocket *sock;
    char *method, *uri, *version;
    spdIoTString *firstLine;

    spdIoT_http_response_clear(httpReq->httpRes);

    spdIoT_socket_settimeout(sock, spdIoT_http_request_gettimeout(httpReq));
    if (spdIoT_socket_connect(sock, ipaddr, port) < 0) {
        spdIoT_socket_delete(sock);
        return httpReq->httpRes;
    }

    spdIoT_http_request_sethost(httpReq, ipaddr, port);
    spdIoT_http_packet_setheadervalue((spdIoTHttpPacket *)httpReq,
                                  SPDIoT_HTTP_USERAGENT, spdIoT_http_request_getuseragent(httpReq));
    method = spdIoT_http_request_getmethod(httpReq);
    uri = spdIoT_http_request_geturi(httpReq);
    version = spdIoT_http_request_getversion(httpReq);

    if (NULL == method || NULL == uri || NULL == version) {
        spdIoT_socket_close(sock);
        spdIoT_socket_delete(sock);
        return httpReq->httpRes;
    }

    firstLine = spdIoT_string_new();
    spdIoT_string_addvalue(firstLine, method);
    spdIoT_string_addvalue(firstLine, SPDIoT_HTTP_SP);
    spdIoT_string_addvalue(firstLine, uri);
    spdIoT_string_addvalue(firstLine, SPDIoT_HTTP_SP);
    spdIoT_string_addvalue(firstLine, version);
    spdIoT_string_addvalue(firstLine, SPDIoT_HTTP_CRLF);
    spdIoT_socket_write(sock, spdIoT_string_getvalue(firstLine), spdIoT_string_length(firstLine));
    spdIoT_string_delete(firstLine);

    spdIoT_http_packet_post((spdIoTHttpPacket *)httpReq, sock);

    spdIoT_http_response_read(httpReq->httpRes, sock, spdIoT_http_request_isheadrequest(httpReq));

    spdIoT_socket_close(sock);
    spdIoT_socket_delete(sock);

    return httpReq->httpRes;
}

spdIoTHttpResponse *spdIoT_http_request_post(spdIoTHttpRequest *httpReq, const char *ipaddr, int port)
{
    return spdIoT_http_request_post_main(httpReq, ipaddr, port, 0);
}

#if define(SPDIoT_USE_OPENSSL)
spdIoTHttpResponse * spdIoT_https_request_post(spdIoTHttpRequest *httpReq, const char *ipaddr, int port)
{
    return spdIoT_http_request_post_main(httpReq, ipaddr, port, 1);
}
#endif

int spdIoT_http_request_read(spdIoTHttpRequest *httpReq, spdIoTSocket *sock)
{
    char lineBuf[SPDIoT_HTTP_READLINE_BUFSIZE];
    spdIoTStringTokenizer *strToken;
    ssize_t readLen;
    spdIoTNetURI *uri = NULL;
    int failed = 0;

    spdIoT_http_request_clean(httpReq);

    do {
        readLen = spdIoT_socket_readline(sock, lineBuf, sizeof(lineBuf));
    } while (readLen >= 1 && readLen <= 2);

    if (readLen <= 0) {
        return -1;
    }

    strTok = spdIoT_string_tokenizer_new(lineBuf, SPDIoT_HTTP_STATUSLINE_DELIM);
    if (spdIoT_string_tokenizer_hasmoretoken(strToken)) {
        spdIoT_http_request_setmethod(httpReq, spdIoT_string_tokenizer_nexttoken(strToken));
    } else {
        failed = 1;
    }
    if (spdIoT_string_tokenizer_hasmoretoken(strToken)) {
        spdIoT_http_request_seturi(httpReq, spdIoT_string_tokenizer_nexttoken(strToken));
    } else {
        failed = 1;
    }
    if (spdIoT_string_tokenizer_hasmoretoken(strToken)) {
        spdIoT_http_request_setversion(httpReq, spdIoT_string_tokenizer_nexttoken(strToken));
    } else {
        failed = 1;
    }
    spdIoT_string_tokenizer_delete(strToken);

    if (failed) {
        return -1;
    }

    uri = spdIoT_net_uri_new();
    if (NULL != uri) {
        spdIoT_net_uri_set(uri, spdIoT_http_request_geturi(httpReq));
        if (spdIoT_net_uri_isabsolute(uri) && spdIoT_net_uri_getrequest(urt) != NULL) {
            spdIoT_http_request_seturi(httpReq, spdIoT_net_uri-getrequest(uri));
        }
        spdIoT_net_uri_delete(uri);
        uri = NULL;
    }
    spdIoT_http_packet_clear((spdIoTHttpPacket *)httpReq);
    spdIoT_http_packet_read_headers((spdIoTHttpPacket *)httpReq, sock, lineBuf, sizeof(lineBuf));

    if (spdIoT_http_packet_hasheader((spdIoTHttpPacket *)httpReq, SPDIoT_HTTP_CONTENT_LENGTH) ||
            spdIoT_http_packet_hasheader((spdIoTHttpPacket *)httpReq, SPDIoT_HTTP_TRANSFER_ENCODING)) {
        spdIoT_http_packet_read_body((spdIoTHttpPacket *)httpReq, sock, lineBuf, sizeof(lineBuf));
    }

    return 0;
}

int spdIoT_http_request_postresponse(spdIoTHttpRequest *httpReq, spdIoTHttpResponse *httpRes)
{
    spdIoTSocket *sock;
    char httpDate[SPDIoT_HTTP_DATE_MAXLEN];
    char *version, *reasonPhrase;
    int statusCode;
    char statusCodeBuf[SPDIoT_STRING_INTEGER_BUFFLEN];

    sock = spdIoT_http_request_getsocket(httpReq);

    spdIoT_http_response_setdate(httpRes,
                spdIoT_http_getdate(spdIoT_getcurrentsystemtime(), httpDate, sizeof(httpDate)));
}
