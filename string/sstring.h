#ifndef _SPDIoT_STRING_H_
#define _SPDIoT_STRING_H_

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

const char* spdIoT_int2str(int value, char* buf, size_t bufSize);
const char* spdIoT_long2str(long value, char* buf, size_t bufSize);
const char* spdIoT_float2str(float value, char* buf, size_t bufSize);
const char* spdIoT_double2str(double value, char* buf, size_t bufSize);
const char* spdIoT_sizet2str(size_t value, char* buf, size_t bufSize);
const char* spdIoT_ssizet2str(ssize_t value, char* buf, size_t bufSize);

#define spdIoT_str2int(value) (value ? atoi(value) : 0)
#define spdIoT_str2long(value) (value ? atol(value) : 0)
#define spdIoT_strhex2long(value) (value ? strtol(value, NULL, 16) : 0)
#define spdIoT_strhex2ulong(value) (value ? strtoul(value, NULL, 16) : 0)
#define spdIoT_str2float(value) ((float)(value ? atof(value) : 0.0))
#define spdIoT_str2double(value) (value ? atof(value) : 0.0)
#define spdIoT_str2sizet(value) ((size_t)(value ? atol(value) : 0))
#define spdIoT_str2ssizet(value) ((ssize_t)(value ? atol(value) : 0))

/* *
 * @brief string data
 * */
typedef struct _spdIoTString {
    char *value;
    size_t memSize;
    size_t valueSize;
}spdIoTString;

/******************************
 * string function
 *****************************/
spdIoTString *spdIoT_string_new();
void spdIoT_string_delete(spdIoTString *str);
void spdIoT_string_clear(spdIoTString *str);

void spdIoT_string_setvalue(spdIoTString *str, const char *value);
void spdIoT_string_setintvalue(spdIoTString * str, int value);
void spdIoT_string_setlongvalue(spdIoTString * str, long value);

#ifdef __cplusplus
}
#endif

#endif
