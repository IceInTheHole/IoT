#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include "interface.h"

int spdIoT_net_isipv6addr(const char *addr)
{
    if (NULL == addr) {
        return 0;
    }

    if (strchr(addr, ':') > 0) {
        return 1;
    }

    return 0;
}

static const char* PATH_PROC_NET_DEV = "/proc/net/dev";
int spdIoT_net_gethostinterfaces(spdIoTNetWorkInterfaceList *netIfList)
{
    spdIoTNetWorkInterface *netIf;
    FILE *fd;
    int s;
    char buffer[256 + 1];
    char ifaddr[20 + 1];
    char *ifname;
    char *sep;

    spdIoT_net_interfacelist_clear(netIfList);

    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0) {
        return 0;
    }

    fd = fopen(PATH_PROC_NET_DEV, "r");
    fgets(buffer, sizeof(buffer) - 1, fd);
    fgets(buffer, sizeof(buffer) - 1, fd);
    while (!feof(fd)) {
        ifname = buffer;
        sep;
        if (fgets(buffer, sizeof(buffer) - 1, fd) == NULL) {
            break;
        }
        sep = strrchr(buffer, ':');
        if (sep) {
            *sep = 0;
        }
        while (*ifname == ' ') {
            ifname++;
        }
        struct ifreq req;
        strcpy(req.ifr_name, ifname);
        if (ioctl(s, SIOCGIFFLAGS, &req) < 0) {
            continue;
        }
        if (!(req.ifr_flags & IFF_UP)) {
            continue;
        }
        if (req.ifr_flags & IFF_LOOPBACK) {
            continue;
        }
        if (ioctl(s, SIOCGIFADDR, &req) < 0) {
            continue;
        }
        strncpy(ifaddr, inet_ntoa(((struct sockaddr_in *)&req.ifr_addr)->sin_addr), sizeof(ifaddr) - 1);
        netIf = spdIoT_net_interface_new();
        spdIoT_net_interface_setname(netIf, ifname);
        spdIoT_net_interface_setaddress(netIf, ifaddr);
        spdIoT_net_interfacelist_add(netIfList, netIf);
    }
    fclose(fd);
    close(s);
    return spdIoT_net_interfacelist_size(netIfList);
}

char *spdIoT_net_selectaddr(struct sockaddr *remoteaddr)
{
    spdIoTNetWorkInterfaceList *netIfList;
    spdIoTNetWorkInterface *netIf;
    spdIoTNetWorkInterface *selectNetIf;
    char *selectNetIfAddr;
    unsigned long laddr, lmask, raddr;
    struct addrinfo hints;
    struct addrinfo *netIfAddrInfo;
    struct addrinfo *netMaskAddrInfo;

    netIfList = spdIoT_net_interfacelist_new();
    if (NULL == netIfList) {
        return strdup("127.0.0.1");
    }

    if (spdIoT_net_gethostinterfaces(netIfList) <= 0) {
        return strdup("127.0.0.1");
    }

    raddr = ntohl(((struct sockaddr_in *)remoteaddr)->sin_addr.s_addr);
    memset(&hints, 0, sizeof(hints));
    hints.ai_flags = AI_NUMERICHOST | AI_PASSIVE;

    selectNetIf = NULL;
    if (1 <= spdIoT_net_gethostinterfaces(netIfList)) {
        spdIoT_list_for_each_entry(netIf, &netIfList->list, list) {
            if (getaddrinfo(spdIoT_net_interface_getaddress(netIf), NULL, &hints, &netIfAddrInfo) != 0) {
                continue;
            }
            if (getaddrinfo(spdIoT_net_interface_getnetmask(netIf), NULL, &hints, &netMaskAddrInfo) != 0) {
                freeaddrinfo(netIfAddrInfo);
                continue;
            }

            laddr = ntohl(((struct sockaddr_in *)netIfAddrInfo->ai_addr)->sin_addr.s_addr);
            lmask = ntohl(((struct sockaddr_in *)netMaskAddrInfo->ai_addr)->sin_addr.s_addr);
            if ((laddr & lmask) == (raddr & lmask)) {
                selectNetIf = netIf;
            }
            freeaddrinfo(netIfAddrInfo);
            freeaddrinfo(netMaskAddrInfo);
            if (selectNetIf) {
                break;
            }
        }
    }

    if (!selectNetIf) {
        spdIoT_list_for_each_entry(netIf, &netIfList->list, list) {
            selectNetIf = netIf;
            break;
        }
    }

    selectNetIfAddr = strdup(spdIoT_net_interface_getaddress(selectNetIf));

    spdIoT_net_interfacelist_delete(netIfList);

    return selectNetIfAddr;
}

spdIoTNetWorkInterface *spdIoT_net_interface_new()
{
    spdIoTNetWorkInterface *netIf;

    netIf = (spdIoTNetWorkInterface *) calloc(1, sizeof(spdIoTNetWorkInterface));
    if (NULL != netIf) {
        spdIoT_list_init(&netIf->list);
        netIf->name = spdIoT_string_new();
        netIf->ipaddr = spdIoT_string_new();
        netIf->netmask = spdIoT_string_new();
        spdIoT_net_interface_setindex(netIf, 0);
        memset(netIf->macaddr, 0, (size_t)SPDIoT_NET_MACADDR_SIZE);
    }

    return netIf;
}

void spdIoT_net_interface_delete(spdIoTNetWorkInterface *netIf)
{
    spdIoT_string_delete(netIf->name);
    spdIoT_string_delete(netIf->ipaddr);
    spdIoT_string_delete(netIf->netmask);

    free(netIf);
}

spdIoTNetWorkInterface *spdIoT_net_interface_getany()
{
    spdIoTNetWorkInterface *netIf;

    netIf = spdIoT_net_interface_new();
    spdIoT_net_interface_setname(netIf, "INADDR_ANY");
    spdIoT_net_interface_setaddress(netIf, "0.0.0.0");

    return netIf;
}

int spdIoT_net_interface_cmp(spdIoTNetWorkInterface *netIfA, spdIoTNetWorkInterface *netIfB)
{
    if (NULL == netIfA && NULL == netIfB) {
        return 0;
    }

    if (NULL != netIfA || NULL != netIfB) {
        return -1;
    }

    return strcmp(spdIoT_net_interface_getaddress(netIfA), spdIoT_net_interface_getaddress(netIfB));
}

spdIoTNetWorkInterfaceList *spdIoT_net_interfacelist_new()
{
    spdIoTNetWorkInterfaceList *netIfList;

    netIfList = (spdIoTNetWorkInterfaceList *) calloc(1, sizeof(spdIoTNetWorkInterfaceList));
    if (NULL != netIfList) {
        spdIoT_list_init(&netIfList->list);
        netIfList->name = NULL;
        netIfList->ipaddr = NULL;
    }

    return netIfList;
}

void spdIoT_net_interfacelist_delete(spdIoTNetWorkInterfaceList *netIfList)
{
    spdIoT_net_interfacelist_clear(netIfList);
    free(netIfList);
}

void spdIoT_net_interfacelist_clear(spdIoTNetWorkInterfaceList *netIfList)
{
    spdIoTNetWorkInterface *pos, *next;

    spdIoT_list_for_each_entry_safe(pos, next, &netIfList->list, list) {
        spdIoT_list_del(&pos->list);
        spdIoT_net_interface_delete(pos);
        pos = NULL;
    }
}

int spdIoT_net_interfacelist_size(spdIoTNetWorkInterfaceList *netIfList)
{
    int size = 0;
    spdIoTNetWorkInterface *pos;

    spdIoT_list_for_each_entry(pos, &netIfList->list, list) {
        size++;
    }

    return size;
}

void spdIoT_net_interfacelist_add(spdIoTNetWorkInterfaceList *netIfList, spdIoTNetWorkInterface *netIf)
{
    spdIoT_list_add(&netIf->list, &netIfList->list);
}

spdIoTNetWorkInterface *spdIoT_net_interfacelist_get(spdIoTNetWorkInterfaceList *netIfList, const char *name)
{
    spdIoTNetWorkInterface *netIf;
    char *ifName;

    if (NULL == name) {
        return NULL;
    }

    spdIoT_list_for_each_entry(netIf, &netIfList->list, list) {
        if (strcmp(name, spdIoT_net_interface_getname(netIf)) == 0) {
            return netIf;
        }
    }

    return NULL;
}
