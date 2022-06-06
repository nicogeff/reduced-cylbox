
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

#include "nal_mem.h"
#include "nal_ip.h"

#ifdef NO_NAL_IP_BSD_4_3
#warning "IP module disable!"
#else

#define MAX(a,b) (a < b ? b : a)

/**
 * \brief Initialize the sockAddr variable with the default values according to the
 * requested IP version and the configuration of the implemented IP stack
 * (IPv4-Only, IPv6-Only or Dual IPv4/IPv6). The IP address will set to the ANY
 * IP Address value.
 * 
 * @param domain Specifies the INET domain requested by the user application.
 * When it is set to ALPF_INET_IPDFT, the abstract layer selects the implemented
 * IP version. For a dual IP stack, it will be IPv6.
 * 
 * @param sockAddr Pointer to a Socket IP Address variable in the user space.
 * 
 * @param addrlen Length of the structure.
 * 
 * @return
 *      NAL_SUCCESS: On success, the function returns the effective byte length 
 * used for the opaque sockAddr structure,\n
 *      NAL_EINVAL: Invalid (NULL) parameters,\n
 *      NAL_EAFNOSUPPORT: Non supported option for parameter domain,\n 
 *      NAL_EINTERNAL: Other internal error).
 */
int nal_sockaddrInit(int domain, T_nal_sockaddr *sockAddrStorage, int sockAddrLen)
{
    int sockaddr_len;
    int result = 0;
#ifdef NAL_HAVE_IPV6
    struct sockaddr_in6* sockaddr_in6;
#endif
#ifdef NAL_HAVE_IPV4
    struct sockaddr_in *sockaddr_in;
#endif

    if (sockAddrStorage == NULL)
    {
        result = NAL_EINVAL;
    }
    else
    {
        /* If IP version is not supported... */
        result = NAL_EAFNOSUPPORT;

#ifdef NAL_HAVE_IPV6
        if ((domain == ALPF_INET_IPDFT) || (domain == AF_INET6) || (domain == AF_INET6))
        {
            /* Initialize IPv6 structure */
            sockaddr_in6 = (struct sockaddr_in6 *) sockAddrStorage;
            sockaddr_len = sizeof ( struct sockaddr_in6);

            if (sockAddrLen < sockaddr_len)
                /* Not enough space into structure */
                result = NAL_EINVAL;
            else
            {
                sockaddr_in6->sin6_family = AF_INET6;
                sockaddr_in6->sin6_port = 0;
                sockaddr_in6->sin6_flowinfo = 0;
                sockaddr_in6->sin6_scope_id = 0;
                sockaddr_in6->sin6_addr = in6addr_any;
                sockaddr_in6->sin6_len = sockaddr_len;
                result = sockaddr_len;
            }
        }
#endif /* NAL_HAVE_IPV6 */

#ifdef NAL_HAVE_IPV4
        if ((domain == NAL_PF_INET_IPDFT) || (domain == AF_INET) || (domain == AF_INET))
        {
            /* Initialize IPv4 structure */
            sockaddr_in = (struct sockaddr_in *) sockAddrStorage;
            sockaddr_len = sizeof ( struct sockaddr_in);

            if (sockAddrLen < sockaddr_len)
                /* Not enough space into structure */
                result = NAL_EINVAL;
            else
            {
                sockaddr_in->sin_family = AF_INET;
                sockaddr_in->sin_port = 0;
                sockaddr_in->sin_addr.s_addr = htonl(INADDR_ANY);
                /* sockaddr_in->sin_len = sockAddrLen; */ /* @todo this field its not present into sockaddr_in of in.h on ubuntu 19.04 */
                result = sockaddr_len;
            }
        }
#endif /* NAL_HAVE_IPV4 */

    }
    return result;
}

/**
 * \brief Returns the domain specified into an Socket IP Address variable.
 * 
 * @param sockAddr Pointer to a Socket IP Address variable.
 * 
 * @return 
 *      NAL_SUCCESS: On success, the function returns the protocol family for 
 * the address: AF_INET: IPv4 address, AF_INET6: IPv6 address.\n
 *      NAL_EINVAL : Invalid (NULL) sockAddr parameter,\n
 *      NAL_EAFNOSUPPORT: Invalid domain.
 */
int nal_sockaddrGetDomain(const T_nal_sockaddr *sockAddr)
{
    int result = 0;
#ifdef NAL_HAVE_IPV4
    struct sockaddr_in *addr = NULL;
#else /* IPv6 */
    struct sockaddr_in6 *addr = NULL;
#endif

#ifdef NAL_HAVE_IPV4
    addr = (struct sockaddr_in *) sockAddr;
#else /* IPv6 */
    addr = (struct sockaddr_in6 *) sockAddr;
#endif 

    if (sockAddr == NULL)
    {
        result = NAL_EINVAL;
    }
    else
    {
        result = NAL_EAFNOSUPPORT;

        if (addr->sin_family == AF_INET)
        {
            result = AF_INET;
        }
#ifdef NAL_HAVE_IPV6
        else if (addr->sin_family == AF_INET6)
        {
            result = AF_INET6;
        }
#endif
    }

    return result;
}

/**
 * \brief Retrieves the length of the Socket IP Address variable  according to the used
 * domain (IPv4 or IPv6).
 * 
 * @param sockAddr Pointer to a Socket IP Address variable.
 * 
 * @return 
 *      NAL_SUCCESS: On success, the function returns the effective byte length used
 * for the opaque sockAddr structure,\n
 *      NAl_EINVAL: Invalid (NULL) sockAddr parameter,\n
 *      NAL_EAFNOTSUPPORT: Invalid domain,\n
 *      NAL_EINTERNAL: Other internal error.
 */
int nal_sockaddrGetLength(const T_nal_sockaddr *sockAddr)
{
    struct sockaddr_in *addr = NULL;
    int result = 0;

    addr = (struct sockaddr_in *) sockAddr;

    if (sockAddr == NULL)
    {
        result = NAL_EINVAL;
    }
    else
    {
        result = NAL_EAFNOSUPPORT;

        if (addr->sin_family == AF_INET)
        {
            result = sizeof ( struct sockaddr_in);
        }
#ifdef NAL_HAVE_IPV6
        else if (addr->sin_family == AF_INET6)
        {
            result = sizeof ( struct sockaddr_in6);
        }
#endif
    }

    return result;
}

/**
 * \brief Retrieves the port number from the Socket IP Address variable.
 * 
 * @param sockAddr Pointer to a Socket IP Address variable.
 * 
 * @param port Pointer to a variable that will be used to return the port number.
 * 
 * @return
 *      NAL_SUCCESS: On success.\n
 *      NAL_EINVAL: Invalid (NULL) sockAddr or port parameter,\n
 *      NAL_EINTERNAL: Other internal error,\n
 *      NAL_EAFNOSUPPORT: Invalid domain.
 */
int nal_sockaddrGetPort(const T_nal_sockaddr* sockAddrStorage, uint16_t* port)
{
    int rc = 0;
    int result = 0;

    if ((sockAddrStorage == NULL) || (port == NULL))
    {
        result = NAL_EINVAL;
    }
    else
    {
        rc = nal_sockaddrGetDomain(sockAddrStorage);

        if ((rc != AF_INET) && (rc != AF_INET6))
        {
            /* Return error in nal_sockaddrGetDomain() */
            result = rc;
        }
        else
        {
            result = NAL_EAFNOSUPPORT;

            if (rc == AF_INET6)
            {
#ifdef NAL_HAVE_IPV6
                *port = ntohs(((struct sockaddr_in6 *) sockAddrStorage)->sin6_port);
                result = NAL_SUCCESS;
#endif
            }
            if (rc == AF_INET)
            {
#ifdef NAL_HAVE_IPV4
                *port = ntohs(((struct sockaddr_in *) sockAddrStorage)->sin_port);
                result = NAL_SUCCESS;
#endif
            }
        }
    }

    return result;
}

/**
 * \brief Retrieves the scope identifier from the Socket IP Address variable.
 * 
 * @param sockAddr Pointer to a Socket IP Address variable.
 * 
 * @param scope Pointer to a variable that will be used to return the scope
 * identifier.
 * 
 * @return 
 *      NAL_SUCCESS: On success,\n
 *      NAL_EINVAL: Invalid (NULL) sockAddr or scope parameter,\n
 *      NAL_EAFNOSUPPORT: sockAddr point to an IPv4 address,\n
 *      NAL_EINTERNAL: Other internal error.
 */
int nal_sockaddrGetScope(const T_nal_sockaddr *sockAddrStorage, uint32_t *scope)
{
    int rc = 0;
    int result = 0;

    if ((sockAddrStorage == NULL) || (scope == NULL))
    {
        result = NAL_EINVAL;
    }
    else
    {
        rc = nal_sockaddrGetDomain(sockAddrStorage);
        if ((rc != AF_INET) && (rc != AF_INET6))
        {
            /* Return error in nal_sockaddrGetDomain() */
            result = rc;
        }
        else
        {
            if (rc == AF_INET6)
            {
#ifdef NAL_HAVE_IPV6
                *scope = ((struct sockaddr_in6 *) sockAddrStorage)->sin6_scope_id;
                result = NAL_SUCCESS;
#else
                result = NAL_EAFNOSUPPORT;
#endif
            }
            else
            {
                result = NAL_EAFNOSUPPORT;
            }
        }
    }

    return result;
}

/**
 * \brief Retrieves the IP Address (in network format) from the Socket IP Address
 * variable.
 * 
 * @param sockAddr Pointer to a Socket IP Address variable.
 * 
 * @param[out] ipNetAddr Pointer to a variable that will be used to return the
 * IP address in network format.
 * 
 * @return 
 *      NAL_SUCCESS: On success,\n
 *      NAL_EINVAL: Invalid (NULL) sockAddr or ipNetAddr parameter,\n
 *      NAL_EAFNOSUPPORT: sockAddr is an IPv6 (or v4) address, and this 
 * version is not supported,\n
 *      NAL_EINTERNAL: Other internal error.
 */
int nal_sockaddrGetNetAddr(const T_nal_sockaddr *sockAddrStorage, T_nal_ipnet_addr *ipNetAddr)
{
#ifdef NAL_HAVE_IPV6
    struct sockaddr_in6 * sockaddr_in6;
#endif
#ifdef NAL_HAVE_IPV4
    struct sockaddr_in * sockaddr_in;
#endif
    int rc = 0;
    int result = 0;

    if ((sockAddrStorage == NULL) || (ipNetAddr == NULL))
    {
        result = NAL_EINVAL;
    }
    else
    {
        rc = nal_sockaddrGetDomain(sockAddrStorage);
        if ((rc != AF_INET) && (rc != AF_INET6))
        {
            /* Return error in nal_sockaddrGetDomain() */
            result = rc;
        }
        else
        {
            result = NAL_EAFNOSUPPORT;

            if (rc == AF_INET6)
            {
#ifdef NAL_HAVE_IPV6
                sockaddr_in6 = (struct sockaddr_in6 *) sockAddrStorage;
                ipNetAddr->netAddrLen = 16;
                memcpy(&ipNetAddr->addr.netAddr6, &sockaddr_in6->sin6_addr, sizeof ( struct in6_addr));
                result = NAL_SUCCESS;
#endif /* NAL_HAVE_IPV6 */
            }
            if (rc == AF_INET)
            {
#ifdef NAL_HAVE_IPV4
                sockaddr_in = (struct sockaddr_in *) sockAddrStorage;
#ifdef NAL_HAVE_IPV6
                ipNetAddr->netAddrLen = 4;
                memcpy(&ipNetAddr->addr.netAddr4, &sockaddr_in->sin_addr, sizeof ( struct in_addr));
#else
                memcpy(ipNetAddr, &sockaddr_in->sin_addr, sizeof ( struct in_addr));
#endif /* NAL_HAVE_IPV6 */
                result = NAL_SUCCESS;
#endif /* NAL_HAVE_IPV4 */
            }
        }
    }
    return result;
}

/**
 * \brief Retrieves the IP Address from the Socket IP Address variable in a string with
 * the standard text presentation format.
 * 
 * @param sockAddrStorage Pointer to a Socket IP Address variable.
 * 
 * @param addrStr Pointer to a buffer that will be used to return the IP
 * address in dot format. The size should be AL_INET6_ADDRSTRLEN at least long.
 * 
 * @param len The length of the \a addrStr buffer.
 * 
 * @param scope This boolean specifies if the scope identifier must be appended
 * to the string address (for link-local addresses only).
 * 
 * @return 
 *      NAL_SUCCESS: On success,\n
 *      NAL_EINVAL: Invalid (NULL) sockAddr or addrStr parameter,\n
 *      NAL_EAFNOSUPPORT: sockAddr is an IPv6 (or v4) address, and this 
 * version is not supported,\n
 *      NAL_ENOSPC: addrStr buffer not big enough,\n
 *      NAL_EINTERNAL: Other internal error.
 */
int nal_sockaddrGetStrAddr(const T_nal_sockaddr *sockAddrStorage, char *addrStr, int len, int scope)
{
#ifdef NAL_HAVE_IPV6
    const struct sockaddr_in6* sockaddr_in6;
    int result_ntop = 0;
#endif
#ifdef NAL_HAVE_IPV4
    const struct sockaddr_in* sockaddr_in;
#endif
    int result = 0;
    int rc = 0;

    if ((sockAddrStorage == NULL) || (addrStr == NULL))
    {
        result = NAL_EINVAL;
    }
    else
    {
        rc = nal_sockaddrGetDomain(sockAddrStorage);

        if ((rc != AF_INET) && (rc != AF_INET6))
        {
            /* Return error in nal_sockaddrGetDomain() */
            result = rc;
        }
        else
        {
            result = NAL_EAFNOSUPPORT;

            if (rc == AF_INET6)
            {
#ifdef NAL_HAVE_IPV6
                sockaddr_in6 = (const struct sockaddr_in6*) sockAddrStorage;
                result_ntop = nal_inetNtop(sockaddr_in6->sin6_family, (const char*) &sockaddr_in6->sin6_addr, addrStr, len);
                if (scope && (result_ntop == NAL_SUCCESS) && IN6_IS_ADDR_LINKLOCAL(&sockaddr_in6->sin6_addr))
                {
                    for (; *addrStr; addrStr++, len--);
                    snprintf(addrStr, len, "%%%"PRIu32, sockaddr_in6->sin6_scope_id);
                    result = NAL_SUCCESS;
                }
                else
                {
                    result = result_ntop;
                }
#endif /* NAL_HAVE_IPV6 */
            }
            else
            {
#ifdef NAL_HAVE_IPV4
                sockaddr_in = (const struct sockaddr_in*) sockAddrStorage;
                result = nal_inetNtop(sockaddr_in->sin_family, (const char*) &sockaddr_in->sin_addr, addrStr, len);
#endif /* NAL_HAVE_IPV4 */
            }
        }
    }
    return result;
}

/**
 * \brief Sets the port number in the Socket IP Address variable.
 * 
 * @warning Before calling this function, the variable must be initialized by
 * al_sockaddr_init() function.
 * 
 * @param sockAddrStorage Pointer to a Socket IP Address variable that has been already
 * initialized.
 * 
 * @param port Port number to set.
 * 
 * @return
 *      NAL_SUCCESS : On success, port set,\n
 *      NAL_EINVAL : Invalid (NULL) value for sockAddr parameter,\n
 *      NAL_EAFNOSUPPORT : sockAddr is an IPv6 (or v4) address, and this 
 * version is not supported,\n
 *      NAL_EINTERNAL : Other internal error.
 */
int nal_sockaddrSetPort(T_nal_sockaddr *sockAddrStorage, uint16_t port)
{
    int rc = 0;
    int result = 0;
#ifdef NAL_HAVE_IPV6
    struct sockaddr_in6 *_sockaddr_in6 = NULL;
#endif
    
    if (sockAddrStorage == NULL)
    {
        result = NAL_EINVAL;
    }
    else
    {
        rc = nal_sockaddrGetDomain(sockAddrStorage);
        if ((rc != AF_INET) && (rc != AF_INET6))
        {
            /* Return error in nal_sockaddrGetDomain() */
            result = rc;
        }
        else
        {
            result = NAL_EAFNOSUPPORT;

            if (rc == AF_INET6)
            {
#ifdef NAL_HAVE_IPV6
                _sockaddr_in6 = (struct sockaddr_in6 *) sockAddrStorage;
                _sockaddr_in6->sin6_port = htons(port);
                /* ( ( struct sockaddr_in6 * )sockAddrStorage )->sin6_port = htons ( port ); */
                result = NAL_SUCCESS;
#endif /* NAL_HAVE_IPV6 */
            }
            if (rc == AF_INET)
            {
#ifdef NAL_HAVE_IPV4
                ((struct sockaddr_in *) sockAddrStorage)->sin_port = htons(port);
                result = NAL_SUCCESS;
#endif /* NAL_HAVE_IPV4 */
            }
        }
    }
    return result;
}

/**
 * \brief Sets the scope identifier in the Socket IP Address variable.
 * 
 * @warning Before calling this function, the variable must be initialized by
 * nal_sockaddrInit() function.
 * 
 * @param sockAddr Pointer to a Socket IP Address variable that has been already
 * initialized.
 * 
 * @param scope Scope number to set.
 * 
 * @return 
 *      NAL_SUCCESS: On success,\n
 *      NAL_EINVAL : Invalid (NULL) value for sockAddr parameter,\n
 *      NAL_EAFNOSUPPORT : sockAddr is an IPv6 (or v4) address, and this version is not supported,\n
 *      NAL_EINTERNAL : Other internal error.
 */
int nal_sockaddrSetScope(T_nal_sockaddr * sockAddrStorage, uint32_t scope)
{
    int rc = 0;
    int result = 0;

    if (sockAddrStorage == NULL)
    {
        result = NAL_EINVAL;
    }
    else
    {
        rc = nal_sockaddrGetDomain(sockAddrStorage);
        if ((rc != AF_INET) && (rc != AF_INET6))
        {
            /* Return error in nal_sockaddrGetDomain() */
            result = rc;
        }
        else
        {
            if (rc == AF_INET6)
            {
#ifdef NAL_HAVE_IPV6
                ((struct sockaddr_in6 *) sockAddrStorage)->sin6_scope_id = scope;
                result = NAL_SUCCESS;
#endif
            }
            else
            {
                /* IPv4 addresses doesn't have scope */
                result = NAL_EAFNOSUPPORT;
            }
        }
    }
    return result;
}

/**
 * \brief Sets the IP Address in the Socket IP Address variable from the address name
 * in the standard text presentation format.
 * 
 * @param sockAddrStorage Pointer to a Socket IP Address variable that has been already
 * initialized.
 * 
 * @param addrStr Pointer to a character string specifying the IP address. It can
 * contain the scope identifier (%n) that is parsed contrary to nal_inetNtop().
 * 
 * @return 
 *      NAL_SUCCESS: On success,\n
 *      NAL_EINVAL: Invalid (NULL) value for sockAddr parameter,\n
 *      NAL_EAFNOSUPPORT: sockAddr is an IPv6 (or v4) address, and this version is not supported,\n
 *      NAL_EINTERNAL: Other internal error,\n
 *      AL_EFAULT: addrStr point to a valid string, containing an invalid address.
 */
int nal_sockaddrSetAddrFromAddrStr(T_nal_sockaddr *sockAddrStorage, const char *addrStr)
{
#ifdef NAL_HAVE_IPV6
    struct sockaddr_in6* sockaddr_in6;
    int i;
    char tmpbuf [ AL_INET6_ADDRSTRLEN ];
#endif
#ifdef NAL_HAVE_IPV4
    struct sockaddr_in* sockaddr_in;
#endif
    int result = 0;
    int rc = 0;

    if ((sockAddrStorage == NULL) || (addrStr == NULL))
    {
        result = NAL_EINVAL;
    }
    else
    {
        rc = nal_sockaddrGetDomain(sockAddrStorage);
        if ((rc != AF_INET) && (rc != AF_INET6))
        {
            /* Return error in nal_sockaddrGetDomain() */
            result = rc;
        }
        else
        {
            result = NAL_EAFNOSUPPORT;

            if (rc == AF_INET6)
            {
#ifdef NAL_HAVE_IPV6
                sockaddr_in6 = (struct sockaddr_in6*) sockAddrStorage;

                /* copy until potential % */
                for (i = 0; i < AL_INET6_ADDRSTRLEN && addrStr [ i ]; i++)
                {
                    if (addrStr [ i ] == '%')
                    {
                        sockaddr_in6->sin6_scope_id = atoi((const char *) (addrStr + i + 1));
                        break;
                    }
                    tmpbuf [ i ] = addrStr [ i ];
                }
                tmpbuf [ i ] = 0;

                result = nal_inetPton(sockaddr_in6->sin6_family, tmpbuf, (char*) &sockaddr_in6->sin6_addr);
#endif /* NAL_HAVE_IPV6 */
            }
            if (rc == AF_INET)
            {
#ifdef NAL_HAVE_IPV4
                sockaddr_in = (struct sockaddr_in*) sockAddrStorage;
                result = nal_inetPton(sockaddr_in->sin_family, addrStr, (char*) &sockaddr_in->sin_addr);
#endif /* NAL_HAVE_IPV4 */
            }
        }
    }
    return result;
}

/**
 * \brief Set the IP Loopback address in the Socket IP Address variable according to
 * the IP domain specified by this variable (IPv4: 127.0.0.1, IPv6: ::0).
 * 
 * @param sockAddrStorage Pointer to a Socket IP Address variable that has been already
 * initialized.
 * 
 * @return
 *      NAL_SUCCESS On success,\n
 *      NAL_EINVAL : Invalid (NULL) value for sockAddr parameter,\n
 *      NAL_EAFNOSUPPORT : sockAddr is an IPv6 (or v4) address, and this version is not supported,\n
 *      NAL_EINTERNAL : Other internal error.
 */
int nal_sockaddrSetLoopback(T_nal_sockaddr *sockAddrStorage)
{
#ifdef NAL_HAVE_IPV6
    struct sockaddr_in6 * sockaddr_in6;
#endif
#ifdef NAL_HAVE_IPV4
    struct sockaddr_in * sockaddr_in;
#endif
    int rc = 0;
    int result = 0;

    if (sockAddrStorage == NULL)
    {
        result = NAL_EINVAL;
    }
    else
    {
        rc = nal_sockaddrGetDomain(sockAddrStorage);
        if ((rc != AF_INET) && (rc != AF_INET6))
        {
            /* Return error in nal_sockaddrGetDomain() */
            result = rc;
        }
        else
        {
            result = NAL_EAFNOSUPPORT;

            if (rc == AF_INET6)
            {
#ifdef NAL_HAVE_IPV6
                sockaddr_in6 = (struct sockaddr_in6 *) sockAddrStorage;
                sockaddr_in6->sin6_addr = in6addr_loopback;
                result = NAL_SUCCESS;
#endif /* NAL_HAVE_IPV6 */
            }
            if (rc == AF_INET)
            {
#ifdef NAL_HAVE_IPV4
                sockaddr_in = (struct sockaddr_in *) sockAddrStorage;
                sockaddr_in->sin_addr.s_addr = htonl(INADDR_LOOPBACK);
                result = NAL_SUCCESS;
#endif /* NAL_HAVE_IPV4 */
            }
        }
    }
    return result;
}

/**
 * \brief Test if an IP address is the loopback.
 * 
 * @param ipNetAddr Address to test in network format
 * 
 * @return 
 *      A boolean value: 1 = Loopback address, 0 = Non loopback address,\n
 *      NAL_EINVAL: Invalid (NULL) value for ipNetAddr parameter,\n
 *      NAL_EAFNOSUPPORT: sockAddr is an IPv6 (or v4) address, and this version is not supported,\n
 *      NAL_EINTERNAL: Other internal error.
 */
int nal_sockaddrNetAddrIsLoopback(T_nal_ipnet_addr *ipNetAddr)
{
    int result = 0;
    int netLoopBackIpv4 = 0;
#ifdef NAL_HAVE_IPV6
    int i = 0;
#endif

    if (ipNetAddr == NULL)
    {
        result = NAL_EINVAL;
    }
    else
    {
#ifdef NAL_HAVE_IPV6
        if (ipNetAddr->netAddrLen == 4)
        {
            /* IPv4 address */
            netLoopBackIpv4 = htonl(INADDR_LOOPBACK);
            if (ipNetAddr->addr.netAddr4.addr == netLoopBackIpv4)
            {
                result = 1;
            }
            else
            {
                result = 0;
            }
        }
        else if (ipNetAddr->netAddrLen == 16)
        {
            /* IPv6 address */
            result = 1;
            for (i = 0; i < 16; i++)
            {
                if (ipNetAddr->addr.netAddr6.sAddr[i] != in6addr_loopback.s6_addr[i])
                    result = 0;
            }
        }
#else
        netLoopBackIpv4 = nal_htonl(INADDR_LOOPBACK);
        if (*ipNetAddr == netLoopBackIpv4)
        {
            result = 1;
        }
        else
        {
            result = 0;
        }
#endif  /* NAL_HAVE_IPV6 */
    }

    return result;
}

/**
 * \brief Simplified version of getaddrinfo that allows T_nal_sockaddr structure
 * construction whatever is the format of the supplied address (name or IP
 * address, V4 or V6).It replaces nal_getHostByName() which is obsolete for IPV6.
 * 
 * @param af The address family (AF_INET, AF_INET6).
 * 
 * @param host IP address or host name for which socket addresses must be
 * retrieved. If host is an IPv6 address, it may contain a scope suffix for
 * interface specification using "%<netif index>".
 * 
 * @param port The port that should be set in socket addresses.
 * 
 * @param[out] addrs A array of returned socket address structures allocated
 * by the API. It should be freed by the caller using \a nal_freeAddrInfo().
 * 
 * @param[out] nbAddr The number of entries of the return array.
 * 
 * @return 
 *      NAL_SUCCESS: On success,\n
 *      NAL_SUCCESS: Call successful, host resolved,\n
 *      NAl_EINVAL: Invalid value (NULL) for parameters host, addrs, or nbAddrs,\n
 *      NAL_EAGAIN: temporary failure in name resolution,\n
 *      NAL_EAFNOSUPPORT: Address family (IPv4 / IPv6) in af parameter not supported,\n
 *      NAL_ENOMEM: Not enough memory to store the results,\n
 *      NAL_EINTERNAL: Other internal error.
 */
int nal_getAddrInfo(int af, const char *host, uint16_t port, T_nal_sockaddr **addrs, int *nbAddrs)
{
    T_nal_sockaddr sa;
    int result = 0;
    int i = 0;
    struct addrinfo * resList, hints, *wResult;
    char *scopeStart;
    int addrScope = 0;

    if ((host == NULL) || (addrs == NULL) || (nbAddrs == NULL))
    {
        result = NAL_EINVAL;
    }
    else
    {
        *addrs = NULL;
        result = nal_sockaddrInit(af, &sa, sizeof ( T_nal_sockaddr));
        if (result >= 0)
        {
            /*
             * Workaround for IPv6 addresses
             * getaddrinfo on darwin does not manage the scope of an IPv6 interface by it's number
             * but by it's name. x:x:x:x:x:x%6 means nothing, x:x:x:x:x:x%en0 works.
             */
            addrScope = -1;
            scopeStart = strchr(host, '%');
            if (scopeStart != NULL)
            {
                scopeStart++;
                addrScope = strtod(scopeStart, NULL);
            }

            i = 0;
            memset(&hints, 0, sizeof (struct addrinfo));
            hints.ai_family = af;
            hints.ai_flags = AI_V4MAPPED | AI_ALL; /*retrieving both IPv6 and IPv4-mapped IPv6 addresses if af = AF_INET6 */
            hints.ai_socktype = 0; /* To retrieve only one which is correct since we don't consider the 'service' param */
            hints.ai_protocol = 0;

            result = getaddrinfo(host, NULL, &hints, &resList);
            if (result != 0)
            {
                switch (result)
                {
                case EAI_AGAIN:
                    result = NAL_EAGAIN;
                    break;

                case EAI_FAMILY:
                    result = NAL_EAFNOSUPPORT;
                    break;

                case EAI_MEMORY:
                    result = NAL_ENOMEM;
                    break;

                default:
                    result = NAL_EINTERNAL;
                    break;
                }
            }
            else
            {
                /* Count results */
                *nbAddrs = 0;
                for (wResult = resList; wResult; wResult = wResult->ai_next)
                    (*nbAddrs)++;

                *addrs = NAL_MALLOC(*nbAddrs * sizeof ( T_nal_sockaddr));

                for (wResult = resList; wResult; wResult = wResult->ai_next)
                {
                    memcpy((*addrs) + i, wResult->ai_addr, wResult->ai_addrlen);
                    nal_sockaddrSetPort((*addrs) + i, port); /* Could be done using the "service" param is the string form was available */
                    if (addrScope > -1)
                        nal_sockaddrSetScope((*addrs) + i, addrScope);
                    i++;
                }
                freeaddrinfo(resList);
                result = NAL_SUCCESS;
            }
        }
    }
    return result;
}

/**
 * \brief Release the memory allocated by al_getaddrinfo
 * 
 * @param[in] addrs The array of socket address structures returned by nal_getAddrInfo.
 * 
 * @return 
 *      NAL_SUCCESS: On success,\n
 *      NAL_EINVAL : Invalid value (NULL) for parameter addrs,\n
 *      NAL_EINTERNAL : Other internal error.
 */
int nal_freeAddrInfo(T_nal_sockaddr *addrs)
{
    int result = 0;

    if (addrs == NULL)
    {
        result = NAL_EINVAL;
    }
    else
    {
        NAL_FREE(addrs);
        result = NAL_SUCCESS;
    }
    return result;
}

/**
 * \brief Initialize the IP stack.
 * 
 * @note This function does nothing for Linux.
 * 
 * @warning Must be called before any other call to NAL IP APIs. Was introduced mainly for
 * WSAStartup support.
 * 
 * @return
 *      NAL_SUCCESS: On success.
 */
int nal_ipInit(void)
{
    return NAL_SUCCESS;
}

/**
 * \brief Shutdowns the IP stack.
 * 
 * @note This function does nothing for Linux
 * 
 * @warning Must be called when no other call to NAL IP APIs is required. Was 
 * introduced mainly for WSACleanup support.
 * 
 * @return
 *      NAL_SUCCESS: On success
 */
int nal_ipShutdown(void)
{
    return NAL_SUCCESS;
}

/**
 * \brief This function converts a 32-bits internet address into a string in 
 * standard dot notation.
 * 
 * @param addr An IP host address in the network byte order.
 * 
 * @param[out] buffer A buffer for receiving the string address. Should be 16
 * byte long at least.
 * 
 * @return 
 *      NAL_SUCCESS: On success,\n
 *      NAL_EINVAL: Invalid (NULL) buffer parameter,\n
 *      NAL_EINTERNAL: Other internal error.
 */
int nal_inetNtoa(uint32_t addr, char *buffer)
{
    struct in_addr inaddr;
    char * s = NULL;
    int result = 0;

    if (buffer == NULL)
    {
        result = NAL_EINVAL;
    }
    else
    {
        inaddr.s_addr = addr;
        s = inet_ntoa(inaddr);
        if (s)
        {
            strcpy(buffer, s);
            result = NAL_SUCCESS;
        }
        else
            result = NAL_EINTERNAL;
    }
    return result;
}

/**
 * \brief This function converts an internet address in standard dot notation into
 * a 32-bits internet address.
 * 
 * @param str Internet address in the numbers-and-dots notation.
 * 
 * @param[out] addr A buffer that will receive the IP host address in the
 * network byte order.
 * 
 * @return 
 *      NAL_SUCCESS: On success,\n
 *      NAL_EINVAL: Invalid (NULL) buffer parameter,\n
 *      NAL_EINTERNAL: Other internal error.
 */
int nal_inetAton(const char *str, uint32_t *addr)
{
    int result = 0;

    if ((str == NULL) || (addr == NULL))
    {
        result = NAL_EINVAL;
    }
    else
    {
        *addr = inet_addr(str);

        if (*addr == INADDR_NONE)
            result = NAL_EINVAL;
        else
            result = NAL_SUCCESS;
    }
    return result;
}

/**
 * \brief Convert a printable characters string to a network address
 * according to the required domain specified by af.
 * 
 * @param af The address family.
 * 
 * @param host The address in the string format.
 * 
 * @param[in,out] addr A pointer to a structure that will receive the binary
 * network address.
 * 
 * @return 
 *      NAL_SUCCESS: On success,\n
 *      NAL_EINVAL: Invalid (NULL) buffer parameter,\n
 *      NAL_EINTERNAL: Other internal error,\n
 *      NAL_EFAULT: ?
 */
int nal_inetPton(int af, const char *host, void* addr)
{
    int RetPton;
    int result = 0;

    if ((host == NULL) || (addr == NULL))
    {
        result = NAL_EINVAL;
    }
    else
    {

        RetPton = inet_pton(af, host, addr);

        if (RetPton < 0)
        {
            switch (errno)
            {
            case EAFNOSUPPORT:
                result = NAL_EAFNOSUPPORT;
                break;

            default:
                result = NAL_EINTERNAL;
                break;
            }
        }
        else
        {
            if (0 == RetPton)
            {
                result = NAL_EFAULT;
            }
            else
            {
                result = NAL_SUCCESS;
            }
        }
    }

    return result;
}

/**
 * \brief Convert a Network address to Printable characters string according 
 * to the required domain specified by af.
 * 
 * @param af The address family.
 * 
 * @param src A pointer to a network address structure.
 * 
 * @param dst A buffer to receive the printable address.
 * 
 * @param len The size of the dst buffer.
 * 
 * @return 
 *      NAL_SUCCESS: On success,\n
 *      NAL_EINVAL: Invalid (NULL) src or dst parameter,\n
 *      NAL_EAFNOSUPPORT: af option not supported,\n
 *      NAL_ENOSPC: dst buffer not big enough,\n
 *      NAL_EINTERNAL: Other internal error,\n
 *      NAL_EIMPL: Function not implemented.
 */
int nal_inetNtop(int af, const void* src, char *dst, int len)
{
    const char * RetNtop = NULL;
    int result = 0;

    if ((src == NULL) || (dst == NULL))
    {
        return NAL_EINVAL;
    }
    else
    {
        RetNtop = inet_ntop(af, src, dst, len);
        if (RetNtop == NULL)
        {
            switch (errno)
            {
            case ENOSPC:
            case ENOMEM:
                result = NAL_ENOSPC;
                break;

            case EAFNOSUPPORT:
                result = NAL_EAFNOSUPPORT;
                break;

            default:
                result = NAL_EINTERNAL;
                break;
            }
        }
        else
        {
            result = NAL_SUCCESS;
        }
    }
    return result;
}

/**
 * \brief Create a new socket.
 * 
 * @detail This function creates a new socket in the specified protocol domain and
 * returns a descriptor for it that can be used by other socket routines.
 * 
 * @param domain BSD domains macros should be used. Only AF_INET (IPv4) is
 * required to be supported by the implementation.
 * 
 * @param type BSD domains macros should be used. SOCK_STREAM for TCP,
 * SOCK_DGRAM for UDP. Other types may be supported since parameters should be
 * passed directly to the underlying BSD implementation.
 * 
 * @param protocol Protocol number. Generally ignored for the AF_INET family.
 * 
 * @return 
 *      A socket descriptor ( >= 0 ) on success,\n
 *      NAL_EPROTONOSUPPORT : protocol option not supported,\n
 *      NAL_ESOCKTNOSUPPORT : type option not supported,\n
 *      NAL_EINVAL : domain option not supported,\n
 *      NAL_EMFILE : Descriptor table full,\n
 *      NAL_EACCES : Permission to create a socket of the specified type and/or 
 * protocol is denied,\n
 *      NAL_EINTERNAL : Other internal error.
 */
int nal_sockCreate(int domain, int type, int protocol)
{
#ifdef NAL_HAVE_IPV6
    int on = 1;
    int SetsockoptResult = 0;
#endif
    int s = 0;
    int error = 0;
    int result = 0;

    /* Create the socket */
    s = socket(domain, type, protocol);

    /* Save error */
    if (s < 0)
        error = errno;
    else
        error = 0;

    /* By default under Linux IPV6 sockets receive also IPv4. Disable for uniform behavior */
#ifdef NAL_HAVE_IPV6
    if ((s >= 0) && (domain == AF_INET6))
    {
        SetsockoptResult = setsockopt(s, IPPROTO_IPV6, IPV6_V6ONLY, &on, sizeof ( on));
        if (SetsockoptResult == -1)
        {
            error = errno;
            close(s);
        }
    }
#endif

    /* Returned value management */
    switch (error)
    {
    case 0:
        result = s;
        break;
    case EAFNOSUPPORT:
        result = NAL_EAFNOSUPPORT;
        break;
    case EMFILE:
        result = NAL_EMFILE;
        break;
    case EACCES:
        result = NAL_EACCES;
        break;
    case EPROTONOSUPPORT:
        result = NAL_EPROTONOSUPPORT;
        break;
    default:
        result = NAL_EINTERNAL;
        break;
    }

    return result;
}

/**
 * \brief This function causes all or part of a full-duplex connection on the 
 * socket to be shut down.
 * 
 * @param s Socket descriptor to shutdown.
 * 
 * @param how. One of:\n
 *      \a NAL_SHUTDOWN_RCV (reception),\n 
 *      \a NAL_SHUTDOWN_SND (emission),\n
 *      \a NAL_SHUTDOWN_ALL (both).
 * 
 * @return 
 *      NAL_SUCCESS: Call successful,\n
 *      NAL_EBADF: s parameter is not a valid socket descriptor,\n
 *      NAL_EINVAL: Invalid value for parameter how,\n
 *      NAL_ENOTCONN: Socket already disconnected (not supported),\n
 *      NAL_ESOCKTNOSUPPORT: Socket s type not supported for this operation,\n
 *      NAL_EINTERNAL: Other internal error.
 */
int nal_sockShutdown(int s, int how)
{
    int returnValue = NAL_SUCCESS;
    int error = 0;
    int result = 0;

    result = shutdown(s, how);

    /* Retrieve error if necessary */
    if (result)
    {
        error = errno;
    }
    else
    {
        error = 0;
    }

    /* Returned value management */
    switch (error)
    {
    case 0:
        returnValue = NAL_SUCCESS;
        break;

    case EBADF:
    case ENOTSOCK:
        returnValue = NAL_EBADF;
        break;

    case ENOTCONN:
        returnValue = NAL_ENOTCONN;
        break;

    default:
        returnValue = NAL_EINTERNAL;
        break;
    }

    return returnValue;
}

/**
 * \brief Close a socket and delete associated data.
 * 
 * @param s Socket descriptor to close.
 * 
 * @return 
 *      NAL_SUCCESS if no error occurred,\n
 *      NAL_EBADF: s parameter is not a valid socket descriptor,\n
 *      NAL_EINTERNAL: Other internal error. 
 */
int nal_sockClose(int s)
{
    int returnValue = 0;
    int result = 0;

    result = close(s);

    /* Retrieve error if necessary */
    if (result == 0)
    {
        returnValue = NAL_SUCCESS;
    }
    else
    {
        /* Returned value management */
        switch (errno)
        {
        case EBADF:
            returnValue = NAL_EBADF;
            break;

        default:
            returnValue = NAL_EINTERNAL;
            break;
        }
    }
    return returnValue;
}

/**
 * \brief This function is used to manipulate options associated with a socket.
 * 
 * @detail The option selection parameters use directly underlying BSD
 * implementation macros. A potential problem is possible since the optval
 * parameter type may differ from one implementation to another...
 * 
 * @param s Socket descriptor.
 * 
 * @param level Typical BSD values are SOL_SOCKET, IPPROTO_IP, IPPROTO_TCP,
 * IPPROTO_UDP.
 * 
 * @param optname The option identifier relative to the level selected.
 * 
 * @param optval. Several types of parameters can be used for the option value:
 *      - Boolean options generally use an int value,
 *      - Other options generally use specific structures,
 *      - Some int values may use different integer types. This API will define a 
 * portable type for options for which such incompatibilities have been 
 * detected: for IP_MULTICAST_TTL an unsigned char will be used.
 * 
 * @param optlen The length of the optval parameter.
 * 
 * @return 
 *      NAL_SUCCESS if no error occurred,\n
 *      NAL_EBADF: s parameter is not a valid socket descriptor,\n
 *      NAl_EINVAL: Invalid (NULL) value for parameters optval.Invalid value for parameter optlen,\n
 *      NAL_ENOPROTOOPT: Option unknown at the level indicated,\n
 *      NAL_EINTERNAL: Other internal error.
 */
int nal_sockSetSockOpt(int s, int level, int optname, const void *optval, int optlen)
{
    int returnValue = 0;
    int result = 0;

    if (optval == NULL)
    {
        returnValue = NAL_EINVAL;
    }
    else
    {
        result = setsockopt(s, level, optname, optval, optlen);

        /* Retrieve error if necessary */
        if (result == 0)
        {
            returnValue = NAL_SUCCESS;
        }
        else
        {
            /* Returned value management */
            perror("setsockopt:");
            switch (errno)
            {
            case EBADF:
                returnValue = NAL_EBADF;
                break;

            case EPERM:
                returnValue = NAL_ENOPROTOOPT;
                break;

            case ENOPROTOOPT:
                returnValue = NAL_ENOPROTOOPT;
                break;

            case EINVAL:
                returnValue = NAL_EINVAL;
                break;

            default:
                returnValue = NAL_EINTERNAL;
                break;
            }
        }
    }
    return returnValue;
}

/**
 * \brief This function is used to retrieve options associated with a socket.
 * 
 * @see nal_sockSetSockOpt().
 * 
 * @param s Socket descriptor to retrieve option.
 * 
 * @param level Level of the option. As for nal_sockSetSockOpt().
 * 
 * @param optname Option "name". As for nal_sockSetSockOpt().
 * 
 * @param[out] optval.  As for nal_sockSetSockOpt() but as an output. The buffer
 * must be supplied by the caller.
 * 
 * @param[out] optlen The length of the actual returned \a optval parameter.
 * 
 * @return 
 *      NAL_SUCCESS if no error occurred,\n
 *      NAL_EBADF: s parameter is not a valid socket descriptor,\n
 *      NAL_EINVAL: Invalid (NULL) value for parameters optval.Invalid value for parameter optlen,\n
 *      NAL_ENOPROTOOPT: Option unknown at the level indicated,\n
 *      NAL_EINTERNAL: Other internal error.
 */
int nal_sockGetSockOpt(int s, int level, int optname, void *optval, int *optlen)
{
    int returnValue = 0;
    int result = 0;
    int *opt;
    struct linger *lingerOpt;

    if ((optval == NULL) || (optlen == NULL))
    {
        returnValue = NAL_EINVAL;
    }
    else
    {
#if defined (NAL_HAVE_IPV6) && defined (IPV6_RECVPKTINFO)
        if (level == IPPROTO_IPV6 && optname == IPV6_PKTINFO) /* Linux kernel 2.6.24 requires to set IPV6_RECVPKTINFO to receive IPV6_PKTINFO... */
            optname = IPV6_RECVPKTINFO;
#endif

        result = getsockopt(s, level, optname, optval, (unsigned int *) optlen);

        /* Error management and returned value normalization */
        if (result == 0)
        {
            returnValue = NAL_SUCCESS;

            if (level == SOL_SOCKET)
            {
                if ((optname == SO_KEEPALIVE) || (optname == SO_REUSEADDR))
                {
                    opt = (int *) optval;
                    if (*opt != 0)
                        *opt = 1;
                }

                if (optname == SO_LINGER)
                {
                    lingerOpt = optval;
                    if (lingerOpt->l_onoff != 0)
                        lingerOpt->l_onoff = 1;
                }
            }
            if ((level == IPPROTO_TCP) && (optname == TCP_NODELAY))
            {
                opt = (int *) optval;
                if (*opt != 0)
                    *opt = 1;
            }
        }
        else
        {
            /* Returned value management */
            switch (errno)
            {
            case EBADF:
                returnValue = NAL_EBADF;
                break;

            case ENOPROTOOPT:
                returnValue = NAL_ENOPROTOOPT;
                break;

            default:
                returnValue = NAL_EINTERNAL;
                break;
            }
        }
    }
    return returnValue;
}

/**
 * \brief This function returns to the caller the Local IP Address/Port number.
 * 
 * @param s Socket descriptor that we wish to inquire about.
 * 
 * @param[out] addr Pointer to the address structure where the address 
 * information will be stored.
 * 
 * @param[out] addrlen The length of the actual returned addr parameter.
 * 
 * @return 
 *      NAL_SUCCESS if no error occurred,\n
 *      NAL_EINVAL : Invalid (NULL) value for parameters addr or addrlen,\n
 *      NAL_EBADF : s parameter is not a valid socket descriptor,\n
 *      NAL_ESOCKTNOSUPPORT : Socket s type not supported for this operation,\n
 *      NAL_EINTERNAL : Other internal error.    
 */
int nal_sockGetSockName(int s, T_nal_sockaddr *addr, int *addrlen)
{
    int result = 0;

    if ((addr == NULL) || (addrlen == 0))
    {
        result = NAL_EINVAL;
    }
    else
    {
        result = (int) getsockname(s, (struct sockaddr*) addr, (unsigned int *) addrlen);

        if (result != 0)
        {
            switch (errno)
            {
            case EBADF:
            case ENOTSOCK:
                result = NAL_EBADF;
                break;

            case EINVAL:
                result = NAL_EINVAL;
                break;

            default:
                result = NAL_EINTERNAL;
                break;
            }
        }
        else
        {
            result = NAL_SUCCESS;
        }
    }
    return result;
}

/**
 * \brief Retrieves a connected socket peer address.
 * 
 * @warning NOT IMPLEMENTED!
 * 
 * @param s Socket descriptor.
 * 
 * @param[out] addr The structure that will receive the socket address.
 * 
 * @param[out] addrlen The length of the actual returned \a addr parameter.
 * 
 * @return 
 *      NAL_SUCCESS if no error occurred, or an NAL error code.
 */
int nal_sockGetPeerName ( int s, T_nal_sockaddr *addr, int *addrlen)
{
    return NAL_SUCCESS;
}

/**
 * \brief Return an indicator specifying if the Socket IP Address variable 
 * is IPv4 or IPv6 address.
 * 
 * @warning NOT IMPLEMENTED!
 * 
 * @param sockAddr Pointer to a Socket IP Address variable.
 * 
 * @return 
 *      On success, the function returns:
 *      - 0 when the domain is IPv4
 *      - when the domain is IPv6
 *      Otherwise, the function returns a negative value specifying 
 * the error cause.
 */
int nal_sockaddrCheckFamily (const T_nal_sockaddr *sockAddr)
{
    return NAL_SUCCESS;
}

/**
 * \brief Perform a command on the socket descriptor.
 * 
 * @detail Some implementations may implement this function using fcntl, ioctl or
 * ioctlsocket according to the portable command implemented.
 * 
 * @param s Socket descriptor to manipulate.
 * 
 * @param cmd The command to perform. Only portable commands defined in AL IP are
 * supported.
 * 
 * @param arg The command argument.
 * 
 * @return
 *      NAL_SUCCESS if no error occurred,\n
 *      NAL_EBADF : s parameter is not a valid socket descriptor,\n
 *      NAL_EINVAL : Invalid (NULL) parameter arg, Invalid value ( != FIONBIO) for parameter cmd,\n
 *      NAL_EINTERNAL : Other internal error.
 */
int al_ioctl(int s, int cmd, int *arg)
{
    int returnValue = 0;
    int result = 0;

    if ((arg == NULL) || ((unsigned int) cmd != (unsigned int) (FIONBIO)))
    {
        returnValue = NAL_EINVAL;
    }
    else
    {
        if (*arg)
            result = fcntl(s, F_SETFL, fcntl(s, F_GETFL) | O_NONBLOCK);
        else
            result = fcntl(s, F_SETFL, fcntl(s, F_GETFL)&~O_NONBLOCK);

        /* Retrieve error if necessary */
        if (result == 0)
        {
            returnValue = NAL_SUCCESS;
        }
        else
        {
            /* Returned value management */
            switch (errno)
            {
            case EBADF:
                returnValue = NAL_EBADF;
                break;

            default:
                returnValue = NAL_EINTERNAL;
                break;
            }
        }
    }
    return returnValue;
}

/**
 * \brief Get IP address for a network host.
 * 
 * @detail This is a very simplified version of gethostbyname() since the 
 * original has some defects:
 *      - Not re-entrant (the returned result is static),
 *      - Allocation of results by the caller with variable data-length and
 * polymorphism.

 * @param name The host name for which IP address must be retrieved.
 * 
 * @param[out] inaddr A structure that will contain the host address.
 * 
 * @return
 *      NAL_SUCCESS if no error occurred, or an NAL error code.
 */
int nal_getHostByName(const char *name, T_nal_in_addr * inaddr)
{
    struct hostent * he;
    /* TODO: not thread-safe */
    he = gethostbyname(name);
    if (he)
    {
        if (he->h_addrtype != AF_INET || he->h_length != sizeof (T_nal_in_addr))
            return NAL_EAFNOSUPPORT;
        memcpy(inaddr, he->h_addr_list[0], he->h_length);
        return NAL_SUCCESS;
    }
    else
    {
        switch (h_errno)
        {
        case NETDB_INTERNAL:
            return NAL_ENETDOWN;
        case HOST_NOT_FOUND:
            return NAL_EHOSTNOTFOUND;
        case TRY_AGAIN:
            return NAL_EAGAIN;
        case NO_RECOVERY:
            return NAL_EINTERNAL;
        case NO_DATA:
            return NAL_ENODATA;
        default:
            return NAL_ERROR; /* TODO: should be a translated code. */
        }
    }
}

/**
 * \brief Specify wait for incoming connection and queue limit.
 * 
 * @detail This function is called on a listening connection-oriented socket in order
 * create a new socket for an incoming connection.
 * 
 * @param s Socket descriptor. Socket to wait for connections on.
 * 
 * @param[out] addr A socket address that will be filled with the connecting
 * entity address. The socket address structure buffer is provided by the
 * caller.
 * 
 * @param[in,out] addrlen Should be initialized with the provided addr
 * buffer and will be filled with the actual size of the returned structure.
 * 
 * @return 
 *      The socket descriptor to use for connected communications,\n
 *      NAL_EBADF: s is not a valid socket descriptor,\n
 *      NAL_EINVAL: Invalid (NULL) value for addr, invalid size for addrlen,\n
 *      NAL_EOPNOTSUPP: s does not support accept operation (not a SOCK_STREAM socket),\n
 *      NAL_ENOMEM: Not enough memory / buffers / sockets descriptors,\n
 *      NAL_EAGAIN: Socket is marked non blocking and call would block,\n
 *      NAL_EINTERNAL: Other internal error.
 */
int nal_sockAccept(int s, T_nal_sockaddr *addr, int *addrlen)
{
    int returnValue = 0;
    int result = 0;

    if ((addr == NULL) || (addrlen == NULL))
    {
        returnValue = NAL_EINVAL;
    }
    else
    {
        result = accept(s, (struct sockaddr*) addr, (unsigned int *) addrlen);

        /* Retrieve error if necessary */
        if (result >= 0)
        {
            returnValue = result;
        }
        else
        {
            /* Returned value management */
            switch (errno)
            {
            case EBADF:
            case ENOTSOCK:
                returnValue = NAL_EBADF;
                break;

            case ENOMEM:
            case EMFILE:
            case ENFILE:
                returnValue = NAL_ENOMEM;
                break;

            case EOPNOTSUPP:
                returnValue = NAL_EOPNOTSUPP;
                break;

            case EWOULDBLOCK:
                returnValue = NAL_EAGAIN;
                break;

            default:
                returnValue = NAL_EINTERNAL;
                break;
            }
        }
    }
    return returnValue;
}

/**
 * \brief Assign a local protocol address to a socket.
 * 
 * @param s Socket descriptor to assign an ip address and port number to.
 * 
 * @param addr The local address to which the socket is to be bound.
 * 
 * @param addrlen The size of the addr parameter.
 * 
 * @return 
 *      NAL_SUCCESS if no error occurred,\n
 *      NAL_EBADF : s parameter is not a valid socket descriptor,\n
 *      NAL_EINVAL : Invalid (NULL) value for parameters addr and addrlen. Invalid
 * size defined by parameter addrlen,\n
 *      NAL_EADDRNOTAVAIL : Address already in use or address not available on this
 * platform,\n
 *      NAL_EINTERNAL : Other internal error.
 */
int nal_sockBind(int s, const T_nal_sockaddr *addr, int addrlen)
{
    int returnValue = 0;
    int result = 0;
    struct sockaddr_in *addr_in = NULL;

    if (addr == NULL)
    {
        returnValue = NAL_EINVAL;
    }
    else
    {
        /* Compute length for bind */
        addr_in = (struct sockaddr_in*) addr;
        switch (addr_in->sin_family)
        {
        case AF_INET:
            addrlen = sizeof ( struct sockaddr_in);
            break;
        case AF_INET6:
            addrlen = sizeof ( struct sockaddr_in6);
            break;
        default:
            returnValue = NAL_EINVAL;
            goto al_bindEnd;
            break;
        }

        result = bind(s, (struct sockaddr *) addr, addrlen);

        /* Retrieve error if necessary */
        if (result == 0)
        {
            returnValue = NAL_SUCCESS;
        }
        else
        {
            /* Returned value management */
            switch (errno)
            {
            case EBADF:
            case ENOTSOCK:
                returnValue = NAL_EBADF;
                break;

            case EADDRINUSE:
            case EADDRNOTAVAIL:
            case EACCES:
                returnValue = NAL_EADDRNOTAVAIL;
                break;

            default:
                returnValue = NAL_EINTERNAL;
                break;
            }
        }
    }
al_bindEnd:
    return returnValue;
}

/**
 * \brief Initiate a connection on a socket.
 * 
 * @param s Socket descriptor to connect with.
 * 
 * @param addr The peer address for the connection.
 * 
 * @param addrlen The size of the addr parameter.
 * 
 * @return 
 *      NAL_SUCCESS if no error occurred,\n
 *      NAL_EBADF : s parameter is not a valid socket descriptor.,\n
 *      NAL_EINVAL : Invalid (NULL) value for parameters addr and addrlen. 
 * Invalid size defined by parameter addrlen,\n
 *      NAL_EADDRNOTAVAIL : Address already in use or address not available on 
 * this platform,\n
 *      NAL_ETIMEDOUT : Operation timed out,\n
 *      NAL_EADDRINUSE : Address already in use,\n
 *      NAL_EAFNOTSUPPORT : Address in the specified address family not 
 * available on this socket,\n
 *      NAL_EALREADY : al_connect already done on this socket,\n
 *      NAL_ECONNREFUSED : The attempt to connect was forcefully rejected by 
 * the server,\n
 *      NAL_EINTERNAL : Other internal error,\n
 *      NAL_EAGAIN : The socket is non-blocking and the connection cannot be 
 * completed immediately.
 */
int nal_sockConnect(int s, const T_nal_sockaddr *addr, int addrlen)
{
    int returnValue = 0;
    int result = 0;
#ifdef NAL_HAVE_IPV4
    struct sockaddr_in* addr_in;
#else /* IPv6 */
    struct sockaddr_in6 *addr_in;
#endif

    if (addr == NULL)
    {
        returnValue = NAL_EINVAL;
    }
    else
    {
        /* Compute length for connect */
#ifdef NAL_HAVE_IPV4
        addr_in = (struct sockaddr_in*) addr;
#else /* IPv6 */
        addr_in = (struct sockaddr_in6 *) addr;
#endif
        switch (addr_in->sin_family)
        {
        case AF_INET:
            addrlen = sizeof ( struct sockaddr_in);
            break;
        case AF_INET6:
            addrlen = sizeof ( struct sockaddr_in6);
            break;
        default:
            returnValue = NAL_EINVAL;
            goto al_connectEnd;
            break;
        }

        result = connect(s, (struct sockaddr*) addr, addrlen);
        /* Retrieve error if necessary  */
        if (result == 0)
        {
            returnValue = NAL_SUCCESS;
        }
        else
        {
            /* Returned value management */
            perror("connect:");
            switch (errno)
            {
            case EBADF:
            case ENOTSOCK:
                returnValue = NAL_EBADF;
                break;

            case EADDRINUSE:
                returnValue = NAL_EADDRINUSE;
                break;

            case EADDRNOTAVAIL:
            case EACCES:
                returnValue = NAL_EADDRNOTAVAIL;
                break;

            case EAFNOSUPPORT:
                returnValue = NAL_EAFNOSUPPORT;
                break;

            case EALREADY:
                returnValue = NAL_EALREADY;
                break;

            case ECONNREFUSED:
                returnValue = NAL_ECONNREFUSED;
                break;

            case EINPROGRESS:
#if EWOULDBLOCK != EAGAIN
            case EWOULDBLOCK:
#endif
            case EAGAIN:
                returnValue = NAL_EAGAIN;
                break;

            default:
                returnValue = NAL_EINTERNAL;
                break;
            }
        }
    }
al_connectEnd:
    return returnValue;
}

/**
 * \brief Specify wait for incoming connection and queue limit.
 * 
 * @param s Socket descriptor to listen on.
 * 
 * @param backlog Length of the queue for pending connections.The maximum length 
 * the queue of pending connections may grow to.
 * 
 * @return 
 *      NAL_SUCCESS if no error occurred,\n
 *      NAL_EBADF : s is not a valid socket descriptor,\n
 *      NAL_EINVAL : s is already connected,\n
 *      NAL_EOPNOTSUPP : s does not support listening operation,\n
 *      NAL_EINTERNAL : Other internal error.
 */
int nal_sockListen(int s, int backlog)
{
    int returnValue = 0;
    int result = 0;

    result = listen(s, backlog);

    /* Retrieve error if necessary */
    if (result == 0)
    {
        returnValue = NAL_SUCCESS;
    }
    else
    {
        /* Returned value management */
        switch (errno)
        {
        case EBADF:
        case ENOTSOCK:
            returnValue = NAL_EBADF;
            break;

        case EINVAL:
            returnValue = NAL_EINVAL;
            break;

        case EOPNOTSUPP:
            returnValue = NAL_EOPNOTSUPP;
            break;

        default:
            returnValue = NAL_EINTERNAL;
            break;
        }
    }
    return returnValue;
}

/**
 * \brief Monitors a set of sockets for read or write.
 * 
 * @detail This function examines a set of socket descriptors to check if some data can
 * be read or written to a specified socket. Note that the <tt>al_fd_set</tt>
 * type is defined in a platform dependent way (generally preprocessor macro).
 * The socket to be checked must be added to the file descriptors array before
 * this function is called. After the call the array only contains descriptors
 * ready for the specified action. This function must be used in polling mode
 * when the application has to wait for multiple events. The sets of file
 * descriptors should be accessed with the following macros:
 *      - FD_ZERO(&fdset): Resets the fdset set,
 *      - FD_SET(fd, &fdset): Adds the fd file descriptor to the fdset set,
 *      - FD_CLR(fd, &fdset): Removes the fd file descriptor from the fdset set,
 *      - FD_ISSET(fd, &fdset): Tests if fd is a member of the fdset set.
 * 
 * @param nfds Maximum file descriptor number plus one.
 * 
 * @param readfds Set of file descriptors to test for reading, may be NULL.
 * 
 * @param writefds Set of file descriptor to test for writing, may be NULL.
 * 
 * @param exceptfds Set of file descriptor to test for exceptional condition,
 * may be NULL.
 * 
 * @param timeout Maximum delay to wait for, may be NULL.
 * 
 * @return 
 *      The number of ready descriptors that are contained in the descriptor
 * sets, or an NAL error code if an error occurred. If the time limit expires,
 * select() returns 0 (NAL_SUCCESS). If select() returns with an error,
 * including one due to an interrupted call, the descriptor sets will be
 * unmodified.\n
 *      NAL_EBADF: One of the socket descriptor is not a valid socket descriptor,\n
 *      NAL_EINTERNAL: Other internal error.
 */
int nal_sockSelect(int nfds,
                   nal_fd_set * readfds,
                   nal_fd_set * writefds,
                   nal_fd_set * exceptfds,
                   T_nal_time * timeout)
{
    int returnValue = 0;
    int result = 0;
    struct timeval local_timeout;

    if (timeout != NULL)
    {
        /* Protect the timeout pointer against modification */
        local_timeout.tv_sec = timeout->sec;
        local_timeout.tv_usec = timeout->usec;

        result = select(nfds, readfds, writefds, exceptfds, &local_timeout);
    }
    else
        result = select(nfds, readfds, writefds, exceptfds, (struct timeval *) timeout);

    /* Retrieve error if necessary */
    if (result >= 0)
    {
        returnValue = result;
    }
    else
    {
        /* Returned value management */
        switch (errno)
        {
        case EBADF:
        case ENOTSOCK:
            returnValue = NAL_EBADF;
            break;

        default:
            returnValue = NAL_EINTERNAL;
            break;
        }
    }

    return returnValue;
}

/**
 * \brief Reads data from a file descriptor, used to receive messages from a socket.
 * 
 * @note This API is somehow redundant with nal_sockRecv() but works on both 
 * files and sockets which may be useful for some applications.
 * 
 * @param s Socket descriptor to read data on.
 * 
 * @param buf[out] Pointer to a buffer for receiving data.
 * 
 * @param len Length of the buffer.
 * 
 * @return 
 *      The number of received bytes. 0 if end of file was encountered,\n
 *      NAL_EBADF: s parameter is not a valid socket descriptor,\n
 *      NAL_ENOBUFS: Not enough memory to complete the operation,\n
 *      NAL_EMSGSIZE: Reception buffer length (parameter len) too small to 
 * receive the message,\n
 *      NAL_EAGAIN: The socket is marked non-blocking, and the receive operation
 * would block, or a receive timeout had been set, and the timeout expired 
 * before data were received,\n
 *      NAL_EINVAL: Invalid (NULL) value for parameters buf.Invalid size (<=0) defined
 * by parameter len. Unsupported flags option,\n
 *      NAL_ECONNRESET : The connection is closed by the peer during a receive
 * attempt on a socket,\n
 *      NAL_ENOTCONN : Socket not connected,\n
 *      NAL_EINTERNAL : Other internal error
 */
int nal_sockRead(int s, void *buf, size_t len)
{
    int returnValue = 0;
    int result = 0;

    if (buf == NULL)
    {
        returnValue = NAL_EINVAL;
    }
    else
    {
        result = read(s, buf, len);

        /* Retrieve error if necessary */
        if (result >= 0)
        {
            returnValue = result;
        }
        else
        {
            /* Returned value management */
            switch (errno)
            {
            case EBADF:
            case ENOTSOCK:
                returnValue = NAL_EBADF;
                break;

            case ENOMEM:
                returnValue = NAL_ENOBUFS;
                break;

            case ENOTCONN:
                returnValue = NAL_ENOTCONN;
                break;

            case EINVAL:
                returnValue = NAL_EINVAL;
                break;

            case EAGAIN:
                returnValue = NAL_EAGAIN;
                break;

            default:
                returnValue = NAL_EINTERNAL;
                break;
            }
        }
    }
    return returnValue;
}

/**
 * \brief Read data from the socket, used to receive messages from a socket.
 * 
 * @warning Note that the flags used for this API are the native BSD ones and are not
 * redefined by the NAL API.
 * 
 * @param s Socket descriptor to read data on.
 * 
 * @param buf[out] Pointer to a buffer for receiving data.
 * 
 * @param len Length of the buffer.
 * 
 * @param flags Typical BSD flags are MSG_PEEK, MSG_DONTWAIT, MSG_OOB...
 * 
 * @return 
 *      The number of received bytes,\n
 *      NAL_EBADF : s parameter is not a valid socket descriptor,\n
 *      NAL_ENOBUFS : Not enough memory to complete the operation,\n
 *      NAL_EMSGSIZE : Reception buffer length (parameter len) too small to 
 * receive the message,\n
 *      NAL_EAGAIN : The socket is marked non-blocking, and the receive 
 * operation would block, or a receive timeout had been set, and the
 * timeout expired before data were received,\n
 *      NAL_EINVAL : Invalid (NULL) value for parameters buf. Invalid size (<=0)
 * defined by parameter len. Unsupported flags option,\n
 *      NAL_ECONNRESET : The connection is closed by the peer during a receive
 * attempt on a socket,\n
 *      NAL_ENOTCONN : Socket not connected,\n
 *      NAL_EINTERNAL : Other internal error.
 */
int nal_sockRecv(int s, void *buf, int len, unsigned int flags)
{
    int returnValue = 0;
    int result = 0;

    if (buf == NULL)
    {
        returnValue = NAL_EINVAL;
    }
    else
    {
        result = recv(s, buf, len, flags);

        /* Retrieve error if necessary */
        if (result >= 0)
        {
            returnValue = result;
        }
        else
        {
            /* Returned value management */
            switch (errno)
            {
            case EBADF:
            case ENOTSOCK:
                returnValue = NAL_EBADF;
                break;

            case ENOMEM:
                returnValue = NAL_ENOBUFS;
                break;

            case ENOTCONN:
                returnValue = NAL_ENOTCONN;
                break;

            case EINVAL:
                returnValue = NAL_EINVAL;
                break;

#if EWOULDBLOCK != EAGAIN
            case EWOULDBLOCK:
#endif
            case EAGAIN:
                returnValue = NAL_EAGAIN;
                break;

            default:
                returnValue = NAL_EINTERNAL;
                break;
            }
        }
    }
    return returnValue;
}

/**
 * \brief Read data from the socket, used to receive messages from a socket.
 * 
 * @detail This function can be used both for datagram or connection-oriented but
 * is mainly used for datagram sockets in order to retrieve the sender
 * address.
 * 
 * @param s Socket descriptor to read data on.
 * 
 * @param buf[out] Pointer to a buffer for receiving data.
 * 
 * @param len Length of the buffer.
 * 
 * @param flags Typical BSD flags are MSG_PEEK, MSG_DONTWAIT, MSG_OOB...
 * 
 * @param[out] from A socket address that will be filled with the source
 * entity address. The socket address structure buffer is provided by the caller.
 * 
 * @param[in,out] fromlen Should be initialized with the provided from
 * buffer and will be filled with the actual size of the returned structure.
 * 
 * @return 
 *      The number of received bytes,\n
 *      NAL_EBADF : s parameter is not a valid socket descriptor,\n
 *      NAL_ENOBUFS : Not enough memory to complete the operation,\n
 *      NAL_EMSGSIZE : Reception buffer length (parameter len) too small to 
 * receive the message,\n
 *      NAL_EAGAIN : The socket is marked non-blocking, and the receive operation
 * would block, or a receive timeout had been set, and the timeout expired 
 * before data were received,\n
 *      NAL_EINVAL : Invalid (NULL) value for parameters buf, from, or fromlen. 
 * Invalid size (<=0) defined by parameter len. Unsupported flags option,\n
 *      NAL_EINTERNAL : Other internal error.
 */
int nal_sockRecvFrom(int s,
                     void * buf,
                     int len,
                     unsigned int flags,
                     T_nal_sockaddr * from,
                     int * fromlen)
{
    int returnValue = 0;
    int result = 0;

    if ((buf == NULL) || (from == NULL) || (fromlen == NULL))
    {
        returnValue = NAL_EINVAL;
    }
    else
    {
        result = recvfrom(s, buf, len, flags, (struct sockaddr*) from, (unsigned int *) fromlen);

        /* Retrieve error if necessary */
        if (result >= 0)
        {
            returnValue = result;
        }
        else
        {
            /* Returned value management */
            switch (errno)
            {
            case EBADF:
            case ENOTSOCK:
                returnValue = NAL_EBADF;
                break;

            case ENOMEM:
                returnValue = NAL_ENOBUFS;
                break;

            case EINVAL:
                returnValue = NAL_EINVAL;
                break;

            case EAGAIN:
                returnValue = NAL_EAGAIN;
                break;

            default:
                returnValue = NAL_EINTERNAL;
                break;
            }
        }
    }
    return returnValue;
}

#ifdef IP_PKTINFO

/**
 * \brief Read data from the socket, used to receive messages from a socket.
 * 
 * @detail This function can be used for datagram sockets in order to retrieve the
 * sender address and the reception interface if the socket was bound to
 * INADDR_ANY.
 * 
 * @param s Socket descriptor.
 * 
 * @param buf[out] Pointer to a buffer for receiving data.
 * 
 * @param len Maximum number of bytes to read.
 * 
 * @param flags Typical BSD flags are MSG_PEEK, MSG_DONTWAIT, MSG_OOB...
 * 
 * @param[out] from A socket address that will be filled with the source
 * entity address. The socket address structure buffer is provided by the
 * caller.
 * 
 * @param[in,out] fromlen Should be initialized with the provided 
 * from buffer and will be filled with the actual size of the returned structure.
 * 
 * @param[out] itf If not NULL, the reception interface is retrieved provided
 * the IP(V6)_PKTINFO has been turned on on the socket.
 * 
 * @return 
 *      The number of received bytes,\n
 *      NAL_EBADF : s parameter is not a valid socket descriptor,\n
 *      NAL_ENOBUFS : Not enough memory to complete the operation,\n
 *      NAL_EMSGSIZE : Reception buffer length (parameter len) too small to 
 * receive the message,\n
 *      NAL_EAGAIN : The socket is marked non-blocking, and the receive operation
 * would block, or a receive timeout had been set, and the timeout expired 
 * before data were received,\n
 *      NAL_EINVAL : Invalid (NULL) value for parameters buf, from, or fromlen. 
 * Invalid size (<=0) defined by parameter len. Unsupported flags option,\n
 *      NAL_EINTERNAL : Other internal error.
 */
int nal_sockRecvOn(int s,
                   void * buf,
                   int len,
                   unsigned int flags,
                   T_nal_sockaddr * from,
                   int * fromlen,
                   uint32_t * itf)
{
    int returnValue = 0;
    int result = 0;
    struct msghdr mh;
    struct iovec iov;
    struct cmsghdr *cmptr = NULL;
    
#ifdef NAL_HAVE_IPV4
    char cbuf[ CMSG_SPACE(sizeof ( struct in_pktinfo)) ];
#else /* IPv6 */
    /* @todo On ubuntu 19.04 x64 struct in6_pktinfo does not seems be present! */
    char cbuf[ CMSG_SPACE(MAX(sizeof ( struct in_pktinfo), sizeof ( struct in6_pktinfo))) ];
#endif

    if ((buf == NULL) || (from == NULL) || (fromlen == NULL) || (len <= 0))
    {
        returnValue = NAL_EINVAL;
    }
    else
    {
        if (from)
        {
            mh.msg_name = from;
            mh.msg_namelen = *fromlen;
        }
        else
        {
            mh.msg_name = NULL;
            mh.msg_namelen = 0;
        }

        iov.iov_base = buf;
        iov.iov_len = len;
        mh.msg_iov = &iov;
        mh.msg_iovlen = 1;
        mh.msg_control = cbuf;
        mh.msg_controllen = sizeof (cbuf);
        mh.msg_flags = 0;

        result = recvmsg(s, &mh, flags);

        /* Receive auxiliary data in msgh */
        if (result >= 0)
        {
            if (itf != NULL)
            {
                for (cmptr = CMSG_FIRSTHDR(&mh); cmptr; cmptr = CMSG_NXTHDR(&mh, cmptr))
                {
                    if (cmptr->cmsg_level == IPPROTO_IP && cmptr->cmsg_type == IP_PKTINFO)
                    {
                        *itf = ((struct in_pktinfo *) CMSG_DATA(cmptr))->ipi_ifindex;
                        break;
                    }
                    if (cmptr->cmsg_level == IPPROTO_IPV6 && cmptr->cmsg_type == IPV6_PKTINFO)
                    {
#ifdef NAL_HAVE_IPV4
                        *itf = (uint32_t) ((struct in_pktinfo *) CMSG_DATA(cmptr))->ipi_ifindex;
#else
                        *itf = (uint32_t) ((struct in6_pktinfo *) CMSG_DATA(cmptr))->ipi6_ifindex;
#endif
                        break;
                    }
                }
                if (cmptr == NULL)
                    returnValue = NAL_EINTERNAL;
                else
                    returnValue = result;
            }
            else
                returnValue = result;
        }
        else
        {
            /* Returned value management */
            switch (errno)
            {
            case EBADF:
            case ENOTSOCK:
                returnValue = NAL_EBADF;
                break;

            case ENOMEM:
                returnValue = NAL_ENOBUFS;
                break;

            case EINVAL:
                returnValue = NAL_EINVAL;
                break;

            case EAGAIN:
                returnValue = NAL_EAGAIN;
                break;

            default:
                returnValue = NAL_EINTERNAL;
                break;
            }
        }
    }

    return returnValue;
}
#endif /* IP_PKTINFO */

/**
 * \brief Write to a file descriptor.
 * 
 * @detail This API is somehow redundant with al_send() but works on both files and
 * sockets which may be useful for some applications.
 * 
 * @param s A descriptor.
 * 
 * @param buf Pointer to a buffer containing data to be sent.
 * 
 * @param len Number of bytes to send.
 * 
 * @return 
 *      The number of sent bytes, or an NAL error code.
 */
int nal_sockWrite(int s, const void *buf, size_t len)
{
    return write(s, buf, len);
}

/**
 * \brief Send a message from a socket, used to transmit a message..
 * 
 * @warning Note that the flags used for this API are the native BSD ones and 
 * are not redefined by the AL API.
 * 
 * @param s Socket descriptor.
 * 
 * @param buf Pointer to a buffer containing data to be sent.
 * 
 * @param len Number of bytes to send.
 * 
 * @param flags Typical BSD flags are MSG_DONTROUTE, MSG_OOB... (unused)
 * 
 * @return 
 *      The number of sent bytes,\n
 *      NAL_EBADF : s parameter is not a valid socket descriptor,\n
 *      NAL_ENOBUFS : Not enough memory to complete the operation,\n
 *      NAL_EMSGSIZE : The socket requires that message be sent atomically,
 * and the size of the message to be sent made this impossible,\n
 *      NAL_ENOTCONN : Socket s is not connected,\n
 *      NAL_EAGAIN : The socket is marked non-blocking, and the emission,\n
 *      NAL_EINVAL : Invalid (NULL) value for parameter buf. Invalid size
 * (<=0) defined by parameter len,\n
 *      NAL_EINTERNAL : Other internal error.
 */
int nal_sockSend(int s, const void *buf, int len, unsigned int flags)
{
    int returnValue = 0;
    int result = 0;

    if (buf == NULL)
    {
        returnValue = NAL_EINVAL;
    }
    else
    {
        result = send(s, buf, len, flags);

        /* Retrieve error if necessary */
        if (result >= 0)
        {
            returnValue = result;
        }
        else
        {
            /* Returned value management */
            switch (errno)
            {
            case EBADF:
            case ENOTSOCK:
                returnValue = NAL_EBADF;
                break;

            case ENOBUFS:
                returnValue = NAL_ENOBUFS;
                break;

            case ENOTCONN:
                returnValue = NAL_ENOTCONN;
                break;

            case EMSGSIZE:
                returnValue = NAL_EMSGSIZE;
                break;

#if EWOULDBLOCK != EAGAIN
            case EWOULDBLOCK:
#endif
            case EAGAIN:
                returnValue = NAL_EAGAIN;
                break;

            case EINPROGRESS:
                returnValue = NAL_EINPROGRESS;
                break;

            case EINVAL:
                returnValue = NAL_EINVAL;
                break;

            default:
                returnValue = NAL_EINTERNAL;
                break;
            }
        }
    }

    return returnValue;
}

/**
 * \brief Send a message from a socket, used to transmit a message only with 
 * UDP sockets.
 * 
 * @detail This function can be used both for datagram or connection-oriented 
 * but is mainly used for datagram sockets in order to specify the destination
 * address.
 * 
 * @warning Note that the flags used for this API are the native BSD ones and 
 * are not redefined by the AL API.
 * 
 * @param s Socket descriptor.
 * 
 * @param buf Pointer to a buffer containing data to be sent.
 * 
 * @param len Number of bytes to send.
 * 
 * @param flags Typical BSD flags are MSG_DONTROUTE, MSG_OOB... (unused)
 * 
 * @param to A socket address that will be filled with the source entity
 * address. The socket address for the target of the message.
 * 
 * @param tolen Should contain the size of the provided \a to buffer.
 * 
 * @return 
 *      The number of sent bytes,\n
 *      NAL_EBADF : s parameter is not a valid socket descriptor.
 *      NAL_ENOBUFS : Not enough memory to complete the operation.
 *      NAL_EMSGSIZE : The socket requires that message be sent atomically,
 * and the size of the message to be sent made this impossible.
 *      NAL_EAGAIN : The socket is marked non-blocking, and the emission
 * operation would block.
 *      NAL_EINVAL : Invalid (NULL) value for parameters buf or to. Invalid size
 * (<=0) defined by parameter len.
 *      NAL_EINTERNAL : Other internal error.
 */
int nal_sockSendTo(int s,
                   const void * buf,
                   int len,
                   unsigned int flags,
                   const T_nal_sockaddr * to,
                   int tolen)
{
    int returnValue = 0;
    int result = 0;
#ifdef NAL_HAVE_IPV4
    struct sockaddr_in *addr_in = NULL;
#else /* IPv6 */
    struct sockaddr_in6 *addr_in = NULL;
#endif

    if ((buf == NULL) || (to == NULL))
    {
        returnValue = NAL_EINVAL;
    }
    else
    {
        /* Compute length for sendto */
#ifdef NAL_HAVE_IPV4
        addr_in = (struct sockaddr_in*) to;
        switch (addr_in->sin_family)
#else /* IPv6 */
        addr_in = (struct sockaddr_in6 *) to;
        switch (addr_in->sin6_family)
#endif
        {
        case AF_INET:
            tolen = sizeof ( struct sockaddr_in);
            break;
        case AF_INET6:
            tolen = sizeof ( struct sockaddr_in6);
            break;
        default:
            returnValue = NAL_EINVAL;
            goto al_sendtoEnd;
            break;
        }

        result = sendto(s, buf, len, flags, (const struct sockaddr *) to, tolen);

        /* Retrieve error if necessary */
        if (result >= 0)
        {
            returnValue = result;
        }
        else
        {
            /* Returned value management */
            perror("sendto");
            switch (errno)
            {
            case EBADF:
            case ENOTSOCK:
                returnValue = NAL_EBADF;
                break;

            case ENOBUFS:
                returnValue = NAL_ENOBUFS;
                break;

            case ENOTCONN:
                returnValue = NAL_ENOTCONN;
                break;

            case EMSGSIZE:
                returnValue = NAL_EMSGSIZE;
                break;

            case EWOULDBLOCK:
                returnValue = NAL_EAGAIN;
                break;

            case EINVAL:
                returnValue = NAL_EINVAL;
                break;

            default:
                returnValue = NAL_EINTERNAL;
                break;
            }
        }
    }

al_sendtoEnd:
    return returnValue;
}

#endif /* NO_NAL_IP_BSD_4_3 */
