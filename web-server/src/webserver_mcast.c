#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "civetweb.h"

#include "nal_common.h"
#include "nal_rtos.h"
#include "nal_mem.h"
#include "nal_net.h"

#include "com_error.h"
#include "com_entity.h"

#include "entmgt_configure.h"

#include "tools_strManip.h"
#include "tools_trace.h"

#include "webserver_mgt.h"
#include "webserver_utils.h"

/**
 * \brief This function stop the thread : WWW
 */
void webserver_mcast_stop(void)
{
    return;
}

/*---------------------------- Event Handler ---------------------------------*/

static T_com_status webservice_mcast_build_ssdp_notify(char **payload, size_t *payloadSize)
{
    T_com_status returnCode = COM_SUCCESS;

    char tmpPayload[1024] = {0};
    size_t tmpPayloadSize = 0u;

    /* Sanity check */
    VERIF_ARGS("payload", payload != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("*payload", *payload == NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("payloadSize", payloadSize != NULL, returnCode, COM_ERR_PARAM);
    if (returnCode != COM_SUCCESS)
    {
        TRACE_GEN(TRACE, "Invalid parameter", __FUNCTION__, __LINE__);
        goto exit;
    }

    nal_snprintf_s(tmpPayload, 1024, 1024, &tmpPayloadSize,
                   "NOTIFY * HTTP/1.1\r\n" \
                   "HOST: %s:%s\r\n" \
                   "NT: urn:cylbox.fr:service:discovery:1\r\n" \
                   "NTS: ssdp:alive\r\n" \
                   "USN: uuid:%s\r\n" \
                   "LOCATION: http://%s:9443/#\r\n" \
                   "CACHE-CONTROL: max-age=1800\r\n" \
                   "SERVER: %s ssdp\r\n",
                   get_WEBSERVER_CONF_MCAST_GROUP(),
                   get_WEBSERVER_CONF_MCAST_PORT(),
                   entmgt_get_device_uuid(),
                   entmgt_get_device_ip_addr(),
                   entmgt_get_device_hostname());

    /* @todo check if tmpPayload is saturate! */

    *payload = NAL_MALLOC(tmpPayloadSize * sizeof (char) + 1);
    if (NULL == *payload)
    {
        returnCode = COM_ERR_NO_MEMORY;
        goto exit;
    }

    memset(*payload, 0, tmpPayloadSize * sizeof (char) + 1);

    (*payloadSize) = tmpPayloadSize;

    /* Copy tmpPayload */
    nal_strncpy_s(*payload, (*payloadSize), tmpPayload, tmpPayloadSize);

exit:
    return returnCode;
}

static T_com_status webservice_mcast_build_ssdp_search_response(char **payload, size_t *payloadSize)
{
    T_com_status returnCode = COM_SUCCESS;

    char tmpPayload[1024] = {0};
    size_t tmpPayloadSize = 0u;

    T_nal_datetime datetime = {0};
    char *datetimestr = NULL;
    size_t datetimestrSize = 0u;

    /* Sanity check */
    VERIF_ARGS("payload", payload != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("*payload", *payload == NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("payloadSize", payloadSize != NULL, returnCode, COM_ERR_PARAM);
    if (returnCode != COM_SUCCESS)
    {
        TRACE_GEN(TRACE, "Invalid parameter", __FUNCTION__, __LINE__);
        goto exit;
    }

    /* Get time */
    nal_getUtcTime(&datetime);
    nal_convertTimeToString(&datetime, &datetimestr, &datetimestrSize);

    nal_snprintf_s(tmpPayload, 1024, 1024, &tmpPayloadSize,
                   "HTTP/1.1 200 OK\r\n" \
                   "CACHE-CONTROL: max-age=1800\r\n" \
                   "DATE: %s GMT\r\n" \
                   "EXT: \r\n" \
                   "LOCATION: http://%s:9443/#\r\n" \
                   "SERVER: CylBoxSSDP\r\n" \
                   "ST: urn:cylbox.fr:service:discovery:1\r\n" \
                   "USN: uuid:%s\r\n",
                   datetimestr,
                   entmgt_get_device_ip_addr(),
                   entmgt_get_device_uuid());

    /* @todo check if tmpPayload is saturated! */

    *payload = NAL_MALLOC(tmpPayloadSize * sizeof (char) + 1);
    if (NULL == *payload)
    {
        returnCode = COM_ERR_NO_MEMORY;
        goto exit;
    }

    memset(*payload, 0, tmpPayloadSize * sizeof (char) + 1);

    (*payloadSize) = tmpPayloadSize;

    /* Copy tmpPayload */
    nal_strncpy_s(*payload, (*payloadSize), tmpPayload, tmpPayloadSize);

exit:
    if (NULL != datetimestr)
    {
        NAL_FREE(datetimestr);
        datetimestr = NULL;
    }

    return returnCode;
}

//static void webservice_mcast_ev_handler(struct mg_connection *nc, int ev, void *p)
//{
//    //    struct mbuf *io = &nc->recv_mbuf;
//    //    (void) p;
//    //    switch (ev)
//    //    {
//    //    case MG_EV_POLL:
//    //    {
//    //        //        const char *peer;
//    //        //        char *payload = NULL;
//    //        //        size_t payloadSize = 0u;
//    //        //        peer = inet_ntoa(nc->sa.sin.sin_addr);
//    //        //        printf("nc->sa: %s %d\n", peer, ntohs(nc->sa.sin.sin_port));
//    //        //        webservice_mcast_build_ssdp_notify(&payload, &payloadSize);
//    //        //        if (NULL != payload)
//    //        //        {
//    //        //            NAL_FREE(payload);
//    //        //            payload = NULL;
//    //        //        }
//    //        break;
//    //    }
//    //    case MG_EV_RECV:
//    //    {
//    //        char *payload = NULL;
//    //        size_t payloadSize = 0u;
//    //
//    //        mbuf_remove(io, io->len);
//    //        webservice_mcast_build_ssdp_search_response(&payload, &payloadSize);
//    //        mg_send(nc, payload, payloadSize);
//    //        nc->flags |= MG_F_SEND_AND_CLOSE;
//    //
//    //        if (NULL != payload)
//    //        {
//    //            NAL_FREE(payload);
//    //            payload = NULL;
//    //        }
//    //
//    //        break;
//    //    }
//    //    default:
//    //        break;
//    //    }
//}

static NAL_BOOL webserver_mcast_isSsdpDiscoveryRequest(const char *data, const size_t dataSize)
{
    char host[64] = {0};
    char man[64] = {0};
    char mx[64] = {0};

    webserver_utils_parse_http_variable(data, dataSize, "HOST", host, sizeof (host));
    webserver_utils_parse_http_variable(data, dataSize, "MAN", man, sizeof (man));
    webserver_utils_parse_http_variable(data, dataSize, "MX", mx, sizeof (mx));

    printf("%s\n", data);
    return NAL_TRUE;
}

/*---------------------------------- Thread ----------------------------------*/

#define MAXBUFSIZE 65536 // Max UDP Packet size is 64 Kbyte

/**
 * \brief This thread provide a listener on a specific port for multicast exchanges
 */
void webserver_mcast_thread(void * data)
{
    int sock = 0;
    int status = 0;
    int socklen = 0;
    char buffer[MAXBUFSIZE] = {0};
    struct sockaddr_in saddr;
    struct ip_mreq imreq;

    // set content of struct saddr and imreq to zero
    memset(&saddr, 0, sizeof (struct sockaddr_in));
    memset(&imreq, 0, sizeof (struct ip_mreq));

    // open a UDP socket
    sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (sock < 0)
        perror("Error creating socket"), exit(0);

    saddr.sin_family = PF_INET;
    saddr.sin_port = htons(1900); /* atoi(get_WEBSERVER_CONF_MCAST_PORT)*/
    saddr.sin_addr.s_addr = htonl(INADDR_ANY); // bind socket to any interface
    status = bind(sock, (struct sockaddr *) &saddr, sizeof (struct sockaddr_in));

    if (status < 0)
        perror("Error binding socket to interface"), exit(0);

    imreq.imr_multiaddr.s_addr = inet_addr("239.255.255.250"); /*get_WEBSERVER_CONF_MCAST_GROUP */
    imreq.imr_interface.s_addr = INADDR_ANY; // use DEFAULT interface

    /* JOIN multi-cast group on default interface */
    status = setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,
                        (const void *) &imreq, sizeof (struct ip_mreq));

    while (NAL_TRUE == g_webserver_mcast_thread_status)
    {
        socklen = sizeof (struct sockaddr_in);
        
        /* receive packet from socket */
        status = recvfrom(sock, buffer, MAXBUFSIZE, 0,
                          (struct sockaddr *) &saddr, &socklen);

        /* Is an SSDP discovery request? */
        if (webserver_utils_isAnSsdpDiscoveryRequest(buffer, strlen(buffer)))
        {
            size_t ret = 0u; 
            char *payload = NULL;
            size_t payloadSize = 0u;

            webservice_mcast_build_ssdp_search_response(&payload, &payloadSize);

            socklen = sizeof(struct sockaddr_in);
            ret = sendto(sock, payload, payloadSize, 0, (struct sockaddr *) &saddr, socklen);

            if (NULL != payload)
            {
                NAL_FREE(payload);
                payload = NULL;
            }
        }
        
        /* Reset */
        socklen = 0;
        memset(buffer, 0, MAXBUFSIZE);
    }

    /* shutdown socket */
    shutdown(sock, 2);

    /* close socket */
    close(sock);
}
