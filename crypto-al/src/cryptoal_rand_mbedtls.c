#include <stdio.h>
#include <stdint.h>

#include "com_error.h"
#include "com_entity.h"

#include "tools_trace.h"
#include "nal_mem.h"

#include "cryptoal_rand.h"

#include "com_cryptolib_header.h"

#ifdef CRYPTO_MBEDTLS

//#include "mbedtls/entropy.h"
//#include "mbedtls/ctr_drbg.h"

/**
 * @brief This function generate a UUID according to RFC-4122 with format: urn:uuid:f81d4fae-7dec-11d0-a765-00a0c91e6bf6
 * @note Using OpenSSL RAND_bytes function to generate a UUID
 * @param buffer A buffer that is at least 38 bytes long 
 * @return CRYPTOAL_SUCCESS if success, otherwise nothing is done
 */
#define  UUID_RANDOM_BYTES_TO_GENERATE  16
int cryptoal_uuidgen(char **buffer, size_t *bufferSize)
{
    int returnCode = CRYPTOAL_SUCCESS;

    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
   unsigned char buf[UUID_RANDOM_BYTES_TO_GENERATE+1];
     int ret = 0;
    
    // T_nal_datetime datetime = {0};
    // char *datetimestr = NULL;
    // size_t datetimestrSize = 0u;
    
    /* Get time */
    // nal_getUtcTime(&datetime);
    // nal_convertTimeToString(&datetime, &datetimestr, &datetimestrSize);

    /* @todo need to change to by using the datetime (with ms), same format as UUID so 16 random bytes  */
    char *personalization = "ed375f88-8822-40a5-b179-066564c1968e";

    START_FUNCTION();

    /* Sanity check */
    VERIF_ARGS("buffer", buffer != NULL, returnCode, CRYPTOAL_EPARAM);
    VERIF_ARGS("*buffer", *buffer == NULL, returnCode, CRYPTOAL_EPARAM);
    VERIF_ARGS("bufferSize", bufferSize != NULL, returnCode, CRYPTOAL_EPARAM);
    if (returnCode != CRYPTOAL_SUCCESS)
    {
        TRACE_GEN(TRACE, "Invalid parameter", __FUNCTION__, __LINE__);
        goto exit;
    }

    /* Init. entropy */
    mbedtls_entropy_init(&entropy);
    mbedtls_ctr_drbg_init(&ctr_drbg);

    mbedtls_ctr_drbg_set_prediction_resistance( &ctr_drbg, MBEDTLS_CTR_DRBG_PR_ON );
    if (mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy,
                              (const unsigned char *) personalization, strlen(personalization)) != 0)
    {
        TRACE_GEN(WARN, "mbedtls_ctr_drbg_seed failed", __FUNCTION__, __LINE__);
        returnCode = CRYPTOAL_ERROR;
        goto exit;
    }

    /* Allocate buffer */
    *buffer = NAL_MALLOC(CRYPTOAL_UUID_SIZE * sizeof (char) + 1);
    if (NULL == *buffer)
    {
        returnCode = CRYPTOAL_ERROR;
        goto exit;
    }
    memset(*buffer, 0, CRYPTOAL_UUID_SIZE * sizeof (char) + 1);

    ret = mbedtls_entropy_func( &entropy, buf, UUID_RANDOM_BYTES_TO_GENERATE /*sizeof( buf )*/ );
    if( ret != 0 )
    {
        TRACE_GEN(WARN, "mbedtls_entropy_func failed", __FUNCTION__, __LINE__);
        returnCode = CRYPTOAL_ERROR;
        goto exit;
    }

    nal_snprintf_s(*buffer, CRYPTOAL_UUID_SIZE, CRYPTOAL_UUID_SIZE, bufferSize,
                   "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
                  buf[0], buf[1],  buf[2], buf[3], buf[4], buf[5], buf[6], buf[7], buf[8], buf[9], buf[10], buf[11], buf[12], buf[13], buf[14], buf[15] );

exit:

    if ( (CRYPTOAL_SUCCESS != returnCode) && ((NULL != buffer) && (NULL != (*buffer))) )
    {
        NAL_FREE(*buffer);
        *buffer = NULL;
        *bufferSize = 0u;
    }
    // if (NULL != datetimestr)
    // {
    //    NAL_FREE(datetimestr);
    //    datetimestr = NULL;
    // }

    return returnCode;
}

#endif  /* CRYPTO_MBEDTLS */
