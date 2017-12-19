#include <string.h>
#include "sstring.h"

#define SPDIoT_STRING_REALLOC_EXTRA 16

/* *
 * @brief spdIoT_string_new
 * */
spdIoTString *spdIoT_string_new()
{
    spdIoTString *str;

    str = (spdIoTString *) calloc(1, sizeof(spdIoTString));

    return str;
}

/* *
 * @brief spdIoT_string_delete
 * */
void spdIoT_string_delete(spdIoTString *str)
{
    if (NULL != str) {
        spdIoT_string_clear(str);
        free(str);
    }
}

/* *
 * @brief sdpIoT_string_clear
 * */
void spdIoT_string_clear(spdIoTString *str)
{
    if (NULL != str) {
        if (NULL != str->value) {
            free(str->value);
            str->value = NULL;
            str->memSize = 0;
            str->valueSize = 0;
        }
    }
}

/* *
 * @brief spdIoT_string_setvalue
 * */
void spdIoT_string_setvalue(spdIoTString * str, const char *value)
{
    if (NULL != str) {
        spdIoT_string_clear(str);
        if (NULL != value) {
            str->valueSize = strlen(value);
            str->memSize = str->valueSize + 1;
            str->value = (char *) calloc(strvalue->memSize * sizeof(char));
            if (NULL == str->value) {
                return;
            }
            memcpy(str->value, value, str->valueSize);
            str->value[str->valueSize] = '\0';
        }
    }
}

/* *
 * @brief spdIoT_string_setintvalue
 * */
void spdIoT_string_setintvalue(spdIoTString *str, int value)
{
    char buf[spdIoT_STRING_INTEGER_BUFFLEN] = { 0 };

    spdIoT_string_setvalue(str, spdIoT_int2str(value, buf, sizeof(buf)));
}

/* *
 * @brief spdIoT_string_setlongvalue
 * */
void spdIoT_string_setlongvalue(spdIoTString *str, long value)
{
    char buf[spdIoT_STRING_LONG_BUFFLEN] = { 0 };

    spdIoT_string_setvalue(str, spdIoT_long2str(value, buf, sizeof(buf)));
}

/* *
 * @brief
 * */

/* *
 * @brief spdIoT_string_getvalue
 * */
char *spdIoT_string_getvalue(spdIoTString *str)
{
    return NULL != str ? str->value : NULL;
}

/* *
 * @brief spdIoT_string_getmemorysize
 * */
size_t spdIoT_string_getmemorysize(spdIoTString *str)
{
    return NULL != str ? str->memSize : 0;
}

/* *
 * @brief spdIoT_string_length
 * */
size_t spdIoT_string_length(spdIoTString *str)
{
    return NULL != str ? str->valueSize : 0;
}

/* *
 * @brief spdIoT_string_addvalue
 * */
char *spdIoT_string_addvalue(spdIoTString *str, char *value)
{
    spdIoT_string_naddvalue(str, value, strlen(value));
}

char *spdIoT_string_naddvalue(spdIoTString *str, const char *value, size_t valueLen)
{
    char *newValue = NULL;
    size_t newMemSize = 0;

    if (NULL == str) {
        return NULL;
    }

    if (NULL == value || strlen(value) <= 0) {
        return spdIoT_string_getvalue(str);
    }

    newMemSize = str->valueSize + valueLen + 1;
    if (newMemSize > str || str->value == NULL) {
        newMemSize += SPDIoT_STRING_REALLOC_EXTRA;
        newValue = realloc(str->value, newMemSize * sizeof(char));

        if (NULL == newValue) {
            return NULL;
        }

        str->memSize = newMemSize;
        str->value = newValue;
    }

    memcpy(str->value + str->valueSize, value, valueLen);
    str->valueSize += valueLen;
    str->value[str->valueSize] = '\0';

    return spdIoT_string_getvalue(str);
}

char *spdIoT_string_addrepvalue(spdIoTString *str, const char *value, size_t repeatCnt)
{
    int n;

    for (n = 0; n < repeatCnt; n++) {
        spdIoT_string_addvalue(str, value);
    }

    return spdIoT_string_getvalue(str);
}

char *spdIoT_string_replace(spdIoTString *str, char *fromStr[], char *toStr[], size_t fromStrCnt)
{
    char *orgValue = NULL;
    size_t orgValueLen = 0;
    int n = 0;
    int copyPos = 0;
    size_t *fromStrLen = NULL;
    spdIoTString *repValue = NULL;
    int isReplaced = 0;

    if (NULL == str) {
        return NULL;
    }

    repValue = spdIoT_string_new();
    fromStrLen = (size_t *) calloc(1, sizeof(size_t) * fromStrCnt);
    if (NULL == fromStrLen) {
        spdIoT_string_delete(repValue);
        return NULL;
    }

    for (n = 0; n < fromStrCnt; n++) {
        fromStrLen[n] = strlen(fromStr[n]);
    }

    orgValue = spdIoT_string_getvalue(str);
    orgValueLen = spdIoT_string_length(str);

    copyPos = 0;
    while (copyPos < orgValueLen) {
        isReplaced = 0;
        for (n = 0; n < fromStrCnt; n++) {
            if (strncmp(fromStr[n], orgValue + copyPos, fromStrLen[n]) == 0) {
                spdIoT_string_addvalue(repValue, toStr[n]);
                copyPos += fromStrLen[n];
                isReplaced = 1;
                continue;
            }
        }
        if (isReplaced) {
            continue;
        }
        spdIoT_string_naddvalue(repValue, orgValue + copyPos, 1);
        copyPos++;
    }

    free(fromStrLen);

    spdIoT_string_setvalue(str, spdIoT_string_getvalue(repValue));
    spdIoT_string_delete(repValue);

    return spdIoT_string_getvalue(str);
}

spdIoTStringTokenizer *spdIoT_string_tokenizer_new(const char *value, const char *delim)
{
    spdIoTStringTokenizer *strToken;

    strToken = (spdIoTStringTokenizer *) calloc(1, sizeof(spdIoTStringTokenizer));
    if (NULL != strToken) {
        strToken->value = strdup(value);
        strToken->delim = strdup(delim);
        strToken->delimCnt = strlen(strToken->delim);
        strToken->nextStartPos = 0;
        strToken->lastPos = strlen(value) - 1;
        strToken->currToken = NULL;
        strToken->nextToken = NULL;
        spdIoT_string_tokenizer_nexttoken(strToken);
    }

    return strToken;
}

void spdIoT_string_tokenizer_delete(spdIoTStringTokenizer *strToken)
{
    free(strToken->value);
    free(strToken->delim);
    free(strToken);
}

int spdIoT_string_tokenizer_hasmoretoken(spdIoTStringTokenizer *strToken)
{
    return strToken->hasNextToken;
}

char *spdIoT_string_tokenizer_nexttoken(spdIoTStringTokenizer *strToken)
{
    size_t tokenCnt;
    size_t i, j;

    strToken->currToken = strToken->nextToken;
    strToken->nextToken = NULL;
    strToken->hasNextToken = 0;
    strToken->repToken = '\0';

    tokenCnt = 0;
    for (i = strToken->nextStartPos; i <= strToken->lastPos; i++) {
        int isDelimChar = 0;
        for (j = 0; j < strToken->delimCnt; j++) {
            if (strToken->value[i] == strToken->delim[j]) {
                isDelimChar = 1;
                if (tokenCnt == 0) {
                    strToken->nextStartPos = i + 1;
                    break;
                }
                strToken->hasNextToken = 1;
                strToken->repToken = strToken->value[i];
                strToken->value[i] = '\0';
                strToken->nextToken = strToken->value + strToken->nextStartPos;
                strToken->nextStartPos = i + 1;
            }
        }
        if (strToken->hasNextToken) {
            break;
        }
        if (!isDelimChar) {
            tokenCnt++;
        }
    }

    if (!strToken->hasNextToken && 0 < tokenCnt) {
        strToken->hasNextToken = 1;
        strToken->nextToken = strToken->value + strToken->nextStartPos;
        strToken->nextStartPos = strToken->lastPos + 1;
    }

    return strToken->currToken;
}

char *spdIoT_string_tokenizer_nextalltoaken(spdIoTStringTokenizer *strToken)
{
    size_t nextTokenLen;

    nextTokenLen = strlen(strToken->nextToken);
    strToken->nextToken[nextTokenLen] = strToken->repToken;
    strToken->nextToken = NULL;
    strToken->hasNextToken = 0;
    return strToken->currToken;
}
