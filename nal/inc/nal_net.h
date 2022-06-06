
#ifndef NAL_NET_H_
#define NAL_NET_H_

#include "nal_ip.h"

#if defined NAL_NET_LINUX
#else
#error "No stack defined ..."
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

    /**
     * @brief Data structure containing the IP information for a network interface.
     */
    typedef struct nal_netif_info {
        char *name; /**< The interface name. */
        unsigned char mac_address[6]; /**< The interface MAC address */

        unsigned short addr_nb; /**< The number of IP addresses contained in the addrs field */

        /**
         * A list of structures containing the IP (v4 or v6) addresses.
         * Notes: could define a sockaddr structure to add the network mask.
         */
        T_nal_sockaddr *addrs;

        unsigned int interface_selector;
    } T_nal_netifInfo;


    extern int nal_getNetifsInfo(T_nal_netifInfo **netifs, size_t *nb_netifs);
    extern int nal_releaseNetifsInfo(T_nal_netifInfo *netifs, size_t nb_netifs);
    extern int nal_convertSockAddrToString(T_nal_sockaddr sockaddr, char **addrbuff, size_t *addrbuffSize);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* NAL_NET_H_ */
