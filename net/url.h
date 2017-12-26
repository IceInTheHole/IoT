#ifndef _SPDIoT_URL_H_
#define _SPDIoT_URL_H_

#include "uri.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _spdIoTNetURI spdIoTNetURL;


#define spdIoT_net_url_new() spdIoT_net_uri_new()
#define spdIoT_net_url_delete(urip) spdIoT_net_uri_delete(urip);
#define spdIoT_net_url_clear(urip) spdIoT_net_uri_clear(urip);

#define spdIoT_net_url_set(urip, value) spdIoT_net_uri_set(urip, value)
#define spdIoT_net_url_setvalue(urip, value) spdIoT_net_uri_setvalue(urip, value)
#define spdIoT_net_url_rebuild(urip) spdIoT_net_uri_rebuild(urip)
#define spdIoT_net_url_getvalue(urip) spdIoT_net_uri_getvalue(urip)

#define spdIoT_net_url_seturi(urip, value) spdIoT_string_setvalue(urip->uri, value)
#define spdIoT_net_url_setprotocol(urip, value) spdIoT_string_setvalue(urip->protocol, value)
#define spdIoT_net_url_setuser(urip, value) spdIoT_string_setvalue(urip->user, value)
#define spdIoT_net_url_setpassword(urip, value) spdIoT_string_setvalue(urip->password, value)
#define spdIoT_net_url_sethost(urip, value) spdIoT_string_setvalue(urip->host, value)
#define spdIoT_net_url_setport(urip, value) (urip->port = value)
#define spdIoT_net_url_setpath(urip, value) spdIoT_string_setvalue(urip->path, value)
#define spdIoT_net_url_addpath(urip, value) spdIoT_net_uri_addpath(urip, value)
#define spdIoT_net_url_setquery(urip, value) spdIoT_string_setvalue(urip->query, value)
#define spdIoT_net_url_setfragment(urip, value) spdIoT_string_setvalue(urip->fragment, value)

#define spdIoT_net_url_geturi(urip) spdIoT_string_getvalue(urip->uri)
#define spdIoT_net_url_getprotocol(urip) spdIoT_string_getvalue(urip->protocol)
#define spdIoT_net_url_getuser(urip) spdIoT_string_getvalue(urip->user)
#define spdIoT_net_url_getpassword(urip) spdIoT_string_getvalue(urip->password)
#define spdIoT_net_url_gethost(urip) spdIoT_string_getvalue(urip->host)
#define spdIoT_net_url_getport(urip) (urip->port)
#define spdIoT_net_url_getpath(urip) spdIoT_string_getvalue(urip->path)
#define spdIoT_net_url_getquery(urip) spdIoT_string_getvalue(urip->query)
#define spdIoT_net_url_getfragment(urip) spdIoT_string_getvalue(urip->fragment)
#define spdIoT_net_url_getrequest(urip) spdIoT_net_uri_getrequest(urip)

#define spdIoT_net_url_hasuri(urip) ((0 < spdIoT_string_length(urip->uri)) ? 1 : 0)
#define spdIoT_net_url_hasprotocol(urip) ((0 < spdIoT_string_length(urip->protocol)) ? 1 : 0)
#define spdIoT_net_url_hasuser(urip) ((0 < spdIoT_string_length(urip->user)) ? 1 : 0)
#define spdIoT_net_url_haspassword(urip) ((0 < spdIoT_string_length(urip->password)) ? 1 : 0)
#define spdIoT_net_url_hashost(urip) ((0 < spdIoT_string_length(urip->host)) ? 1 : 0)
#define spdIoT_net_url_hasport(urip) ((0 < urip->port) ? 1 : 0)
#define spdIoT_net_url_haspath(urip) ((0 < spdIoT_string_length(urip->path)) ? 1 : 0)
#define spdIoT_net_url_hasquery(urip) ((0 < spdIoT_string_length(urip->query)) ? 1 : 0)
#define spdIoT_net_url_hasfragment(urip) ((0 < spdIoT_string_length(urip->fragment)) ? 1 : 0)
#define spdIoT_net_url_hasrequest(urip) spdIoT_net_uri_hasrequest(urip)

#define spdIoT_net_url_ishttpprotocol(urip) spdIoT_streq(spdIoT_string_getvalue(urip->protocol), spdIoT_NET_URI_PROTOCOL_HTTP)

#define spdIoT_net_url_isabsolute(uripath) spdIoT_net_uri_hasprotocol(uripath)
#define spdIoT_net_url_isrelative(urip) ((spdIoT_net_uri_hasprotocol(urip) == 1) ? 0 : 1)
#define spdIoT_net_url_isabsolutepath(uripath) spdIoT_net_uri_isabsolutepath(uripath)

//const char* spdIoT_net_gethosturl(const char* host, int port, const char* uri, char* buf, size_t bufSize);
//const char* spdIoT_net_getmodifierhosturl(const char* host, int port, const char* uri, const char* begin, const char* end, char* buf, size_t bufSize);

#define spdIoT_net_url_getquerydictionary(urip) spdIoT_net_uri_getquerydictionary(urip);

#ifdef __cplusplus
}
#endif

#endif // _SPDIoT_URL_H_

