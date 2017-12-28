#ifndef _UH_URL_H_
#define _UH_URL_H_

#include "uri.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _uhNetURI uhNetURL;


#define uh_net_url_new() uh_net_uri_new()
#define uh_net_url_delete(urip) uh_net_uri_delete(urip);
#define uh_net_url_clear(urip) uh_net_uri_clear(urip);

#define uh_net_url_set(urip, value) uh_net_uri_set(urip, value)
#define uh_net_url_setvalue(urip, value) uh_net_uri_setvalue(urip, value)
#define uh_net_url_rebuild(urip) uh_net_uri_rebuild(urip)
#define uh_net_url_getvalue(urip) uh_net_uri_getvalue(urip)

#define uh_net_url_seturi(urip, value) uh_string_setvalue(urip->uri, value)
#define uh_net_url_setprotocol(urip, value) uh_string_setvalue(urip->protocol, value)
#define uh_net_url_setuser(urip, value) uh_string_setvalue(urip->user, value)
#define uh_net_url_setpassword(urip, value) uh_string_setvalue(urip->password, value)
#define uh_net_url_sethost(urip, value) uh_string_setvalue(urip->host, value)
#define uh_net_url_setport(urip, value) (urip->port = value)
#define uh_net_url_setpath(urip, value) uh_string_setvalue(urip->path, value)
#define uh_net_url_addpath(urip, value) uh_net_uri_addpath(urip, value)
#define uh_net_url_setquery(urip, value) uh_string_setvalue(urip->query, value)
#define uh_net_url_setfragment(urip, value) uh_string_setvalue(urip->fragment, value)

#define uh_net_url_geturi(urip) uh_string_getvalue(urip->uri)
#define uh_net_url_getprotocol(urip) uh_string_getvalue(urip->protocol)
#define uh_net_url_getuser(urip) uh_string_getvalue(urip->user)
#define uh_net_url_getpassword(urip) uh_string_getvalue(urip->password)
#define uh_net_url_gethost(urip) uh_string_getvalue(urip->host)
#define uh_net_url_getport(urip) (urip->port)
#define uh_net_url_getpath(urip) uh_string_getvalue(urip->path)
#define uh_net_url_getquery(urip) uh_string_getvalue(urip->query)
#define uh_net_url_getfragment(urip) uh_string_getvalue(urip->fragment)
#define uh_net_url_getrequest(urip) uh_net_uri_getrequest(urip)

#define uh_net_url_hasuri(urip) ((0 < uh_string_length(urip->uri)) ? 1 : 0)
#define uh_net_url_hasprotocol(urip) ((0 < uh_string_length(urip->protocol)) ? 1 : 0)
#define uh_net_url_hasuser(urip) ((0 < uh_string_length(urip->user)) ? 1 : 0)
#define uh_net_url_haspassword(urip) ((0 < uh_string_length(urip->password)) ? 1 : 0)
#define uh_net_url_hashost(urip) ((0 < uh_string_length(urip->host)) ? 1 : 0)
#define uh_net_url_hasport(urip) ((0 < urip->port) ? 1 : 0)
#define uh_net_url_haspath(urip) ((0 < uh_string_length(urip->path)) ? 1 : 0)
#define uh_net_url_hasquery(urip) ((0 < uh_string_length(urip->query)) ? 1 : 0)
#define uh_net_url_hasfragment(urip) ((0 < uh_string_length(urip->fragment)) ? 1 : 0)
#define uh_net_url_hasrequest(urip) uh_net_uri_hasrequest(urip)

#define uh_net_url_ishttpprotocol(urip) uh_streq(uh_string_getvalue(urip->protocol), uh_NET_URI_PROTOCOL_HTTP)

#define uh_net_url_isabsolute(uripath) uh_net_uri_hasprotocol(uripath)
#define uh_net_url_isrelative(urip) ((uh_net_uri_hasprotocol(urip) == 1) ? 0 : 1)
#define uh_net_url_isabsolutepath(uripath) uh_net_uri_isabsolutepath(uripath)

//const char* uh_net_gethosturl(const char* host, int port, const char* uri, char* buf, size_t bufSize);
//const char* uh_net_getmodifierhosturl(const char* host, int port, const char* uri, const char* begin, const char* end, char* buf, size_t bufSize);

#define uh_net_url_getquerydictionary(urip) uh_net_uri_getquerydictionary(urip);

#ifdef __cplusplus
}
#endif

#endif // _UH_URL_H_

