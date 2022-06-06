#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "com_error.h"
#include "tools_trace.h"
#include "tools_fileManip.h"
#include "nal_mem.h"
#include "com_entity.h"

#include "com_cryptolib_header.h"
#include "cryptoal_hash.h"
#include "cryptoal_signature.h"

#ifdef  CRYPTO_MBEDTLS

/************************  Static functions  *****************************/

#define DEBUG_LEVEL 1

static void cryptoal_my_debug(void *ctx, int level, const char *file, int line, const char *str)
{
    ((void) level);
    mbedtls_fprintf((FILE *) ctx, "%s:%04d: %s", file, line, str);
    fflush((FILE *) ctx);
}

/************************  exported functions  *****************************/

/* Generate signature of the hash of given buffer. Algo are RSA2048 and SHA256.
 * Local certificate and key is used (belonging to us), and we can find it.
 * Signature string is malloced inside, and shall be freed by caller.
 * 
 * return COM_SUCCESS if execution is Ok, otherwise nothing is done.
*/
T_com_status cryptoal_hash_signature(void *input_buffer, unsigned long input_buffer_length, unsigned char ** ppSignature, size_t * pLength)
{
    T_com_status returnCode = COM_ERR_INTERNAL;
    int ret = MBEDTLS_EXIT_FAILURE;
    mbedtls_pk_context pk;
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    unsigned char hash[32 + 1];
    const unsigned char *pers = "mbedtls_pk_sign";
    char * certsFilePath = NULL;
    char string_error[32];
    int ii = 0;

    memset (hash, 0, 32+1);

    START_FUNCTION();

    VERIF_ARGS("input_buffer pointer shall not be Null", input_buffer != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("length of input_buffer  shall not be Null", input_buffer_length != 0u, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("ppSignature pointer shall not be Null", ppSignature != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS(" * ppSignature pointer shall be Null", (* ppSignature) == NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("pLength output pointer  shall not be Null", pLength != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS(" * pLength output value  shall be Null", (* pLength) == 0u, returnCode, COM_ERR_PARAM);
    if (returnCode == COM_ERR_PARAM)
    {
        goto exit;
    }

    /* Mbed context init */
    mbedtls_entropy_init(&entropy);
    mbedtls_ctr_drbg_init(&ctr_drbg);
    mbedtls_pk_init(&pk);

    ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, pers, strlen(pers));
    if (ret != 0)
    {
        TRACE_GEN(WARN, " Mbed sIgning failed, mbedtls_ctr_drbg_seed returned negative code", __FUNCTION__, __LINE__);
        goto exit;
    }

    if (tools_buildFilePath(entmgt_launcher_getAppRootPath(), ENTMGT_DIR_CERTIFICATE_LOCAL, ENTMGT_NAME_KEY_LOCAL, &certsFilePath) != TOOLS_SUCCESS)
    {
        TRACE_GEN(ERROR, "Mbed signing: Build certificate file path failed", __FUNCTION__, __LINE__);
        goto exit;
    }
    ret = mbedtls_pk_parse_keyfile(& pk, certsFilePath, "devsec");
    if (ret != 0)
    {
        TRACE_GEN(WARN, " Mbed sIgning failed, mbedtls_pk_parse_keyfile returned bad code", __FUNCTION__, __LINE__);
        goto exit;
    }

    /*
     * Compute the SHA-256 hash of the input file,  then calculate the signature of the hash.
     */
    ret = mbedtls_md(mbedtls_md_info_from_type(MBEDTLS_MD_SHA256), input_buffer, input_buffer_length, hash);
    if (ret != 0)
    {
        TRACE_GEN(WARN, " Mbed sIgning failed, mbedtls_md returned bad code", __FUNCTION__, __LINE__);
        goto exit;
    }


    printf ("\nHash:  \n");
    for (ii=0; ii<32+1; ii++)
        printf ( "%02x ", hash[ii] );
    printf ("\n\n");



    * ppSignature = malloc(MBEDTLS_PK_SIGNATURE_MAX_SIZE + 1);
    if (NULL == *ppSignature)
    {
        TRACE_GEN(WARN, "Mbed signing; failed to malloc buffer", __FUNCTION__, __LINE__);
        goto exit;
    }

    ret = mbedtls_pk_sign(& pk, MBEDTLS_MD_SHA256, hash, 0, * ppSignature, pLength, mbedtls_ctr_drbg_random, & ctr_drbg);
    if (ret != 0)
    {
        TRACE_GEN(WARN, " Mbed sIgning failed, mbedtls_pk_sign returned bad code", __FUNCTION__, __LINE__);
        goto exit;
    }

    /* Only here success is reached */
    returnCode = COM_SUCCESS;

exit:

    mbedtls_pk_free(&pk);
    mbedtls_ctr_drbg_free(&ctr_drbg);
    mbedtls_entropy_free(&entropy);

    if (COM_SUCCESS != returnCode)
    {
#if defined(MBEDTLS_ERROR_C)
        mbedtls_strerror(ret, string_error, sizeof (string_error));
        TRACE_GEN(TRACE, string_error, __FUNCTION__, __LINE__);
#endif
        if  (NULL != *ppSignature)
            NAL_FREE(*ppSignature);
    }

    END_FUNCTION();
    return returnCode;
}

/* Check if given signature is related to the hash of given buffer. Algo are RSA2048 and SHA256.
 * External certificate is used (belonging to the nomad tool managing us, or to the CA authority), and we can find it.
 * So 2 cases of signature that we verify here: an Rbac given by nomad tool, or a patch given by the factory, that's why 3rd
 * param is a boolen: true for a patch to verify with CA-ROOT certificate, false for a conf or rbac from nomad toolthat is an intermediate CA.
 * Signature string come from the outside, nothing is done on it (not freed nor erased).
 * 
 * return COM_SUCCESS if execution is Ok and in this case (*pValid) is written (true if signature is valid), otherwise nothing is done.
*/
T_com_status cryptoal_verify_hash_signature( void *input_buffer, unsigned long input_buffer_length, bool ca_root,
                                     unsigned char * pSignature, size_t sign_len, bool * pValid)
{

    T_com_status returnCode = COM_SUCCESS;
    T_tools_status retCode = TOOLS_SUCCESS;
    int ret = 1;
    size_t i;
    mbedtls_x509_crt clicert;
    mbedtls_pk_context * pk = NULL;
    unsigned char hash[32 + 1];
    char string_error[32];
    char * certsFilePath = NULL;

    START_FUNCTION();

    VERIF_ARGS("input_buffer pointer shall not be Null", input_buffer != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("length of input_buffer  shall not be Null", input_buffer_length != 0u, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("pSignature pointer shall not be Null", pSignature != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("sign_len  shall not be Null", sign_len != 0u, returnCode, COM_ERR_PARAM);
    VERIF_ARGS(" pValid pointer  shall be Null", pValid != NULL, returnCode, COM_ERR_PARAM);
    if (returnCode == COM_ERR_PARAM)
    {
        goto exit;
    }

    mbedtls_x509_crt_init( & clicert );
    //mbedtls_pk_init(&pk);

    if (true == ca_root)
        retCode = tools_buildFilePath(entmgt_launcher_getAppRootPath(), ENTMGT_DIR_CERTIFICATE_CAROOT, ENTMGT_NAME_CERTIFICATE_CAROOT, &certsFilePath);
    else
        retCode = tools_buildFilePath(entmgt_launcher_getAppRootPath(), ENTMGT_DIR_CERTIFICATE_TOOL, ENTMGT_NAME_CERTIFICATE_TOOL, &certsFilePath);

    if (retCode != TOOLS_SUCCESS)
    {
        TRACE_GEN(ERROR, "Mbed verifying signature failed: Build certificate file path failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_SSL_CERTIFICATE_ERROR;
        goto exit;
    }

    ret = mbedtls_x509_crt_parse_file( & clicert, certsFilePath);
    if( ret != 0 )
    {
        TRACE_GEN (WARN, "Unable to parse certificate for signature verification, mbedtls_x509_crt_parse_file failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_SSL_CERTIFICATE_ERROR;
        goto exit;
    }

    pk = & clicert.pk;

    /* compute hash on clear text: */
    ret = mbedtls_md(mbedtls_md_info_from_type(MBEDTLS_MD_SHA256), input_buffer, input_buffer_length, hash);
    if (ret != MBEDTLS_EXIT_SUCCESS)
    {
        TRACE_GEN(WARN, " Mbed verifying signature:  failed to hash input buffer", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_SSL_CRYPTO_ERROR;
        goto exit;
    }

    ret = mbedtls_pk_verify( pk, MBEDTLS_MD_SHA256, hash, 0, pSignature, sign_len);
    if (ret != MBEDTLS_EXIT_SUCCESS)
    {
        * pValid = false;
    }
    else
    {
        * pValid = true;
    }

exit:

#if defined(MBEDTLS_ERROR_C)
    if (returnCode != COM_SUCCESS)
    {
        mbedtls_strerror(ret, string_error, sizeof (string_error));
        TRACE_GEN(TRACE, string_error, __FUNCTION__, __LINE__);
    }
#endif

    END_FUNCTION();
    return returnCode;
}


#endif  // CRYPTO_MBEDTLS

