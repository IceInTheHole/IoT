#ifndef _SPDIoT_URI_H_
#define _SPDIoT_URI_H_

#include "../string/sstring.h"
#include "../dictionary/dictionary.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SPDIoT_NET_URI_UNKOWN_PORT (-1)
#define SPDIoT_NET_URI_DEFAULT_HTTP_PORT 80
#define SPDIoT_NET_URI_DEFAULT_FTP_PORT 21
#define SPDIoT_NET_URI_DEFAULT_PATH "/"
#define SPDIoT_NET_URI_MAXLEN 256

#define SPDIoT_NET_URI_PROTOCOL_DELIM "://"
#define SPDIoT_NET_URI_USER_DELIM "@"
#define SPDIoT_NET_URI_COLON_DELIM ":"
#define SPDIoT_NET_URI_SLASH_DELIM "/"
#define SPDIoT_NET_URI_SBLACET_DELIM "["
#define SPDIoT_NET_URI_EBLACET_DELIM "]"
#define SPDIoT_NET_URI_SHARP_DELIM "#"
#define SPDIoT_NET_URI_QUESTION_DELIM "?"
#define SPDIoT_NET_URI_ESCAPING_CHAR "%"
#define SPDIoT_NET_URI_AMP_DELIM "&"
#define SPDIoT_NET_URI_EQ_DELIM "="

#define SPDIoT_NET_URI_PROTOCOL_HTTP "http"
#define SPDIoT_NET_URI_PROTOCOL_FTP "ftp"

typedef struct _spdIoTNetURI {
    spdIoTString* uri;
    spdIoTString* protocol;
    spdIoTString* user;
    spdIoTString* password;
    spdIoTString* host;
    int port;
    spdIoTString* path;
    spdIoTString* query;
    spdIoTString* fragment;
    spdIoTString* request;
    spdIoTDictionary* queryDictionary;
}spdIoTNetURI;

spdIoTNetURI *spdIoT_net_uri_new();
void spdIoT_net_uri_delete(spdIoTNetURI *uri);
void spdIoT_net_uri_clear(spdIoTNetURI *uri);
void spdIoT_net_uri_setvalue(spdIoTNetURI *uri, const char *value);
void spdIoT_net_uri_rebuild(spdIoTNetURI *uri);
const char *spdIoT_net_uri_getvalue(spdIoTNetURI *uri);

#define spdIoT_net_uri_set(urip, value) spdIoT_net_uri_setvalue(urip, value)

#define spdIoT_net_uri_seturi(urip, value) spdIoT_string_setvalue(urip->uri, value)
#define spdIoT_net_uri_setprotocol(urip, vavlue) spdIoT_string_setvalue(urip->protocol, value)
#define spdIoT_net_uri_setuser(urip, value) spdIoT_string_setvalue(urip->user, value)
#define spdIoT_net_uri_setpassword(urip, value) spdIoT_string_setvalue(urip->password, value)
#define spdIoT_net_uri_setport(urip, value) (urip->port = value)
#define spdIoT_net_uri_setpath(urip, value) spdIoT_string_setvalue(urip->path, value)
#define spdIoT_net_uri_addpath(urip, value) spdIoT_string_addvalue(urip->path, value)
#define spdIoT_net_uri_setquery(urip, value) spdIoT_string_setvalue(urip->query, value)
#define spdIoT_net_uri_setfragment(urip, value) spdIoT_string_setvalue(urip->fragment, value)

#define spdIoT_net_uri_geturi(urip) spdIoT_string_getvalue(urip->uri)
#define spdIoT_net_uri_getprotocol(urip) spdIoT_string_getvalue(urip->protocol)
#define sdpIoT_net_uri_getuser(urip) spdIoT_stirng_getvalue(urip->user)
#define sdpIoT_net_uri_getpassword(urip) spdIoT_string_getvalue(urip->password)
#define spdIoT_net_uri_gethost(urip) spdIoT_string_getvalue(urip->host)
#define spdIoT_net_uri_getport(uirp) (urip->port)
#define spdIoT_net_uri_getpath(urip) spdIoT_string_getvalue(urip->path)
#define spdIoT_net_uri_getquery(urip) spdIoT_string_getvalue(urip->query)
#define spdIoT_net_uri_getfragment(urip) spdIoT_string_getvalue(urip->fragment)

char *spdIoT_net_uri_getrequest(spdIoTNetURI *uri);

#define spdIoT_net_uri_hasuri(urip) ((0 < spdIoT_string_length(urip->uri)) ? 1 : 0)
#define spdIoT_net_uri_hasprotocol(urip) ((0 < spdIoT_string_length(urip->protocol)) ? 1 : 0)
#define spdIoT_net_uri_hasuser(urip) ((0 < spdIoT_string_length(urip->user)) ? 1 : 0)
#define spdIoT_net_uri_haspassword(urip) ((0 < spdIoT_string_length(urip->password)) ? 1 : 0)
#define spdIoT_net_uri_hashost(urip) ((0 < spdIoT_string_length(urip->host)) ? 1 : 0)
#define spdIoT_net_uri_hasport(urip) ((0 < urip->port) ? 1 : 0)
#define spdIoT_net_uri_haspath(urip) ((0 < spdIoT_string_length(urip->path)) ? 1 : 0)
#define spdIoT_net_uri_hasquery(urip) ((0 < spdIoT_string_length(urip->query)) ? 1 : 0)
#define spdIoT_net_uri_hasfragment(urip) ((0 < spdIoT_string_length(urip->fragment)) ? 1 ; 0)

#define spdIoT_net_uri_ishttpprotocol(urip) (strcmp(spdIoT_string_getvalue(urip->protocol), SPDIoT_NET_URI_PROTOCOL_HTTP) == 0 ? 1 : 0)
#define spdIoT_net_uri_isabsolute(urip) spdIoT_net_uri_hasprotocol(urip)
#define spdIoT_net_uri_isabsolutepath(urippath) ((*urippath == '/') ? 1 : 0)
#define spdIoT_net_uri_isrelative(urip) ((spdIoT_net_uri_hasprotocol(uri) == 1) ? 0 : 1)

spdIoTDictionary *spdIoT_net_uri_getquerydictionary(spdIoTNetURI *uri);
#ifdef __cplusplus
}
#endif
#endif // _SPDIoT_URI_H_
