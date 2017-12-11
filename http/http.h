#ifndef _SPDIoT_HTTP_H_
#define _SPDIoT_HTTP_H_

#include "../string/sstring.h"
#include "../list/list.h"
#include "uri.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _spdIoTHttpHeader {
    spdIoTListHead list;
    spdIoTString *name;
    spdIoTString *value;
}spdIoTHttpHeader, spdIoTHttpHeaderList;

typedef struct _spdIoTHttpPacket {
    spdIoTHttpHeaderList headerList;
    spdIoTString *content;
}spdIoTHttpPacket;

typedef struct _spdIoTHttpResponse {
    spdIoTHttpHeaderListHead headerList;
    spdIoTString *content;
    spdIoTString *version;
    int statusCode;
    spdIoTString *reasonPhrase;
    void *userData;
    int timeout;
}spdIoTHttpResponse;

typedef struct _spdIoTHttpRequest {
    spdIoTHttpHeaderListHead *headerList;
    spdIoTString *content;
    spdIoTString *method;
    spdIoTString *uri;
    spdIoTString *version;
    spdIoTString *userAgent;
    spdIoTHttpResponse *httpRes;
    spdIoTNetURI *postURL;
    void *userData;
    int timeout;
}spdIoTHttpRequest;

#ifdef __cplusplus
}
#endif
#endif
