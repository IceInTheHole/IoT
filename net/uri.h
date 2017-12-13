#ifndef _SPDIoT_URI_H_
#define _SPDIoT_URI_H_

#include "../string/sstring.h"
#include "../dictionary/dictionary.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SPDIoT_NET_URI_KNKOWN_PORT (-1)
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


#ifdef __cplusplus
}
#endif
#endif // _SPDIoT_URI_H_
