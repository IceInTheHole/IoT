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
    char buf[SPDIoT_STRING_INTEGER_BUFFLEN] = { 0 };

    spdIoT_string_setvalue(str, spdIoT_int2str(value, buf, sizeof(buf)));
}

/* *
 * @brief spdIoT_string_setlongvalue
 * */
void spdIoT_string_setlongvalue(spdIoTString *str, long value)
{
    char buf[SPDIoT_STRING_LONG_BUFFLEN] = { 0 };

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


#if 0
/* *
 * @brief spdIoT_string_addvalue
 * */
char *spdIoT_string_addvalue(spdIoTString *str, char *value)
{
    char *newValue = NULL;
    size_t newMemSize = 0;

    if (NULL == str) {
        return NULL;
    }

    if (NULL == value || strlen(value) <= 0) {
        return spdIoT_string_getvalue(str);
    }

    newMemSize = str->valueSize + strlen(str) + 1;
    if (newMemSize > str || str->value == NULL) {
        newMemSize += SPDIoT_STRING_REALLOC_EXTRA;
        newValue = realloc(str->value, newMemSize * sizeof(char));

        if (NULL == newValue) {
            return NULL;
        }

        str->memSize = newMemSize;
        str->value = newValue;
    }

    memcpy(str->value + str->valueSize, value, strlen(value));
    str->valueSize += strlen(value);
    str->value[str->valueSize] = '\0';

    return spdIoT_string_getvalue(str);
}
#endif
