
#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Basic.h>

#include "nal_common.h"
#include "nal_net.h"
#include "nal_mem.h"

#include "nal_net_tests.h"

void nal_test_net_getNetifsInfo(void)
{
    int result = NAL_SUCCESS;

    T_nal_netifInfo *netifs = NULL;
    int nb_netifs = 0;
    char *ipv4Str = NULL;
    size_t ipv4StrSize = 0u;

    result = nal_getNetifsInfo(&netifs, &nb_netifs);
    CU_ASSERT_EQUAL(result, NAL_SUCCESS);
    CU_ASSERT_NOT_EQUAL(nb_netifs, 0);
    CU_ASSERT_PTR_NOT_NULL(netifs);

    result = nal_convertSockAddrToString(netifs[1].addrs[0], &ipv4Str, &ipv4StrSize);
    CU_ASSERT_EQUAL(result, NAL_SUCCESS);
    CU_ASSERT_PTR_NOT_NULL(ipv4Str);

    /* Release buffer */
    NAL_FREE(ipv4Str);

    /* Release netif structure */
    result = nal_releaseNetifsInfo(netifs, nb_netifs);
    CU_ASSERT_EQUAL(result, NAL_SUCCESS);
}