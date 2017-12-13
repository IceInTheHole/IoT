
#include <stdlib.h>
#include <string.h>
#include "http.h"
#include "interface.h"

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
void *spdIoT_http_headerlist_set(spdIoTHttpHeaderList *headerList,
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
    spdIoTHttpPacket *packet;

    packet = (spdIoTHttpPacket *) calloc(1, sizeof(spdIoTHttpPacket));
    if (NULL != packet) {
        packet->headerList = spdIoT_http_headerlist_new();
        packet->content = spdIoT_string_new();
    }

    return packet;
}

/* *
 * @brief spdIoT_http_packet_delete
 * */
void spdIoT_http_packet_delete(spdIoTHttpPacket *httpPkt)
{
    spdIoT_http_headerlist_delete(httpPkt->headerList);
    spdIoT_string_delete(httpPkt->content)
}

/* *
 * @brief spdIoT_http_packet_clear
 * */
void spdIoT_http_packet_clear(spdIoTHttpPacket *httpPkt)
{
    spdIoT_http_headerlist_clear(httpPkt->headerList);
    spdIoT_string_clear(httpPkt->content);
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
void spdIoT_http_packet_setheaderinteger(spdIoTHttpPacket *httpPkt,
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
void spdIoT_http_packet_setheadersizet(spdIoTHttpPacket *httpPkt,
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
    return spdIoT_http_header_getvalue(httpPkt->headerList, name);
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
void spdIoT_http_packet_post(spdIoTHttpPacket *httpPkt, spdIo)
