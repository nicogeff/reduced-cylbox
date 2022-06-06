
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <CUnit/Basic.h>

#include "nal_mem.h"

#include "cryptoal_rand.h"

#include "cryptoal_rand_tests.h"

/* */
void cryptoal_test_rand_uuidgen(void)
{
    int returnCode = CRYPTOAL_SUCCESS;
    char *uuid = NULL;
    size_t uuidSize = 0u;
    int ii = 0;

    returnCode = cryptoal_uuidgen(&uuid, &uuidSize);
    CU_ASSERT_EQUAL(returnCode, CRYPTOAL_SUCCESS);
    CU_ASSERT_PTR_NOT_NULL(uuid);
    CU_ASSERT_NOT_EQUAL(uuidSize, 0u);
    printf("\nuuidgen généré: %s\n", uuid);

    uuid = NULL; uuidSize = 0;
    returnCode = cryptoal_uuidgen(&uuid, &uuidSize);
    CU_ASSERT_EQUAL(returnCode, CRYPTOAL_SUCCESS);
    CU_ASSERT_PTR_NOT_NULL(uuid);
    CU_ASSERT_NOT_EQUAL(uuidSize, 0u);
    printf("another uuidgen généré: %s\n", uuid);


    if (NULL != uuid)
    {
        NAL_FREE(uuid);
        uuid = NULL;
    }
}

void cryptoal_test_rand_uuidgen_defensive(void)
{
    int returnCode = CRYPTOAL_SUCCESS;
    char *uuid = NULL;
    size_t uuidSize = 0u;

    returnCode = cryptoal_uuidgen(NULL, &uuidSize);
    CU_ASSERT_EQUAL(returnCode, CRYPTOAL_EPARAM);
    CU_ASSERT_PTR_NULL(uuid);
    CU_ASSERT_EQUAL(uuidSize, 0u);

    returnCode = cryptoal_uuidgen(NULL, NULL);
    CU_ASSERT_EQUAL(returnCode, CRYPTOAL_EPARAM);
    CU_ASSERT_PTR_NULL(uuid);
    CU_ASSERT_EQUAL(uuidSize, 0u);

    returnCode = cryptoal_uuidgen(&uuid, NULL);
    CU_ASSERT_EQUAL(returnCode, CRYPTOAL_EPARAM);
    CU_ASSERT_PTR_NULL(uuid);
    CU_ASSERT_EQUAL(uuidSize, 0u);
}

/*************** Bouchons for tests ****************/
char theRootPathOfResources[] = "/tmp/";

char * entmgt_launcher_getAppRootPath()
{
    return theRootPathOfResources;
}

