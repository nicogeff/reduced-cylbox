#include <stdio.h>
#include <stdint.h>

#include "tools_trace.h"
#include "nal_mem.h"
#include "cryptoal_rand.h"

#ifdef CRYPTO_OPENSSL

#include "openssl/rand.h"

/**
 * @brief This function generate a UUID according to RFC-4122
 * @note Using OpenSSL RAND_bytes function to generate a UUID
 * @param buffer A buffer that is at least 38 bytes long 
 * @return 
 */
int cryptoal_uuidgen(char **buffer, size_t *bufferSize)
{
    int returnCode = CRYPTOAL_SUCCESS;
    size_t writeSize = 0u;

    union
    {
        struct
        {
            uint32_t time_low;
            uint16_t time_mid;
            uint16_t time_hi_and_version;
            uint8_t clk_seq_hi_res;
            uint8_t clk_seq_low;
            uint8_t node[6];
        };
        uint8_t __rnd[16];
    } uuid;


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

    if (RAND_bytes(uuid.__rnd, sizeof (uuid)) != 1)
    {
        TRACE_GEN(TRACE, "RAND_bytes failed", __FUNCTION__, __LINE__);
        returnCode = CRYPTOAL_ERROR;
        goto exit;
    }

    /** 
     * Refer Section 4.2 of RFC-4122
     * https://tools.ietf.org/html/rfc4122#section-4.2
     */
    uuid.clk_seq_hi_res = (uint8_t) ((uuid.clk_seq_hi_res & 0x3F) | 0x80);
    uuid.time_hi_and_version = (uint16_t) ((uuid.time_hi_and_version & 0x0FFF) | 0x4000);

    /* Allocate buffer */
    *buffer = NAL_MALLOC(CRYPTOAL_UUID_SIZE * sizeof (char) + 1);
    if (NULL == *buffer)
    {        
        returnCode = CRYPTOAL_ERROR;
        goto exit;
    }
    memset(*buffer, 0, CRYPTOAL_UUID_SIZE * sizeof (char) + 1);

    nal_snprintf_s(*buffer, CRYPTOAL_UUID_SIZE, CRYPTOAL_UUID_SIZE, bufferSize,
                   "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
                   uuid.time_low, uuid.time_mid, uuid.time_hi_and_version,
                   uuid.clk_seq_hi_res, uuid.clk_seq_low,
                   uuid.node[0], uuid.node[1], uuid.node[2],
                   uuid.node[3], uuid.node[4], uuid.node[5]);
     
exit:
    
    if(CRYPTOAL_SUCCESS != returnCode && NULL != buffer && NULL != (*buffer))
    {
        NAL_FREE (*buffer);
        *bufferSize = 0u;
    }

    return returnCode;
}

#endif  /* CRYPTO_OPENSSL */

