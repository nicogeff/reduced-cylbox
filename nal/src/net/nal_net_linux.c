
#include <stdlib.h>
#include <memory.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <linux/if_ether.h>
#include <arpa/inet.h>
#include <errno.h>

#ifdef NAL_HAVE_IPV6
#include <ifaddrs.h>
#endif

#include "nal_mem.h"
#include "nal_net.h"
#include "nal_libc.h"

#ifdef NO_NAL_NET_LINUX
#warning "NET module disable!"
#else

#define NAL_IS_VALID_NETIF(nif) (((nif)->ifa_addr != NULL) && ((nif)->ifa_addr->sa_family == AF_INET || (nif)->ifa_addr->sa_family == AF_INET6))

#ifdef NAL_HAVE_IPV4

/* @brief This function return the network interface index */
static int nal_getNetifIndex(const char *netifname, int *netifindex)
{
    int result = NAL_SUCCESS;

    int s = 0;
    struct ifreq ifr = {0};

    if ((NULL == netifname) || (NULL == netifindex))
    {
        result = NAL_EINVAL;
    }
    else
    {
        s = socket(AF_INET, SOCK_DGRAM, 0);
        if (s < 0)
        {
            result = NAL_EINTERNAL;
        }
        else
        {
            nal_strncpy_s(ifr.ifr_name, IFNAMSIZ, netifname, strlen(netifname) * sizeof (char));

            if (ioctl(s, SIOCGIFINDEX, &ifr) < 0)
            {
                result = NAL_EINTERNAL;
                goto exit;
            }

            (*netifindex) = ifr.ifr_ifindex;
        }
    }

exit:

    /* Data recollection and cleaning */
    if (0 != s)
    {
        close(s);
    }

    return result;
}

/* @brief This function return the network interface MAC address */
static int nal_getNetifMacAddr(const char *netifname, char netifmac[], size_t netifmacSize)
{
    int result = NAL_SUCCESS;

    int s = 0;
    struct ifreq ifr;

    if ((NULL == netifname) || (NULL == netifmac) || (netifmacSize < IFHWADDRLEN))
    {
        result = NAL_EINVAL;
    }
    else
    {
        s = socket(AF_INET, SOCK_DGRAM, 0);
        if (s < 0)
        {
            result = NAL_EINTERNAL;
        }
        else
        {
            nal_strncpy_s(ifr.ifr_name, IFNAMSIZ, netifname, strlen(netifname) * sizeof (char));

            if (ioctl(s, SIOCGIFHWADDR, &ifr) < 0)
            {
                result = NAL_EINTERNAL;
                goto exit;
            }

            netifmac[0] = ifr.ifr_hwaddr.sa_data[0];
            netifmac[1] = ifr.ifr_hwaddr.sa_data[1];
            netifmac[2] = ifr.ifr_hwaddr.sa_data[2];
            netifmac[3] = ifr.ifr_hwaddr.sa_data[3];
            netifmac[4] = ifr.ifr_hwaddr.sa_data[4];
            netifmac[5] = ifr.ifr_hwaddr.sa_data[5];
        }
    }

exit:

    /* Data recollection and cleaning */
    if (0 != s)
    {
        close(s);
    }

    return result;
}


#define NAL_NET_MAX_NETIF_HANDLED 64

/**
 * @brief This function return the list of network interface with these following
 * information:
 * - network interface index
 * - network interface name
 * - network interface MAC address
 * - network interface IPv4 address (the first one)
 * @note This function return only one IP address for each network interface
 * @note This function can manage only NAL_NET_MAX_NETIF_HANDLED network interface
 * @param[out] netifs Network interfaces information filled by this function
 * @param[out] nb_netifs Number of interfaces filled by this function
 * @return A NAL error code
 */
int nal_getNetifsInfo(T_nal_netifInfo **netifs, size_t *nb_netifs)
{
    int result = NAL_SUCCESS;
    int s = 0;
    int i = 0;

    int ifindex = 0;

    char ifdata[1024] = {0};

    struct ifconf ifcfg;
    struct ifreq *ifr;

    char ifmac[IFHWADDRLEN] = {0};
    size_t ifmacSize = IFHWADDRLEN;

    T_nal_netifInfo * netifList[NAL_NET_MAX_NETIF_HANDLED] = {0};
    T_nal_netifInfo *netifObject = NULL;

    /* Sanity checks */
    if ((NULL == netifs) || (NULL == nb_netifs))
    {
        result = NAL_EINVAL;
    }
    else
    {
        s = socket(AF_INET, SOCK_DGRAM, 0);
        if (s < 0)
        {
            result = NAL_ERROR;
        }
        else
        {
            ifcfg.ifc_len = sizeof (ifdata);
            ifcfg.ifc_buf = (caddr_t) ifdata;

            if (ioctl(s, SIOCGIFCONF, &ifcfg) < 0)
            {
                result = NAL_ERROR;
                goto exit;
            }

            ifr = (struct ifreq*) ifdata;

            while ((char*) ifr < (ifdata + ifcfg.ifc_len))
            {
                switch (ifr->ifr_addr.sa_family)
                {
                case AF_INET:

                    /* Get interface name */
                    netifObject = NAL_MALLOC(sizeof (T_nal_netifInfo));
                    if (NULL == netifObject)
                    {
                        result = NAL_ENOMEM;
                        goto exit;
                    }
                    memset(netifObject, 0, sizeof (T_nal_netifInfo));
                    netifObject->name = NAL_MALLOC(strlen(ifr->ifr_name) * sizeof (char) + 1);
                    if (NULL == netifObject->name)
                    {
                        result = NAL_ENOMEM;
                        goto exit;
                    }
                    memset(netifObject->name, 0, strlen(ifr->ifr_name) * sizeof (char) + 1);
                    memcpy(netifObject->name, ifr->ifr_name, strlen(ifr->ifr_name) * sizeof (char));

                    /* Get IP Address */
                    /* @todo Only the first one IP address is handled */
                    netifObject->addr_nb = 1;
                    netifObject->addrs = NAL_MALLOC(sizeof (T_nal_sockaddr));
                    if (NULL == netifObject->addrs)
                    {
                        result = NAL_ENOMEM;
                        goto exit;
                    }
                    memcpy(netifObject->addrs, &ifr->ifr_addr, sizeof (T_nal_sockaddr));

                    /* Get interface index */
                    if (NAL_SUCCESS != nal_getNetifIndex(ifr->ifr_name, &ifindex))
                    {
                        result = NAL_ERROR;
                        goto exit;
                    }
                    netifObject->interface_selector = ifindex;

                    /* Get MAC address */
                    memset(ifmac, 0, IFHWADDRLEN * sizeof (char));
                    if (NAL_SUCCESS != nal_getNetifMacAddr(ifr->ifr_name, ifmac, ifmacSize))
                    {
                        result = NAL_ERROR;
                        goto exit;
                    }
                    memcpy(netifObject->mac_address, ifmac, ifmacSize);
                    break;
                case AF_INET6:
                    break;
                default:
                    break;
                }

                /* Save object into the list */
                if (i < NAL_NET_MAX_NETIF_HANDLED)
                    netifList[i] = netifObject;
                else
                {
                    result = NAL_ENOSPC;
                    goto exit;
                }

                /* Next interface */
                ifr = (struct ifreq*) ((char*) ifr + sizeof (*ifr));
                i++;
            }
            close(s);

            /** 
             * Assign all interface into out parameters 
             */

            *netifs = NAL_MALLOC(i * sizeof (T_nal_netifInfo));
            if (NULL == (*netifs))
            {
                result = NAL_ENOMEM;
                goto exit;
            }
            memset((*netifs), 0, sizeof (T_nal_netifInfo));

            for (int ii = 0; ii < i; ++ii)
            {
                (*netifs)[ii].name = NAL_MALLOC(strlen(netifList[ii]->name) * sizeof (char) + 1);
                memset((*netifs)[ii].name, 0, strlen(netifList[ii]->name) * sizeof (char) + 1);
                memcpy((*netifs)[ii].name, netifList[ii]->name, strlen(netifList[ii]->name) * sizeof (char));
                
                memcpy((*netifs)[ii].mac_address, netifList[ii]->mac_address, IFHWADDRLEN);
                
                (*netifs)[ii].interface_selector = netifList[ii]->interface_selector;
                
                (*netifs)[ii].addr_nb = netifList[ii]->addr_nb;
                
                (*netifs)[ii].addrs = NAL_MALLOC(netifList[ii]->addr_nb * sizeof (T_nal_sockaddr));
                memcpy((*netifs)[ii].addrs, netifList[ii]->addrs, netifList[ii]->addr_nb * sizeof (T_nal_sockaddr));
            }

            *nb_netifs = (size_t) i;
        }
    }

exit:

    /* Release local objects */
    for (int ii = 0; ii < i; ++ii)
    {
        if (NULL != netifList[ii])
        {
            if (NULL != netifList[ii]->name)
            {
                NAL_FREE(netifList[ii]->name);
                netifList[ii]->name = NULL;
            }
            if (NULL != netifList[ii]->addrs)
            {
                NAL_FREE(netifList[ii]->addrs);
                netifList[ii]->addrs = NULL;
            }
            NAL_FREE(netifList[ii]);
            netifList[ii] = NULL;
        }
    }

    /* Release socket */
    if (0 != s)
    {
        close(s);
    }

    return result;
}

/**
 * @brief Release the data allocated by the function nal_getNetifsInfo().
 * @param[in] netifs Network interfaces information
 * @param[in] netifs Network interfaces information
 */
int nal_releaseNetifsInfo(T_nal_netifInfo *netifs, size_t nb_netifs)
{
    int i = 0;
    int result = NAL_SUCCESS;

    /* Sanity checks */
    if ((NULL == netifs))
    {
        result = NAL_EINVAL;
    }
    else
    {
        for (i = 0; i < nb_netifs; i++)
        {
            if (NULL != netifs[i].name)
            {
                NAL_FREE(netifs[i].name);
                netifs[i].name = NULL;
            }
            if (NULL != netifs[i].addrs)
            {
                NAL_FREE(netifs[i].addrs);
                netifs[i].addrs = NULL;
            }
        }
        NAL_FREE(netifs);
        /*netifs = NULL; static warning, do nothing */
    }
    return result;
}

/**
 * @brief This function convert a sockaddr to a string
 * @param sockaddr
 * @param addrbuff
 * @param addrbuffSize
 * @return 
 */
int nal_convertSockAddrToString(T_nal_sockaddr sockaddr, char **addrbuff, size_t *addrbuffSize)
{
    int result = NAL_SUCCESS;

    /* Sanity checks */
    if ((NULL == addrbuff) || (NULL != (*addrbuff)) || (NULL == addrbuffSize))
    {
        result = NAL_EINVAL;
    }
    else
    {
        struct sockaddr_in *sa = (struct sockaddr_in*) &sockaddr;

        *addrbuff = NAL_MALLOC(INET_ADDRSTRLEN * sizeof (char));
        if (NULL == *addrbuff)
        {
            result = NAL_ENOMEM;
            goto exit;
        }
        memset(*addrbuff, 0, INET_ADDRSTRLEN * sizeof (char));

        inet_ntop(AF_INET, &(sa->sin_addr), *addrbuff, INET_ADDRSTRLEN);
        
        (*addrbuffSize) = INET_ADDRSTRLEN * sizeof (char);
    }

exit:

    if ((NAL_SUCCESS != result) && (NULL != *addrbuff))
    {
        NAL_FREE(*addrbuff);
        /*addrbuff = NULL; static warning, do nothing */
    }

    return result;
}

#endif /* NAL_HAVE_IPV4 */

#endif /* NO_NAL_NET_LINUX */