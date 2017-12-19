#ifndef _SPDIoT_SOCKET_H_
#define _SPDIoT_SOCKET_H_

#include "../string/sstring.h"

#if defined(SPDIoT_USE_OPENSSL)
#include <openssl/ssl.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define SPDIoT_NET_SOCKET_NONE      0

#define SPDIoT_NET_SOCKET_STREAM    0x01
#define SPDIoT_NET_SOCKET_DGRAM     0x02

#define SPDIoT_NET_SOCKET_CLIENT    1
#define SPDIoT_NET_SOCKET_SERVER    2

#define SPDIoT_NET_SOCKET_MAXHOST   32
#define SPDIoT_NET_SOCKET_MAXSERV   32

#define SPDIoT_SOCKET_LF '\n'

#define SPDIoT_NET_SOCKET_DGRAM_RECV_BUFSIZE    512
#define SPDIoT_NET_SOCKET_ANCILLARY_BUFSIZE     512
#define SPDIoT_NET_SOCKET_MULTICAST_DEFAULT_TTL 4
#define SPDIoT_NET_SOCKET_AUTO_IP_NET           0xa9fe0000
#define SPDIoT_NET_SOCKET_AUTO_IP_MASK          0xffff0000

typedef struct _spdIoTSocket {
	int id;
	int type;
	int direction;
    spdIoTString *ipaddr;
	int port;
#if define(SPDIoT_USE_OPENSSL)
	SSL_CTX *ctx;
	SSL *ssl;
#endif
}spdIoTSocket;

typedef struct _spdIoTDatagramPacket {
    spdIoTString *data;
    spdIoTString *localAddress;
    int localPort;
    spdIoTString *remoteAddress;
    int remotePort;
}spdIoTDatagramPacket;

spdIoTSocket *spdIoT_socket_new(int type);
#define spdIoT_socket_stream_new() spdIoT_socket_new(SPDIoT_NET_SOCKET_STREAM)
#define spdIoT_socket_dgram_new()  spdIoT_socket_new(SPDIoT_NET_SOCKET_DGRAM)
int spdIoT_socket_delete(spdIoTSocket *sock);

void spdIoT_socket_setid(spdIoTSocket *sock, int value);
#define spdIoT_socket_getid(socket) (socket->id)

#define spdIoT_socket_settype(socket, value) (socket->type = value)
#define spdIoT_socket_gettype(socket) (socket->type)
#define spdIoT_socket_issocketstream(socket) (socket->type & SPDIoT_NET_SOCKET_STREAM ? 1 : 0)
#define spdIoT_socket_isdatagramstream(socket) (socket->type & SPDIoT_NET_SOCKET_DGRAM ? 1 : 0)

#define spdIoT_socket_setdirection(socket, value) (socket->direction = value)
#define spdIoT_socket_getdirection(socket) (socket->direction)
#define spdIoT_socket_isclient(socket) (socket->direction == SPDIoT_NET_SOCKET_CLIENT ? 1 : 0)
#define spdIoT_socket_isserver(socket) (socket->direction == SPDIoT_NET_SOCKET_SERVER ? 1 : 0)

#define spdIoT_socket_setaddress(socket, value) spdIoT_string_setvalue(socket->ipaddr, value)
#define spdIoT_socket_setport(socket, value) (socket->port, value)
#define spdIoT_socket_getaddress(socket) spdIoT_string_getvalue(socket->ipaddr)
#define spdIoT_socket_getport(socket) (socket->port)

int spdIoT_socket_isbound(spdIoTSocket *sock);
int spdIoT_socket_close(spdIoTSocket *sock);

int spdIoT_socket_listen(spdIoTSocket *sock);

int spdIoT_socket_bind(spdIoTSocket *sock, int bindPort, const char *bindAddr, int bindFlag, int reuseFlag);
int spdIoT_socket_accept(spdIoTSocket *serverSock, spdIoTSocket *clientSock);
int spdIoT_socket_connect(spdIoTSocket *sock, const char *addr, int port);
ssize_t spdIoT_socket_read(spdIoTSocket *sock, char *buffer, size_t bufferLen);
size_t spdIoT_socket_write(spdIoTSocket *sock, const char *buffer, size_t bufferLen);
ssize_t spdIoT_socket_readLine(spdIoTSocket *sock, char *buffer, size_t bufferLen);
size_t spdIoT_socket_skip(spdIoTSocket *sock, size_t skipLen);

size_t spdIoT_socket_sendto(spdIoTSocket *sock, const char *addr, int port, const char *data, size_t dataLen);
ssize_t spdIoT_socket_recv(spdIoTSocket *sock, spdIoTDatagramPacket *dgmPkt);

int spdIoT_socket_joingroup(spdIoTSocket *sock, const char *mcastAddr, const char *ifAddr);

int spdIoT_socket_setreuseaddress(spdIoTSocket *sock, int flag);
int spdIoT_socket_setmulticastttl(spdIoTSocket *sock, int ttl);
int spdIoT_socket_settimeout(spdIoTSocket *sock, int sec);


spdIoTDatagramPacket *spdIoT_socket_datagram_packet_new();
void spdIoT_socket_datagram_packet_delete(spdIoTDatagramPacket *dgmPkt);

#define spdIoT_socket_datagram_packet_setdata(dgmPkt, value) spdIoT_string_setvalue(dgmPkt->data, value)
#define spdIoT_socket_datagram_packet_getdata(dgmPkt) spdIoT_string_getvalue(dgmPkt->data)

#define spdIoT_socket_datagram_packet_setlocaladdress(dgmPkt, addr) spdIoT_string_setvalue(dgmPkt->localAddress, addr)
#define spdIoT_socket_datagram_packet_getlocaladdress(dgmPkt) spdIoT_string_getvalue(dgmPkt->localAddress)
#define spdIoT_socket_datagram_packet_setlocalport(dgmPkt, port) (dgmPkt->localPort = port)
#define spdIoT_socket_datagram_packet_getlocalport(dgmPkt) (dgmPkt->localPort)
#define spdIoT_socket_datagram_packet_setremoteaddress(dgmPkt, addr) spdIoT_string_setvalue(dgmPkt->remoteAddress, addr)
#define spdIoT_socket_datagram_packet_getremoteaddress(dgmPkt) spdIoT_string_getvalue(dgmPkt->remoteAddress)
#define spdIoT_socket_datagram_packet_setremoteport(dgmPkt, port) (dgmPkt->remotePort = port)
#define spdIoT_socket_datagram_packet_getremoteport(dgmPkt) (dgmPkt->remotePort)

void spdIoT_socket_datagram_packet_copy(spdIoTDatagramPacket *dstDgmPkt, spdIoTDatagramPacket *srcDgmPkt);

#if define(SPDIoT_USE_OPENSSL)
#define SPDIoT_NET_SOCKET_SSL 0x0100
#define spdIoT_socket_ssl_new() spdIoT_socket_new(SPDIoT_NET_SOCKET_STREAM | SPDIoT_NET_SOCKET_SSL)
#define spdIoT_socket_isssl(sock) (sock->type & SPDIoT_NET_SOCKET_SSL ? 1 : 0)
#endif

#ifdef __cplusplus
}
#endif

#endif
