#include <stdlib.h>
#include <string.h>
#include "uri.h"

/* *
 * @brief spdIoT_net_uri_new
 * */
spdIoTNetURI *spdIoT_net_uri_new()
{
    spdIoTNetURI *uri;

    uri = (spdIoTNetURI *) calloc(1, sizeof(spdIoTNetURI));
    if (NULL != uri) {
        uri->uri = spdIoT_string_new();
        uri->protocol = spdIoT_string_new();
        uri->user = spdIoT_string_new();
        uri->password = spdIoT_string_new();
        uri->host = spdIoT_string_new();
        uri->port = 0;
        uri->path = spdIoT_string_new();
        uri->query = spdIoT_string_new();
        uri->fragment = spdIoT_string_new();
        uri->request = NULL;
        uri->queryDictionary = NULL;

        spdIoT_string_setvalue(uri->path, SPDIoT_NET_URI_DEFAULT_PATH);
    }

    return uri;
}

/* *
 * @brief spdIoT_net_uri_delete
 * */
void spdIoT_net_uri_delete(spdIoTNetURI *uri)
{
    spdIoT_string_delete(uri->uri);
    spdIoT_string_delete(uri->protocol);
    spdIoT_string_delete(uri->user);
    spdIoT_string_delete(uri->password);
    spdIoT_string_delete(uri->host);
    spdIoT_string_delete(uri->path);
    spdIoT_string_delete(uri->query);
    spdIoT_string_delete(uri->fragment);
    if (uri->request != NULL)
        spdIoT_string_delete(uri->request);
    if (uri->queryDictionary != NULL)
        spdIoT_dictionary_delete(uri->queryDictionary);

    free(uri);
}

/* *
 * @brief spdIoT_net_uri_clear
 * */
void spdIoT_net_uri_clear(spdIoTNetURI *uri)
{
    spdIoT_string_clear(uri->uri);
    spdIoT_string_clear(uri->protocol);
    spdIoT_string_clear(uri->user);
    spdIoT_string_clear(uri->password);
    spdIoT_string_clear(uri->host);
    uri->port = 0;
    spdIoT_string_clear(uri->path);
    spdIoT_string_clear(uri->query);
    spdIoT_string_clear(uri->fragment);
    if (uri->request != NULL) {
      spdIoT_string_delete(uri->request);
      uri->request = NULL;
    }
}

/* *
 * @brief spdIoT_net_uri_setvalue
 * */
void spdIoT_net_uri_setvalue(spdIoTNetURI *uri, const char *value)
{
    char *protocol;
    size_t uriLen;
    size_t currIdx;
    ssize_t prototIdx;
    ssize_t atIdx;
    ssize_t colonIdx;
    ssize_t shashIdx;
    char *host;
    ssize_t eblackIdx;
    spdIoTString *hostStr;
    spdIoTString *portStr;
    size_t hostLen;
    ssize_t sharpIdx;
    ssize_t questionIdx;
    size_t queryLen;

    uriLen = strlen(value);
    spdIoT_net_uri_clear(uri);
    spdIoT_net_uri_seturi(uri, value);

    currIdx = 0;

    /*** protocol ***/
    prototIdx = spdIoT_strstr(value, SPDIoT_NET_URI_PROTOCOL_DELIM);
    if (0 < prototIdx) {
        spdIoT_string_setnvalue(uri->protocol, value, prototIdx);
        currIdx += prototIdx + strlen(SPDIoT_NET_URI_PROTOCOL_DELIM);
    }

    /*** user (password) ***/
    atIdx = spdIoT_strstr(value + currIdx, SPDIoT_NET_URI_USER_DELIM);
    if (0 < atIdx) {
        colonIdx = spdIoT_strstr(value + currIdx, SPDIoT_NET_URI_COLON_DELIM);
        if (0 < colonIdx && colonIdx < atIdx) {
            spdIoT_string_setnvalue(uri->user, value + currIdx, colonIdx);
            spdIoT_string_setnvalue(uri->password,
                                    value + currIdx + colonIdx + 1, atIdx - (colonIdx + 1));
        } else {
            spdIoT_string_setnvalue(uri->user, value + currIdx, atIdx - currIdx);
            currIdx += atIdx + 1;
        }
    }

    /*** host (port) ***/
    shashIdx = spdIoT_strstr(value + currIdx, SPDIoT_NET_URI_SLASH_DELIM);
    if (0 < shashIdx) {
        spdIoT_string_setnvalue(uri->host, value + currIdx, shashIdx);
        currIdx += shashIdx;
    } else if (spdIoT_net_uri_isabsolute(uri)) {
        spdIoT_string_setnvalue(uri->host, value + currIdx, strlen(value) - currIdx);
        currIdx += strlen(value) - currIdx;
    }
    host = spdIoT_net_uri_gethost(uri);
    colonIdx = strrchr(host, ':');
    eblackIdx = strrchr(host, ']');
    if (0 < colonIdx && eblackIdx < colonIdx) {
        hostStr = spdIoT_string_new();
        spdIoT_string_setvalue(hostStr, host);
        hostLen = spdIoT_string_length(hostStr);

        /*** host ***/
        spdIoT_string_setnvalue(uri->host, spdIoT_string_getvalue(hostStr), colonIdx);
        host = spdIoT_net_uri_gethost(uri);
        if (0 < hostLen) {
            if (host[0] == '[' && host[hostLen - 1] == ']') {
                spdIoT_string_setnvalue(uri->host,
                                        spdIoT_string_getvalue(hostStr) + 1, colonIdx - 2);
            }
        }
        /*** port ***/
        portStr = spdIoT_string_new();
        spdIoT_string_setnvalue(
                    portStr, spdIoT_string_getvalue(hostStr) + colonIdx + 1, hostLen - colonIdx - 1);
        uri->port = atoi(spdIoT_string_getvalue(portStr));
        spdIoT_string_delete(portStr);
        spdIoT_string_delete(hostStr);
    } else {
        uri->port = SPDIoT_NET_URI_UNKOWN_PORT;
        protocol = spdIoT_net_uri_getprotocol(uri);
        if (strcmp(protocol, SPDIoT_NET_URI_PROTOCOL_HTTP) == 0) {
            uri->port = SPDIoT_NET_URI_DEFAULT_HTTP_PORT;
        } else if (strcmp(protocol, SPDIoT_NET_URI_DEFAULT_FTP_PORT) == 0) {
            uri->port = SPDIoT_NET_URI_DEFAULT_FTP_PORT;
        }
    }

    /* *
     * Handle relative URL
     * */
    if (!spdIoT_net_uri_isabsolute(uri)) {
        spdIoT_string_addvalue(uri->path, value);
    } else {
        /* first set path simply to the rest of URI */
        spdIoT_string_setnvalue(uri->path, value + currIdx, uriLen - currIdx);
    }

    /*** path (Query/Fragment) ***/
    sharpIdx = spdIoT_strstr(value + currIdx, SPDIoT_NET_URI_SHARP_DELIM);
    if (0 <sharpIdx) {
        spdIoT_string_setnvalue(uri->path, value + currIdx, sharpIdx);
        spdIoT_string_setnvalue(
                    uri->fragment, value + currIdx + sharpIdx + 1, uriLen - (currIdx + sharpIdx + 1));
    }
    questionIdx = spdIoT_strstr(value + currIdx, SPDIoT_NET_URI_QUESTION_DELIM);
    if (0 < questionIdx) {
        spdIoT_string_setnvalue(uri->path, value + currIdx, questionIdx);
        queryLen = uriLen - (currIdx + questionIdx + 1);
        if (0 < sharpIdx) {
            queryLen -= uriLen - (currIdx + sharpIdx);
            spdIoT_string_setnvalue(uri->query, value + questionIdx + 1, queryLen);
        }
    }
}

/* *
 * @brief spdIoT_net_uri_rebuild
 * */
void spdIoT_net_uri_rebuild(spdIoTNetURI *uri)
{
    char portStr[32];
    char *path;

    spdIoT_string_setvalue(uri->uri, spdIoT_net_uri_getprotocol(uri));
    spdIoT_string_addvalue(uri->uri, SPDIoT_NET_URI_PROTOCOL_DELIM);
    spdIoT_string_addvalue(uri->uri, spdIoT_net_uri_gethost(uri));
    spdIoT_string_addvalue(uri->uri, SPDIoT_NET_URI_COLON_DELIM);
    spdIoT_string_addvalue(uri->uri,
                           spdIoT_int2str(spdIoT_net_uri_getport(uri), portStr, sizeof(portStr)));
    if (0 < strlen(spdIoT_net_uri_getpath(uri))) {
        path = spdIoT_net_uri_getpath(uri);
        if (path[0] != '/') {
            spdIoT_string_addvalue(uri->uri, SPDIoT_NET_URI_SLASH_DELIM);
        }
        spdIoT_string_addvalue(uri->uri, spdIoT_net_uri_getpath(uri));
        if (spdIoT_strchr(spdIoT_net_uri_getpath(uri), "?", 1) == -1 &&
                0 < strlen(spdIoT_net_uri_getquery(uri))) {
            spdIoT_string_addvalue(uri->uri, SPDIoT_NET_URI_QUESTION_DELIM);
            spdIoT_string_addvalue(uri->uri, spdIoT_net_uri_getquery(uri));
        }
    }
}

/* *
 * @brief spdIoT_net_uri_getvalue
 * */
const char *spdIoT_net_uri_getvalue(spdIoTNetURI *uri)
{
    spdIoT_net_uri_rebuild(uri);
    return spdIoT_string_getvalue(uri->uri);
}

/* *
 * @brief spdIoT_net_uri_getrequest
 * */
char *spdIoT_net_uri_getrequest(spdIoTNetURI *uri)
{
    if (!spdIoT_net_uri_hasquery(uri)) {
        return spdIoT_net_uri_getpath(uri);
    }

    if (uri->request == NULL) {
        uri->request = spdIoT_string_new();
    }

    spdIoT_string_setvalue(uri->request, spdIoT_net_uri_getpath(uri));
    spdIoT_string_addvalue(uri->request, SPDIoT_NET_URI_QUESTION_DELIM);
    spdIoT_string_addvalue(uri->request, spdIoT_net_uri_getquery(uri));

    return spdIoT_string_getvalue(uri->request);
}


/* *
 * @brief spdIoT_net_uri_getquerydictionary
 * */
spdIoTDictionary *spdIoT_net_uri_getquerydictionary(spdIoTNetURI *uri)
{
    char *query;
    size_t queryOffset;
    ssize_t eqIdx, ampIdx;
    spdIoTString *paramName;
    spdIoTString *paramValue;

    if (NULL == uri->queryDictionary) {
        uri->queryDictionary = spdIoT_dictionary_new();
    }

    paramName = spdIoT_string_new();
    paramValue = spdIoT_string_new();

    query = spdIoT_net_uri_getquery(uri);
    queryOffset = 0;

    eqIdx = spdIoT_strstr(query, "=");
    while (0 < eqIdx) {
        ampIdx = spdIoT_strstr(query + queryOffset, "&");
        if (ampIdx <= 0) {
            ampIdx = spdIoT_strstr(query + queryOffset, "#");
            if (ampIdx <= 0) {
                ampIdx = strlen(query + queryOffset);
            }
        }
        if (ampIdx <= eqIdx) {
            break;
        }

        spdIoT_string_setnvalue(paramName, query + queryOffset, eqIdx);
        spdIoT_string_setnvalue(paramValue, query + queryOffset + eqIdx + 1, (ampIdx - eqIdx - 1));
        spdIoT_dictionary_setvalue(uri->queryDictionary, spdIoT_string_getvalue(paramName),
                                   spdIoT_string_getvalue(paramValue));
        queryOffset += ampIdx + 1;
        eqIdx = spdIoT_strstr(query + queryOffset, "=");
    }

    spdIoT_string_delete(paramName);
    spdIoT_string_delete(paramValue);

    return uri->queryDictionary;
}
