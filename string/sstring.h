#ifndef _SPDIoT_STRING_H_
#define _SPDIoT_STRING_H_

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SPDIoT_STRING_INTEGER_BUFFLEN   16
#define SPDIoT_STRING_LONG_BUFFLEN      32
#define SPDIoT_STRING_FLOAT_BUFFLEN     64
#define SPDIoT_STRING_DOUBLE_BUFFLEN    64

const char* spdIoT_int2str(int value, char* buf, size_t bufSize);
const char* spdIoT_long2str(long value, char* buf, size_t bufSize);
const char* spdIoT_float2str(float value, char* buf, size_t bufSize);
const char* spdIoT_double2str(double value, char* buf, size_t bufSize);
const char* spdIoT_sizet2str(size_t value, char* buf, size_t bufSize);
const char* spdIoT_ssizet2str(ssize_t value, char* buf, size_t bufSize);

char *spdIoT_strstrip(char *str);
char *spdIoT_strstripall(char *str);
char *spdIoT_strstripstr(char *str, char *delim, size_t ndelim);
char *spdIoT_strstripstrall(char *str, char *delim, size_t ndelim);

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

typedef struct _spdIoTStringTokenizer {
    char *value;
    char *delim;
    size_t delimCnt;
    size_t nextStartPos;
    size_t lastPos;
    char *currToken;
    char *nextToken;
    char repToken;
    int hasNextToken;
}spdIoTStringTokenizer;

spdIoTStringTokenizer *spdIoT_string_tokenizer_new(const char *str, const char *delim);
void spdIoT_string_tokenizer_delete(spdIoTStringTokenizer *strToken);
int spdIoT_string_tokenizer_hasmoretoken(spdIoTStringTokenizer *strToken);
char *spdIoT_string_tokenizer_nexttoken(spdIoTStringTokenizer *strToken);
char *spdIoT_string_tokenizer_nextalltoken(spdIoTStringTokenizer *strToken);

/******************************
 * string function
 *****************************/
spdIoTString *spdIoT_string_new();
void spdIoT_string_delete(spdIoTString *str);
void spdIoT_string_clear(spdIoTString *str);

void spdIoT_string_setvalue(spdIoTString *str, const char *value);
void spdIoT_string_setintvalue(spdIoTString *str, int value);
void spdIoT_string_setlongvalue(spdIoTString * tr, long value);

char *spdIoT_string_getvalue(spdIoTString *str);
#define spdIoT_string_getintvalue(str) spdIoT_str2int(spdIoT_string_getvalue(str));
#define spdIoT_string_getlongvalue(str) spdIoT_str2long(spdIoT_string_getvalue(str));
#define spdIoT_string_getfloatvalue(str) spdIoT_str2float(spdIoT_string_getvalue(str));
#define spdIoT_string_getdoublevalue(str) spdIoT_str2double(spdIoT_string_getvalue(str));

size_t spdIoT_string_getmemorysize(spdIoTString *str);
size_t spdIoT_string_length(spdIoTString *str);
#ifdef __cplusplus
}
#endif

#endif
