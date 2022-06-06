#include <stdbool.h>
#include <stddef.h>
#include <math.h>
#include <string.h>

#include "com_error.h"
#include "com_entity.h"
#include "tools_trace.h"
#include "nal_mem.h"

#include "cryptoal_base64.h"

#ifdef CRYPTO_MBEDTLS

#include "mbedtls/config.h"
#include "mbedtls/base64.h"

/*
 * Encode buffer with given size.
 * outData buffer is malloced here and returned, so caller has to free it. Size of outed data is returned too.
 * 
 * Return 0 if success, otherwise nothing is done
 */
int cryptoal_base64Encode(const char *inData, const size_t inDataSize, char **outData, size_t *outDataSize)
{
    T_com_status returnCode = COM_SUCCESS;

    unsigned char * dst_buffer;
    size_t olen = 0u;
    
    START_FUNCTION();

    /* Sanity check */
    VERIF_ARGS("inData", inData != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("inDataSize", inDataSize != 0u, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("outData", outData != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("*outData", *outData == NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("outDataSize", outDataSize != NULL, returnCode, COM_ERR_PARAM);
    if (returnCode != COM_SUCCESS)
    {
        TRACE_GEN(ERROR, "Invalid parameter", __FUNCTION__, __LINE__);
        goto exit;
    }


    /* just to get size of result, w/o writing it */
    int ret = mbedtls_base64_encode ( NULL , 0u,  & olen, (const unsigned char *) inData, inDataSize );
    if (MBEDTLS_ERR_BASE64_BUFFER_TOO_SMALL != ret)
    {
        returnCode = COM_ERR_INTERNAL;
        TRACE_GEN(ERROR, "base64 encode error to get size", __FUNCTION__, __LINE__);
        goto exit;
    }

    dst_buffer = NAL_MALLOC (olen + 1);
    if (NULL == dst_buffer)
    {
        returnCode = COM_ERR_NO_MEMORY;
        TRACE_GEN(ERROR, "base64 encode error to malloc size", __FUNCTION__, __LINE__);
        goto exit;
    }

    ret = mbedtls_base64_encode ( dst_buffer, olen+1,  & olen, (const unsigned char *) inData, inDataSize );
    if (0 != ret)
    {
        returnCode = COM_ERR_INTERNAL;
        TRACE_GEN(ERROR, "base64 encode error to encode input buffer", __FUNCTION__, __LINE__);
        goto exit;
    }

    * outData = dst_buffer;
    * outDataSize = olen;

exit:

    END_FUNCTION();
    return returnCode;
}

/*
 * Decode a base64 buffer finishing with 0, no input size is needed since base64 data cannot be 0.
 * outData buffer is malloced here and returned, so caller has to free it. Size of outed data is returned too.
 * 
 * Return 0 if success, otherwise nothing is done
 */
int cryptoal_base64Decode(const char *inData, char **outData, size_t *outDataSize)
{
    T_com_status returnCode = COM_SUCCESS;

    size_t decodedSize = 0u;
    size_t readingSize = 0u;

    START_FUNCTION();

    /* Sanity check */
    VERIF_ARGS("inData", inData != NULL, returnCode, COM_SUCCESS);
    VERIF_ARGS("outData", outData != NULL, returnCode, COM_SUCCESS);
    VERIF_ARGS("*outData", *outData == NULL, returnCode, COM_SUCCESS);
    VERIF_ARGS("*outDataSize", outDataSize != NULL, returnCode, COM_SUCCESS);
    if (returnCode != COM_SUCCESS)
    {
        TRACE_GEN(TRACE, "Invalid parameter", __FUNCTION__, __LINE__);
        goto exit;
    }

    unsigned char * dst_buffer = NULL;
    size_t olen = 0u;
    int ret = mbedtls_base64_decode ( NULL, 0u,  & olen, (const unsigned char *) inData, strlen(inData) );
    if (MBEDTLS_ERR_BASE64_BUFFER_TOO_SMALL != ret)
    {
        returnCode = COM_ERR_INTERNAL;
        TRACE_GEN(ERROR, "base64 decode error to get size", __FUNCTION__, __LINE__);
        goto exit;
    }
    
    dst_buffer = NAL_MALLOC (olen + 1);
    if (NULL == dst_buffer)
    {
        returnCode = COM_ERR_NO_MEMORY;
        TRACE_GEN(ERROR, "base64 decode error to malloc size", __FUNCTION__, __LINE__);
        goto exit;
    }

    ret = mbedtls_base64_decode ( dst_buffer, olen+1,  & olen, (const unsigned char *) inData, strlen(inData) );
    if (0 != ret)
    {
        returnCode = COM_ERR_INTERNAL;
        TRACE_GEN(ERROR, "base64 decode error to decode input buffer", __FUNCTION__, __LINE__);
        goto exit;
    }

    * outData = dst_buffer;
    * outDataSize = olen;

exit:

    if ( (COM_SUCCESS != returnCode) && (NULL != (*outData)) )
    {
        NAL_FREE (*outData);
        *outData = NULL;
        *outDataSize = 0u;
    }

    END_FUNCTION();
    return returnCode;
}

#endif  /*CRYPTO_MBEDTLS*/