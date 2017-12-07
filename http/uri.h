#ifndef _SPDIoT_URI_H_
#define _SPDIoT_URI_H_

#include "../string/sstring.h"

#ifdef __cplusplus
extern "C" {
#endif


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
    mUpnpDictionary* queryDictionary;
}spdIoTNetURI;


#ifdef __cplusplus
}
#endif
#endif // _SPDIoT_URI_H_
