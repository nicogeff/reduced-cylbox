#include <math.h>
#include <string.h>

#include "com_error.h"
#include "com_entity.h"

#include "tools_trace.h"
#include "nal_mem.h"

#include "cryptoal_base64.h"

#ifdef CRYPTO_OPENSSL

#include "openssl/evp.h"
#include "openssl/bio.h"
#include "openssl/buffer.h"


int cryptoal_base64Encode(const char *inData, const size_t inDataSize, char **outData, size_t *outDataSize)
{
    int returnCode = CRYPTOAL_SUCCESS;

    BIO *bio = NULL;
    BIO *b64 = NULL;

    BUF_MEM *pData = NULL;

    START_FUNCTION();

    /* Sanity check */
    VERIF_ARGS("inData", inData != NULL, returnCode, CRYPTOAL_EPARAM);
    VERIF_ARGS("outData", outData != NULL, returnCode, CRYPTOAL_EPARAM);
    VERIF_ARGS("*outData", *outData == NULL, returnCode, CRYPTOAL_EPARAM);
    VERIF_ARGS("*outDataSize", outDataSize != NULL, returnCode, CRYPTOAL_EPARAM);
    if (returnCode != CRYPTOAL_SUCCESS)
    {
        TRACE_GEN(TRACE, "Invalid parameter", __FUNCTION__, __LINE__);
        goto exit;
    }

    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);

    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
    BIO_write(bio, inData, inDataSize);
    BIO_flush(bio);
    BIO_get_mem_ptr(bio, &pData);
    BIO_set_close(bio, BIO_NOCLOSE);

    (*outData) = pData->data;
    (*outDataSize) = (*pData).length;

exit:

    BIO_free_all(bio);

    END_FUNCTION();
    return returnCode;
}

int cryptoal_base64Decode(const char *inData, char **outData, size_t *outDataSize)
{
    int returnCode = CRYPTOAL_SUCCESS;

    BIO *bio = NULL;
    BIO *b64 = NULL;
    size_t decodedSize = 0u;
    size_t readingSize = 0u;

    START_FUNCTION();

    /* Sanity check */
    VERIF_ARGS("inData", inData != NULL, returnCode, CRYPTOAL_EPARAM);
    VERIF_ARGS("outData", outData != NULL, returnCode, CRYPTOAL_EPARAM);
    VERIF_ARGS("*outData", *outData == NULL, returnCode, CRYPTOAL_EPARAM);
    VERIF_ARGS("*outDataSize", outDataSize != NULL, returnCode, CRYPTOAL_EPARAM);
    if (returnCode != CRYPTOAL_SUCCESS)
    {
        TRACE_GEN(TRACE, "Invalid parameter", __FUNCTION__, __LINE__);
        goto exit;
    }

    /* Get size of inData when it will be decoded */
    decodedSize = crypoal_base64DecodedSize(inData);

    /* Allocate output parameter outData */
    *outData = NAL_MALLOC(decodedSize * sizeof (char) + 1);
    if (NULL == *outData)
    {
        returnCode = CRYPTOAL_ERROR;
        goto exit;
    }
    memset(*outData, 0, decodedSize * sizeof (char) + 1);
    
    bio = BIO_new_mem_buf(inData, -1);
    b64 = BIO_new(BIO_f_base64());
    bio = BIO_push(b64, bio);

    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
    readingSize = BIO_read(bio, *outData, strlen(inData));

    if (decodedSize != readingSize)
    {
        returnCode = CRYPTOAL_ERROR;
        goto exit;
    }

    *outDataSize = readingSize;

exit:

    BIO_free_all(bio);

    if (CRYPTOAL_SUCCESS != returnCode && NULL != (*outData))
    {
        NAL_FREE(*outData);
        *outData = NULL;
        *outDataSize = 0u;
    }

    END_FUNCTION();
    return returnCode;
}

size_t crypoal_base64DecodedSize(const char *inData)
{
    size_t len = 0u;
    size_t padding = 0u;
    size_t returnSize = 0u;

    START_FUNCTION();

    /* Sanity check */
    if (NULL == inData)
        goto exit;

    len = strlen(inData);

    if (inData[len - 1] == '=' && inData[len - 2] == '=')
        padding = 2;
    else if (inData[len - 1] == '=')
        padding = 1;

    returnSize = ((len * 3) / 4) - padding;

exit:

    END_FUNCTION();
    return returnSize;
}

#endif /* CRYPTO_OPENSSL */
