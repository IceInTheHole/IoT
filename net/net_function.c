#include <stdlib.h>
#include <string.h>
#include "interface.h"

int spdIoT_net_isipv6addr(const char *addr)
{
    if (NULL == addr) {
        return 0;
    }

    if (strchr(addr, ":", 1) > 0) {
        return 1;
    }

    return 0;
}
