
#include <stdlib.h>
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
