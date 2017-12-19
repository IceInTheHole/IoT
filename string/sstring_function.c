#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define UNUSE(x)

/* *
 * @brief spdIoT_int2str
 * */
const char *spdIoT_int2str(int value, char *buf, size_t bufSize)
{
    UNUSE(bufsize);

    sprintf(buf, "%d", value);

    return buf;
}

/* *
 * @brief spdIoT_long2str
 * */
const char *spdIoT_long2str(long value, char *buf, size_t bufSize)
{
    UNUSE(bufSize);
    sprintf(buf, "%ld", value);

    return buf;
}

/* *
 * @brief spdIoT_float2str
 * */
const char *spdIoT_float2str(float value, char *buf, size_t bufSize)
{
    UNUSE(bufSize);
    sprintf(buf, "%f", value);

    return buf;
}

/* *
 * @brief spdIoT_double2str
 * */
const char *spdIoT_double2str(double value, char *buf, size_t bufSize)
{
    UNUSE(bufSize);
    sprintf(buf, "%lf", value);

    return buf;
}


/* *
 * @brief spdIoT_sizet2str
 * */
const char *spdIoT_sizet2str(size_t value, char *buf, size_t bufSize)
{
    UNUSE(bufSize);
    sprintf(buf, "%zd", value);

    return buf;
}


/* *
 * @brief spdIoT_sizet2str
 * */
const char *spdIoT_ssizet2str(ssize_t value, char *buf, size_t bufSize)
{
    UNUSE(bufSize);
    sprintf(buf, "%zd", value);

    return buf;
}

char *spdIoT_strstrip(char *str)
{
    size_t strLen;
    ssize_t i;

    strLen = strlen(str);
    if (0 == strLen) {
        return str;
    }

    for (i = strLen - 1; i >= 0; i--) {
        if (isspace(str[i])) {
            strLen--;
        } else {
            break;
        }
    }

    for (i = 0; i < strLen; i++) {
        if (!isspace(str[i])) {
            break;
        }
    }

    memmove(str, str + i, strLen - i);
    str[strLen - i] = '\0';

    return str;
}

char *spdIoT_strstripall(char *str)
{
    size_t strLen, newStrLen;
    ssize_t i, j;
    char *newStr;

    strLen = strlen(str);
    if (0 == strLen) {
        return str;
    }

    newStrLen = strLen;
    for (i = strLen - 1; i >= 0; i--) {
        if (isspace(str[i])) {
            newStrLen--;
        }
    }

    newStr = (char *) calloc(1, newStrLen + 1);
    if (NULL != newStr) {
        j = 0;
        for (i = 0; i < strLen; i++) {
            if (!isspace(str[i])) {
                newStr[j++] = str[i];
            }
        }
        strcpy(str, newStr);
        str[newStrLen] = '\0';
        free(newStr);
    }

    return str;
}


char *spdIoT_strstripstr(char *str, char *delim, size_t ndelim)
{
    size_t strLen;
    ssize_t i;

    strLen = strlen(str);
    if (0 == strLen) {
        return str;
    }

    for (i = strLen - ndelim; i >= 0; i -= ndelim) {

        if (strncmp(str + i, delim, ndelim) == 0) {
            strLen -= ndelim;
        } else {
            break;
        }
    }

    for (i = 0; i < strLen; i++) {
        if (strncmp(str + i, delim, ndelim) != 0) {
            break;
        }
    }

    memmove(str, str + i, strLen - 1);
    str[strLen] = '\0';

    return str;
}

char *spdIoT_strstripstrall(char *str, char *delim, size_t ndelim)
{
    size_t strLen, newStrLen;
    ssize_t i, j;
    char *newStr;

    strLen = strlen(str);
    if (0 == strLen) {
        return str;
    }

    newStrLen = strLen;
    for (i = 0; i < strLen;) {
        if (strncmp(str + i, delim, ndelim) == 0) {
            newStrLen -= ndelim;
            i += ndelim;
        } else {
            i++;
        }
    }

    newStr = (char *) calloc(1, newStrLen + 1);
    if (NULL != newStr) {
        j = 0;
        for (i = 0; i < strLen;) {
            if (strncmp(str + i, delim, ndelim) == 0) {
                i += ndelim;
            } else {
                newStr[j] = str[i];
                i++;
                j++;
            }
        }
        strcpy(str, newStr);
        str[newStrLen] = '\0';
        free(newStr);
    }

    return str;
}
