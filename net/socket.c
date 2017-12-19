#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "socket.h"

#define spdIoT_socket_getrawtype(socket) (((socket->type & SPDIoT_NET_SOCKET_STREAM) == SPDIoT_NET_SOCKET_STREAM) ? SOCK_STREAM : SOCK_DGRAM)

spdIoTSocket *spdIoT_socket_new(int type)
{
    spdIoTSocket *sock;

    sock = (spdIoTSocket) calloc(1, sizeof(spdIoTSocket));
    if (NULL != sock) {
        sock->id = -1;
        spdIoT_socket_settype(sock, type);
        spdIoT_socket_setdirection(sock, SPDIoT_NET_SOCKET_NONE);
        sock->ipaddr = spdIoT_string_new();
        spdIoT_socket_setaddress(socket, "");
        spdIoT_socket_setport(sock, -1);
#if define(SPDIoT_USE_OPENSSL)
        sock->ctx = NULL;
        sock->ssl = NULL;
#endif
    }

    return sock;
}

int spdIoT_socket_delete(spdIoTSocket *sock)
{
    spdIoT_socket_close(sock);
    spdIoT_string_delete(sock->ipaddr);
    free(sock);

    return 0;
}

int spdIoT_socket_isbound(spdIoTSocket *sock)
{
    return sock->id > 0 ? 1 : 0;
}

void spdIoT_socket_setid(spdIoTSocket *sock, int value)
{
    sock->id = value;
}

int spdIoT_socket_close(spdIoTSocket *sock)
{
    if (!spdIoT_socket_isbound(sock)) {
        return 0;
    }

#if define(SPDIoT_USE_OPENSSL)
    if (spdIoT_socket_isssl(sock)) {
        if (sock->ssl) {
            SSL_shutdown(sock->ssl);
            SSL_free(sock->ssl);
            sock->ssl = NULL;
        }
        if (sock->ctx) {
            SSL_CTX_free(sock->ctx);
            sock->ctx = NULL;
        }
    }
#endif

    close(sock->id);
    return 0;
}

int spdIoT_socket_listen(spdIoTSocket *sock)
{
    return listen(sock->id, SOMAXCONN);
}

int spdIoT_socket_bind(spdIoTSocket *sock, int bindPort,
                       const char *bindAddr, int bindFlag, int reuseFlag)
{
    struct addrinfo *addrInfo;
    int ret = 0;

    if (spdIoT_socket_tosocketaddrinfo(spdIoT_socket_getrawtype(sock),
                                       bindAddr, bindPort, &addrInfo, bindFlag) < 0) {
        return -1;
    }
    spdIoT_socket_setid(sock, socket(addrInfo->ai_family, addrInfo->ai_socktype, 0));
    if (sock->id == -1) {
        spdIoT_socket_close(sock);
        return -1;
    }

    if (reuseFlag) {
        if (spdIoT_socket_setreuseaddress(sock, 1) < 0) {
            spdIoT_socket_close(sock);
            return -1;
        }
    }
    ret = bind(sock->id, addrInfo->ai_addr, addrInfo->ai_addrlen);
    freeaddrinfo(addrInfo);

    spdIoT_socket_setdirection(sock, SPDIoT_NET_SOCKET_SERVER);
    spdIoT_socket_setaddress(sock, bindAddr);
    spdIoT_socket_setport(sock, bindPort);

    return 0;
}

int spdIoT_socket_accept(spdIoTSocket *serverSock, spdIoTSocket *clientSock)
{
    struct socketadrr_in sockaddr;
    socklen_t socklen;
    char localAddr[SPDIoT_NET_SOCKET_MAXHOST];
    char localPort[SPDIoT_NET_SOCKET_MAXSERV];
    struct sockaddr_storage sockClientAddr;
    socklen_t nLength = sizeof(sockClientAddr);

    spdIoT_socket_setid(clientSock,
                        accept(serverSock->id, (struct sockaddr *)&sockClientAddr, nLength));
    if (clientSock->id < 0) {
        return -1;
    }

    spdIoT_socket_setaddress(clientSock, spdIoT_socket_getaddress(serverSock));
    spdIoT_socket_setport(clientSock, spdIoT_socket_getport(serverSock));
    socklen = sizeof(struct sockaddr_in);

    if (getsockname(clientSock->id, (struct sockaddr*)&sockaddr, &socklen) == 0
            && getnameinfo((struct sockaddr *)&sockaddr, socklen, localAddr, sizeof(localAddr),
                           localPort, sizeof(localPort), NI_NUMERICHOST | NI_NUMERICSERV) == 0) {
        spdIoT_socket_setaddress(clientSock, localAddr);
    }

    return 0;
}

int spdIoT_socket_connect(spdIoTSocket *sock, const char *addr, int port)
{
    struct addrinfo *toaddrinfo;
    int ret;

    if (spdIoT_socket_tosockaddrinfo(spdIoT_socket_getrawtype(sock),
                                     addr, port, &toaddrinfo, 1) == false) {
        return -1;
    }
    if (!spdIoT_socket_isbound(sock)) {
        spdIoT_socket_setid(sock, socket(toaddrinfo->ai_family, toaddrinfo->ai_socktype, 0));
    }
    ret = connect(sock->id, toaddrinfo->ai_addr, toaddrinfo->ai_addrlen);
    freeaddrinfo(toaddrinfo);

    spdIoT_socket_setdirection(sock, SPDIoT_NET_SOCKET_CLIENT);

#if define(SPDIoT_USE_OPENSSL)
    if (spdIoT_socket_isssl(sock)) {
        sock->ctx = SSL_CTX_new(SSLv23_client_method());
        sock->ssl = SSL_new(sock->ctx);
        if (SSL_set_fd(sock->ssl, spdIoT_socket_getid(sock)) == 0) {
            spdIoT_sock_close(sock);
            return -1;
        }
        if (SSL_connect(sock->ssl) < 1) {
            spdIoT_socket_close(sock);
            return -1;
        }
    }
#endif
    return ret;
}

ssize_t spdIoT_socket_read(spdIoTSocket *sock, char *buffer, size_t bufferlen)
{
    ssize_t recvlen;
#if define(SPDIoT_USE_OPENSSL)
    recvlen = SSL_read(sock->ssl, buffer, bufferlen);
#else
    recvlen = recv(sock->id, buffer, bufferlen, 0);
#endif
    return recvlen;
}

#define UN_NET_SOCKET_SEND_RETRY_CNT 10
size_t spdIoT_socket_write(spdIoTSocket *sock, const char *cmd, size_t cmdLen)
{
    ssize_t nSent;
    size_t nTotalSent = 0;
    size_t cmdPos = 0;
    int retryCnt = 0;

    if (cmdLen <=0 ) {
        return 0;
    }

    do {
#if define(SPDIoT_USE_OPENSSL)
        if (spdIoT_socket_isssl(sock)) {
            nSent = SSL_write(sock->ssl, cmd + cmdPos, cmdLen);
        } else {
            nSent = send(sock->id, cmd + cmdPos, cmdLen, 0);
        }
#else
        nSent = send(sock->id, cmd + cmdPos, cmdLen, 0);
#endif

        if (nSent <= 0) {
            retryCnt++;
            if (SPDIoT_NET_SOCKET_SEND_RETRY_CNT < retryCnt) {
                nTotalSent = 0;
                break;
            }
            usleep(20000);
        } else {
            nTotalSent += nSent;
            cmdPos += nSent;
            cmdLen -= nSent;
            retryCnt = 0;
        }
    }while (0 < cmdLen);

    return nTotalSent;
}

ssize_t spdIoT_socket_readline(spdIoTSocket *sock, char *buffer, size_t bufferLen)
{
    ssize_t readCnt;
    ssize_t readLen;
    char c;

    readCnt = 0;
    while (readCnt < (bufferLen - 1)) {
        readLen = spdIoT_socket_read(sock, &buffer[readCnt], sizeof(char));
        if (readLen <= 0) {
            return -1;
        }
        readCnt++;
        if (buffer[readCnt - 1] == SPDIoT_SOCKET_LF) {
            break;
        }
    }
    buffer[readCnt] = '\0';
    if (buffer[readCnt - 1] != SPDIoT_SOCKET_LF) {
        do {
            readLen = spdIoT_socket_read(sock, &c, sizeof(char));
            if (readLen < 0) {
                break;
            }
        } while (c != SPDIoT_SOCKET_LF);
    }
}

size_t spdIoT_socket_skip(spdIoTSocket *sock, size_t skipLen)
{
    ssize_t readCnt;
    ssize_t readLen;
    char c;

    readCnt = 0;
    while (readCnt < skipLen) {
        readLen = spdIoT_socket_read(sock, &c, sizeof(char));
        if (readLen <= 0) {
            break;
        }
        readCnt++;
    }

    return readCnt;
}

size_t spdIoT_socket_sendto(spdIoTSocket *sock,
                            const char *addr, int port, const char *data, size_t dataLen)
{
    struct addrinfo *addrInfo;
    ssize_t sentLen;
    int isBoundFlag;

    if (NULL == data) {
        return 0;
    }
    if (dataLen <= 0) {
        dataLen = strlen(data);
    }
    if (dataLen <= 0) {
        return 0;
    }

    isBoundFlag = spdIoT_socket_isbound(sock);
    sentLen = -1;

    if (spdIoT_socket_tosockaddrinfo(spdIoT_socket_getrawtype(sock),
                                       addr, port, &addrInfo, 1) < 0) {
        return -1;
    }
    if (!isBoundFlag) {
        spdIoT_socket_setid(sock, socket(addrInfo->ai_family, addrInfo->ai_socktype, 0));
    }

    spdIoT_socket_setmulticastttl(sock, SPDIoT_NET_SOCK_MULTICAST_DEFAULT_TTL);

    if (0 <= sock->id) {
        sentLen = sendto(sock->id, data, dataLen, 0, addrInfo->ai_addr, addrInfo->ai_addrlen);
    }
    freeaddrinfo(addrInfo);

    if (!isBoundFlag) {
        spdIoT_socket_close(sock);
    }

    return sentLen;
}

ssize_t spdIoT_socket_recv(spdIoTSocket *sock, spdIoTDatagramPacket *dgmPkt)
{
    ssize_t recvLen = 0;
    char recvBuf[SPDIoT_NET_SOCKET_DGRAM_RECV_BUFSIZE + 1];
    char remoteAddr[SPDIoT_NET_SOCKET_MAXHOST];
    char remotePort[SPDIoT_NET_SOCKET_MAXSERV];
    char *localAddr;
    struct sockaddr_storage from;
    socklen_t fromLen = sizeof(from);

    recvLen = recvfrom(sock->id, recvBuf, sizeof(recvBuf) - 1, 0, (struct sockaddr*)&from, &fromLen);

    if (recvLen <= 0) {
        return 0;
    }

    recvBuf[recvLen] = '\0';
    spdIoT_socket_datagram_packet_setdata(dgmPkt, recvBuf);
    spdIoT_socket_datagram_packet_setlocalport(dgmPkt, spdIoT_socket_getport(sock));
    spdIoT_socket_datagram_packet_setremoteaddress(dgmPkt, "");
    spdIoT_socket_datagram_packet_setremoteport(dgmPkt, 0);

    if (getnameinfo((struct sockaddr*)&from, fromLen, remoteAddr, sizeof(remoteAddr),
                    remotePort, sizeof(remotePort), NI_NUMERICHOST | NI_NUMERICSERV) == 0) {
        spdIoT_socket_datagram_packet_setremoteaddress(dgmPkt, remoteAddr);
        spdIoT_socket_datagram_packet_setremoteport(dgmPkt, spdIoT_str2int(remotePort));
    }

    localAddr = spdIoT_net_selectaddr((struct sockaddr*)&from);
    spdIoT_socket_datagram_packet_setlocaladdress(dgmPkt, localAddr);
    free(localAddr);

    return recvLen;
}

int spdIoT_socket_setreuseaddress(spdIoTSocket *sock, int flag)
{
    int sockOptRet;
    int optval;

    optval = flag == 1 ? 1: 0;
    sockOptRet = setsockopt(sock->id,
                            SOL_SOCKET, SO_REUSEADDR, (const char *)&optval, sizeof(optval));
    return sockOptRet == 0 ? 0 : -1;
}

int spdIoT_socket_setmulticastttl(spdIoTSocket *sock, int ttl)
{
    int sockOptRet;
    int _ttl;
    unsigned int len = 0;

    sockOptRet = setsockopt(sock->id,
                            IPPROTO_IP, IP_MULTICAST_TTL, (const unsigned char *)&ttl, sizeof(ttl));
    return sockOptRet == 0 ? 0 : -1;
}

int spdIoT_socket_settimeout(spdIoTSocket *sock, int sec)
{
    int sockOptRet;
    struct timeval timeout;
    timeout.tv_sec = (time_t)sec;
    timeout.tv_usec = 0;

    sockOptRet = setsockopt(sock->id,
                            SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
    if (sockOptRet == 0) {
        sockOptRet = setsockopt(sock->id,
                                SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));
    }

    return sockOptRet == 0 ? 0 : -1;
}

int spdIoT_socket_joingroup(spdIoTSocket *sock, const char *mcastAddr, const char *ifAddr)
{
    struct addrinfo hints;
    struct addrinfo *mcastAddrInfo, *ifAddrInfo;

    struct ipv6_mreq ipv6mr;
    struct sockaddr_in6 toaddr6, ifaddr6;
    int scopeID;

    struct ip_mreq ipmr;
    struct sockaddr_in toaddr, ifaddr;

    int joinSuccess;
    int sockOptRetCode;spdIoT_socket_tosockaddrinfo

    memset(&hints, 0, sizeof(hints));
    hints.ai_flags = AI_NUMERICHOST | AI_NUMERICSERV;

    if (getaddrinfo(mcastAddr, NULL, &hints, &mcastAddrInfo) != 0) {
        return -1;
    }

    if (getaddrinfo(ifAddr, NULL, &hints, &ifAddrInfo) != 0) {
        freeaddrinfo(mcastAddrInfo);
        return -1;
    }

    joinSuccess = 0;

    if (spdIoT_net_isipv6address(mcastAddr)) {
        memcpy(&toaddr6, mcastAddrInfo->ai_addr, sizeof(struct sockaddr_in6));
        memcpy(&ifaddr6, ifAddrInfo->ai_addr, sizeof(struct sockaddr_in6));
        ipv6mr.ipv6mr_multiaddr = toaddr6.sin6_addr;
        scopeID = spdIoT_net_getipv6scopeid(ifAddr);
        ipv6mr.ipv6mr_interface = scopeID;

        sockOptRetCode = setsockopt(sock->id,
                                IPPROTO_IPV6, IPV6_MULTICAST_IF, (char *)&scopeID, sizeof(scopeID));
        if (sockOptRetCode != 0) {
            joinSuccess = -1;
        }

        sockOptRetCode = setsockopt(sock->id,
                                IPPROTO_IPV6, IPV6_JOIN_GROUP, (char *)&scopeID, sizeof(scopeID));
        if (sockOptRetCode != 0) {
            joinSuccess = -1;
        }
    } else {
        memcpy(&toaddr, mcastAddrInfo->ai_addr, sizeof(struct sockaddr_in));
        memcpy(&ifaddr, ifAddrInfo->ai_addr, sizeof(struct sockaddr_in));
        memcpy(&ipmr.imr_multiaddr.s_addr, &toaddr.sin_addr, sizeof(struct in_addr));
        memcpy(&ipmr.imr_interface.s_addr, &ifaddr.sin_addr, sizeof(struct in_addr));
        sockOptRetCode =
                setsockopt(sock->id, IPPROTO_IP, IP_MULTICAST_IF,
                           (char *)&ipmr.imr_interface.s_addr, sizeof(struct in_addr));
        if (sockOptRetCode != 0) {
            joinSuccess = -1;
        }

        sockOptRetCode =
                setsockopt(sock->id, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&ipmr, sizeof(ipmr));
        if (sockOptRetCode != 0) {
            joinSuccess = -1;
        }
    }

    freeaddrinfo(mcastAddrInfo);
    freeaddrinfo(ifAddrInfo);

    return joinSuccess;
}

int spdIoT_socket_tosockaddrin(const char *addr, int port,
                             struct sockaddr_in *sockaddr, int isBindAddr)
{
    memset(sockaddr, 0, sizeof(sockaddr));

    sockaddr->sin_family = AF_INET;
    sockaddr->sin_addr.s_addr = htonl(INADDR_ANY);
    sockaddr->sin_port = htons((unsigned short)port);

    if (isBindAddr) {
        sockaddr->sin_addr.s_addr = inet_addr(addr);
        if (sockaddr->sin_addr.s_addr == INADDR_ANY) {
            struct hostent *hent = gethostbyname(addr);
            if (NULL == hent) {
                return -1;
            }
            memcpy(&(sockaddr->sin_addr), hent->h_addr, hent->h_length);
        }
    }

    return 0;
}

int spdIoT_socket_tosockaddrinfo(int sockType, const char *addr,
                                 int port, struct addrinfo **addrInfo, int isBingAddr)
{
    struct addrinfo hints;
    char portStr[32];
    int errorn;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = sockType;
    hints.ai_flags = AI_PASSIVE;
    sprintf(portStr, "%d", port);
    if ((errorn = getaddrinfo(addr, portStr, &hints, addrInfo)) != 0) {
        return -1;
    }

    if (isBingAddr) {
        return 0;
    }

    hints.ai_family = (*addrInfo)->ai_family;
    freeaddrinfo(*addrInfo);
    if ((errorn = getaddrinfo(NULL, portStr, &hints, addrInfo)) != 0) {
        return -1;
    }

    return 0;
}

spdIoTDatagramPacket *spdIoT_socket_datagram_packet_new()
{
    spdIoTDatagramPacket *dgmPkt;

    dgmPkt = (spdIoTDatagramPacket *) calloc(1, sizeof(spdIoTDatagramPacket));
    if (NULL != dgmPkt) {
        dgmPkt->data = spdIoT_string_new();
        dgmPkt->localAddress = spdIoT_string_new();
        dgmPkt->remoteAddress = spdIoT_string_new();

        spdIoT_socket_datagram_packet_setlocalport(dgmPkt, 0);
        spdIoT_socket_datagram_packet_setremoteport(dgmPkt, 0);
    }

    return dgmPkt;
}

void spdIoT_socket_datagram_packet_delete(spdIoTDatagramPacket *dgmPkt)
{
    spdIoT_string_delete(dgmPkt->data);
    spdIoT_string_delete(dgmPkt->localAddress);
    spdIoT_string_delete(dgmPkt->remoteAddress);

    free(dgmPkt);
}

void spdIoT_socket_datagram_packet_copy(spdIoTDatagramPacket *dstDgmPkt, spdIoTDatagramPacket *srcDgmPkt)
{
    spdIoT_socket_datagram_packet_setdata(dstDgmPkt, spdIoT_socket_datagram_packet_getdata(srcDgmPkt));
    spdIoT_socket_datagram_packet_setlocaladdress(
                dstDgmPkt, spdIoT_socket_datagram_packet_getlocaladdress(srcDgmPkt));
    spdIoT_socket_datagram_packet_setlocalport(
                dstDgmPkt, spdIoT_socket_datagram_packet_getlocalport(srcDgmPkt));
    spdIoT_socket_datagram_packet_setremoteaddress(
                dstDgmPkt, spdIoT_socket_datagram_packet_getremoteaddress(srcDgmPkt));
    spdIoT_socket_datagram_packet_setremoteport(
                dstDgmPkt, spdIoT_socket_datagram_packet_getremoteport(srcDgmPkt));
}
