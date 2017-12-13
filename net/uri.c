#include <stdlib.h>
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
