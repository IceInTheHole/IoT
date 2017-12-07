#ifndef _SPDIoT_HTTP_H_
#define _SPDIoT_HTTP_H_

#include "../string/sstring.h"
#include "../list/list.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _spdIoTHttpHeader {
    spdIoTListHead list;
    spdIoTString *name;
    spdIoTString *value;
}sdpIoTHttpHeader, spdIoTHttpHeaderListHead, spdIoTHttpHeaderNode;

typedef struct _spdIoTHttpPacket {
    spdIoTHttpHeaderListHead headerList;
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
    spdIoTNetURL *postURL;
    void *userData;
    int timeout;
}spdIoTHttpRequest;

#ifdef __cplusplus
}
#endif
#endif
