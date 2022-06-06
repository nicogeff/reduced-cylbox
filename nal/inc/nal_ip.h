
#ifndef NAL_IP_H_
#define NAL_IP_H_

#include "nal_common.h"

#if defined NAL_IP_BDS_4_3
#include "nal_ip_bsd_4_3.h"
#else
#error "No stack defined ..."
#endif

#if !defined (NAL_HAVE_IPV4) && !defined (NAL_HAVE_IPV6)
#error "No IPv4 or IPv6 defined ..."
#endif

#define NAL_SOCKADDR_VERSION    1

/** 
 * See RFC3493
 */
/* #define NAL_F0LLOW_RFC3493 */

/**
 * Address string length
 */
#define NAL_INET4_ADDRSTRLEN     16
#define NAL_INET6_ADDRSTRLEN     46

/*
 * Define the default IP version to be used in the function nal_sockaddrInit()
 */
/* #define NAL_PF_INET_IPDFT    AF_INET */ /* Force to use IPv4-Only */
#define NAL_PF_INET_IPDFT       0           /* Use IPv6 if it is supported by the target */

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * \brief IPv4 address structure.
 */
typedef struct nal_in_addr
{
    /**
     * 32 bits IP address, network byte order.s_addr renamed addr to avoid macro conflict on windows 
     */
    uint32_t addr;
} T_nal_in_addr;

/**
 * \brief IPv6 address structure.
 */
typedef struct nal_in6_addr
{
    uint8_t sAddr[16]; /* 128 bits IP address, network byte order */
} T_nal_in6_addr;

/**
 * \brief Socket address usable for any protocol.
 */
typedef struct nal_sockaddr
{
    uint8_t sa_data[NAL_SOCKADDR_STORAGE_SIZE];
} T_nal_sockaddr;

/**
 * \brief Address conversion macros                        *
 */
#if defined (NAL_BIG_ENDIAN)         /* big endian processor definition */
/* Converts a short from the host to the network order. */
#define nal_htons(host)   (unsigned short)(host)
/* Converts a short from the network to the host order. */
#define nal_ntohs(net)    nal_htons(net)
/* Converts a long from the host to the network order. */
#define nal_htonl(host)   (unsigned long)(host)
/* Converts a long from the network to the host order. */
#define nal_ntohl(net)    nal_htonl(net)
#elif defined (NAL_LITTLE_ENDIAN)    /* little endian processor definition */
/* Converts a short from the host to the network order. */
#define nal_htons(host)       \
                           ((((unsigned short)(host) >> 8) & 0x00FF) | \
                            (((unsigned short)(host) << 8) & 0xFF00))
/* Converts a short from the network to the host order. */
#define nal_ntohs(net)    nal_htons(net)
/* Converts a long from the host to the network order. */
#define nal_htonl(host)   ((((unsigned long)(host) >> 24) & 0x000000FF) | \
                            (((unsigned long)(host) >> 8)  & 0x0000FF00) | \
                            (((unsigned long)(host) << 8)  & 0x00FF0000) | \
                            (((unsigned long)(host) << 24) & 0xFF000000))
/* Converts a long from the network to the host order. */
#define nal_ntohl(net)        nal_htonl(net)
#else
#error "NAL_XXX_ENDIAN macro not defined"
#endif

#define NAL_SHUTDOWN_RCV    0   /**< Receiving disallowed */
#define NAL_SHUTDOWN_SND    1   /**< Sending disallowed */
#define NAL_SHUTDOWN_ALL    2   /**< Receiving and sending disallowed */

#ifdef NAL_HAVE_IPV4
typedef uint32_t T_nal_ipnet_addr;
#else /* IPv6 */

typedef struct nal_ipnet_addr
{
    uint16_t netAddrLen; /** 4 for IPv4 , 16 pour IPv6, */

    union
    {
        T_nal_in_addr netAddr4;
        T_nal_in6_addr netAddr6;
    } addr;
} T_nal_ipnet_addr;
#endif


extern int nal_ipInit
(
 void
 );

extern int nal_ipShutdown
(
 void
 );

extern int nal_sockCreate
(
 int domain,
 int type,
 int protocol
 );

extern int nal_sockShutdown
(
 int s,
 int how
 );

extern int nal_sockClose
(
 int s
 );

extern int nal_sockGetSockName
(
 int s,
 T_nal_sockaddr *addr,
 int *addrlen
 );

extern int nal_sockGetPeerName
(
 int s,
 T_nal_sockaddr *addr,
 int *addrlen
 );

extern int nal_sockSetSockOpt
(
 int s,
 int level,
 int optname,
 const void *optval,
 int optlen
 );

extern int nal_sockGetSockOpt
(
 int s,
 int level,
 int optname,
 void *optval,
 int *optlen
 );

extern int nal_sockIoctl
(
 int s,
 int cmd,
 int *arg
 );

extern int nal_sockAccept
(
 int s,
 T_nal_sockaddr *addr,
 int *addrlen
 );

extern int nal_sockBind
(
 int s,
 const T_nal_sockaddr *addr,
 int addrlen
 );

extern int nal_sockConnect
(
 int s,
 const T_nal_sockaddr *addr,
 int addrlen
 );

extern int nal_sockListen
(
 int s,
 int backlog
 );

extern int nal_sockSelect
(
 int nfds,
 nal_fd_set *readfds,
 nal_fd_set *writefds,
 nal_fd_set *exceptfds,
 T_nal_time *timeout
 );

extern int nal_inetNtoa
(
 uint32_t addr,
 char *buffer
 );

extern int nal_inetAton
(
 const char *str,
 uint32_t *addr
 );

extern int nal_inetNtop
(
 int af,
 const void *src,
 char *dst,
 int len
 );

extern int nal_inetPton
(
 int af,
 const char *host,
 void *addr
 );

extern int nal_sockRecv
(
 int s,
 void *buf,
 int len,
 unsigned int flags
 );

extern int nal_sockRead
(
 int s,
 void *buf,
 size_t len
 );

extern int nal_sockRecvFrom
(
 int s,
 void *buf,
 int len,
 unsigned int flags,
 T_nal_sockaddr *from,
 int *fromlen
 );

extern int nal_sockRecvOn
(
 int s,
 void *buf,
 int len,
 unsigned int flags,
 T_nal_sockaddr *from,
 int *fromlen,
 uint32_t *itf
 );

extern int nal_sockSend
(
 int s,
 const void *buf,
 int len,
 unsigned int flags
 );

extern int nal_sockWrite
(
 int s,
 const void *buf,
 size_t len
 );

extern int nal_sockSendTo
(
 int s,
 const void *buf,
 int len,
 unsigned int flags,
 const T_nal_sockaddr *to,
 int tolen
 );

extern int nal_sockaddrCheckFamily
(
 const T_nal_sockaddr *sockAddr
 );

extern int nal_sockaddrInit
(
 int domain,
 T_nal_sockaddr *sockAddr,
 int addrlen
 );

extern int nal_sockaddrGetDomain
(
 const T_nal_sockaddr *sockAddr
 );

extern int nal_sockaddrGetLength
(
 const T_nal_sockaddr *sockAddr
 );

extern int nal_sockaddrGetPort
(
 const T_nal_sockaddr *sockAddr,
 uint16_t *port
 );

extern int nal_sockaddrGetScope
(
 const T_nal_sockaddr *sockAddr,
 uint32_t *scope
 );

extern int nal_sockaddrGetNetAddr
(
 const T_nal_sockaddr *sockAddr,
 T_nal_ipnet_addr *ipNetAddr
 );

extern int nal_sockaddrGetStrAddr
(
 const T_nal_sockaddr *sockAddr,
 char *addrStr,
 int len,
 int scope
 );

extern int nal_sockaddrNetAddrIsLoopback
(
 T_nal_ipnet_addr *ipNetAddr
 );

extern int nal_sockaddrSetPort
(
 T_nal_sockaddr *sockAddr,
 uint16_t port
 );

extern int nal_sockaddrSetScope
(
 T_nal_sockaddr *sockAddr,
 uint32_t scope
 );

extern int nal_sockaddrSetAddrFromAddrStr
(
 T_nal_sockaddr *sockAddr,
 const char *addrStr
 );

extern int nal_sockaddrSetLoopback
(
 T_nal_sockaddr *sockAddr
 );

extern int nal_getHostByName
(
 const char *name,
 T_nal_in_addr *addr
 );

extern int nal_getAddrInfo
(
 int af,
 const char *host,
 uint16_t port,
 T_nal_sockaddr **addrs,
 int *nbAddrs
 );

extern int nal_freeAddrInfo
(
 T_nal_sockaddr *addrs
 );

#ifdef __cplusplus
}
#endif

#endif /* NAL_IP_H_ */
