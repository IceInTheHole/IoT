#include <stdlib.h>

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
