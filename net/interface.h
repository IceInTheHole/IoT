#ifndef _SPDIoT_INTERFACE_H_
#define _SPDIoT_INTERFACE_H_

#include <netdb.h>
#include "../string/sstring.h"
#include "../list/list.h"

#define SPDIoT_NET_IPV4_ADDRSTRING_MAXSIZE ((3 * 4) + (1 * 3) + 1)
#define SPDIoT_NET_IPV6_ADDRSTRING_MAXSIZE (1 + (8 * 4) + (1 * 7) + 1 + 1)

#define SPDIoT_NET_IPV4_LOOPBACK "127.0.0.1"
#define SPDIoT_NET_IPV6_LOOPBACK "fixmelater"
#define SPDIoT_NET_MACADDR_SIZE 6

typedef struct _spdIoTNetWorkInterface {
    spdIoTListHead list;
    spdIoTString *name;
    spdIoTString *ipaddr;
    spdIoTString *netmask;
    unsigned char macaddr[SPDIoT_NET_MACADDR_SIZE];
    int index;
}spdIoTNetWorkInterface, spdIoTNetWorkInterfaceList;

spdIoTNetWorkInterface *spdIoT_net_interface_new();
void spdIoT_net_interface_delete();
spdIoTNetWorkInterface *spdIoT_net_interface_getany();
#define spdIoT_net_interface_setname(netIf, value) spdIoT_string_setvalue(netIf->name, value)
#define spdIoT_net_interface_getname(netIf) spdIoT_string_getvalue(netIf->name)
#define spdIoT_net_interface_setaddress(netIf, value) spdIoT_string_setvalue(netIf->ipaddr, value)
#define spdIoT_net_interface_getaddress(netIf) spdIoT_string_getvalue(netIf->ipaddr)
#define spdIoT_net_interface_setnetmask(netIf, value) spdIoT_string_setvalue(netIf->netmask, value)
#define spdIoT_net_interface_getnetmask(netIf) spdIoT_string_getvalue(netIf->netmask)
#define spdIoT_net_interface_setmacaddress(netIf, value) memcpy(netIf->macaddr, value, SPDIoT_NET_MACADDR_SIZE)
#define spdIoT_net_interface_getmacaddress(netIf, buf) memcpy(buf, netIf->macaddr, SPDIoT_NET_MACADDR_SIZE)
#define spdIoT_net_interface_setindex(netIf, value) (netIf->index = value)
#define spdIoT_net_interface_getindex(netIf) (netIf->index)

int spdIoT_net_interface_cmp(spdIoTNetWorkInterface *netIfA, spdIoTNetWorkInterface *netIfB);

spdIoTNetWorkInterfaceList *spdIoT_net_interfacelist_new();
void spdIoT_net_interfacelist_delete(spdIoTNetWorkInterfaceList *netIfList);
void spdIoT_net_interfacelist_clear(spdIoTNetWorkInterfaceList *netIfList);
void spdIoT_net_interfacelist_add(spdIoTNetWorkInterfaceList *netIfList, spdIoTNetWorkInterface *netIf);
int spdIoT_net_interfacelist_size(spdIoTNetWorkInterfaceList *netIfList);
spdIoTNetWorkInterface *spdIoT_net_interfacelist_get(spdIoTNetWorkInterfaceList *netIfList, const char *name);

int spdIoT_net_isipv6addr(const char *addr);
int spdIoT_net_gethostinterfaces(spdIoTNetWorkInterfaceList *netIfList);
char *spdIoT_net_selectaddr(struct sockaddr *remoteaddr);
#endif // _SPDIoT_INTERFACE_H_
