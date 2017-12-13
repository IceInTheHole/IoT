#ifndef _SPDIoT_INTERFACE_H_
#define _SPDIoT_INTERFACE_H_

#define SPDIoT_NET_IPV4_ADDRSTRING_MAXSIZE ((3 * 4) + (1 * 3) + 1)
#define SPDIoT_NET_IPV6_ADDRSTRING_MAXSIZE (1 + (8 * 4) + (1 * 7) + 1 + 1)

#define SPDIoT_NET_IPV4_LOOPBACK "127.0.0.1"
#define SPDIoT_NET_IPV6_LOOPBACK "fixmelater"
#define SPDIoT_NET_MACADDR_SIZE 6

int spdIoT_net_isipv6addr(const char *addr);

#endif // _SPDIoT_INTERFACE_H_
