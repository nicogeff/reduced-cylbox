#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "com_error.h"
#include "tools_trace.h"
#include "tools_fileManip.h"
#include "nal_mem.h"
#include "nal_stor.h"
#include "com_entity.h"

#include "com_cryptolib_header.h"
#include "cryptoal_hash.h"
#include "cryptoal_certificate.h"

#ifdef  CRYPTO_MBEDTLS

/************************  Static functions  *****************************/

#define DEBUG_LEVEL 1

static void cryptoal_my_debug(void *ctx, int level, const char *file, int line, const char *str)
{
    ((void) level);
    mbedtls_fprintf((FILE *) ctx, "%s:%04d: %s", file, line, str);
    fflush((FILE *) ctx);
}


//=====================  Local function used to generate key file or certificate file ======================/

/* May be reduced wen using Ellipic curve keys */
#define SIZE_KEY_CERTIFICATE_BUFFER 4096

/* Important notice: mbedtls DO NOT support keyfile protection by a password when writing keyfile. */
static int cryptoal_write_private_key(mbedtls_pk_context *key, const char *output_file)
{
    int ret = 0;
    FILE *fd;
    unsigned char output_buf[SIZE_KEY_CERTIFICATE_BUFFER];
    unsigned char *c = output_buf;
    size_t len = 0;

    memset(output_buf, 0, SIZE_KEY_CERTIFICATE_BUFFER);
    ret = mbedtls_pk_write_key_pem(key, output_buf, SIZE_KEY_CERTIFICATE_BUFFER);
    if (0 != ret)
    {
        goto exit;
        ;
    }
    len = strlen((char *) output_buf);

    if ((fd = fopen(output_file, "wb")) == NULL)
    {
        ret = -1;
        goto exit;
        ;
    }

    if (fwrite(c, 1, len, fd) != len)
    {
        fclose(fd);
        ret = -1;
        goto exit;
        ;
    }

    fclose(fd);

exit:
    return ( ret);
}

static int cryptoal_write_certificate(mbedtls_x509write_cert *crt, const char *output_file,
                             int (*f_rng)(void *, unsigned char *, size_t), void *p_rng)
{
    int ret = 0;
    FILE * fd;
    unsigned char output_buf[SIZE_KEY_CERTIFICATE_BUFFER];
    size_t len = 0;

    memset(output_buf, 0, SIZE_KEY_CERTIFICATE_BUFFER);
    ret = mbedtls_x509write_crt_pem(crt, output_buf, SIZE_KEY_CERTIFICATE_BUFFER, f_rng, p_rng);
    if (ret < 0)
    {
        goto exit;
        ;
    }

    len = strlen((char *) output_buf);

    if ((fd = fopen(output_file, "w")) == NULL)
    {
        ret = -1;
        goto exit;
        ;
    }

    if (fwrite(output_buf, 1, len, fd) != len)
    {
        fclose(fd);
        ret = -1;
        goto exit;
        ;
    }

    fclose(fd);

exit:
    return ( ret);
}

#if 0
static int my_verify( void *data, mbedtls_x509_crt *crt, int depth, uint32_t *flags )
{
    char buf[1024];
    ((void) data);

    mbedtls_printf( "\nVerify requested for (Depth %d):\n", depth );
    mbedtls_x509_crt_info( buf, sizeof( buf ) - 1, "", crt );
    mbedtls_printf( "%s", buf );

    if ( ( *flags ) == 0 )
        mbedtls_printf( "  This certificate has no flags\n" );
    else
    {
        mbedtls_x509_crt_verify_info( buf, sizeof( buf ), "  ! ", *flags );
        mbedtls_printf( "%s\n", buf );
    }

    return( 0 );
}
#endif

/*============================   GLobal functions   =============================*/


/* Suppress all certificate and keypair of the CA chain. We know its structure.
 * To bo called when CA is failing, and then generate an autosigned certificate in degraded mode.
 * 
 * returns nothing.
 */
void cryptoal_ca_chain_clean_certificates (void)
{
    T_com_status returnCode = COM_SUCCESS;
    int ret = 0;
    const char * rootPath = entmgt_launcher_getAppRootPath();
    char * outFilePath = NULL;

    START_FUNCTION();

    /* Suppress Local cert file if one */
    if (tools_buildFilePath(rootPath, ENTMGT_DIR_CERTIFICATE_LOCAL, ENTMGT_NAME_CERTIFICATE_LOCAL, &outFilePath) != TOOLS_SUCCESS)
    {
        TRACE_GEN(ERROR, "Building local certificate file path failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_NO_MEMORY;
        goto exit;     
    }
    ret = nal_streamRemove (outFilePath);
    if (NAL_SUCCESS != ret)
    {
        TRACE_GEN(TRACE, "Unable to delete local certificate, it may not exist", __FUNCTION__, __LINE__);
    }
    NAL_FREE(outFilePath);

    /* Suppress Local keypair file if one */
    if (tools_buildFilePath(rootPath, ENTMGT_DIR_CERTIFICATE_LOCAL, ENTMGT_NAME_KEY_LOCAL, &outFilePath) != TOOLS_SUCCESS)
    {
        TRACE_GEN(ERROR, "Building local certificate file path failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_NO_MEMORY;
        goto exit;     
    }
    ret = nal_streamRemove (outFilePath);
    if (NAL_SUCCESS != ret)
    {
        TRACE_GEN(TRACE, "Unable to delete local certificate, it may not exist", __FUNCTION__, __LINE__);
    }
    NAL_FREE(outFilePath);


    /* Suppress TOOL cert file if one */
    if (tools_buildFilePath(rootPath, ENTMGT_DIR_CERTIFICATE_TOOL, ENTMGT_NAME_CERTIFICATE_TOOL, &outFilePath) != TOOLS_SUCCESS)
    {
        TRACE_GEN(ERROR, "Building local certificate file path failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_NO_MEMORY;
        goto exit;     
    }
    ret = nal_streamRemove (outFilePath);
    if (NAL_SUCCESS != ret)
    {
        TRACE_GEN(TRACE, "Unable to delete TOOL certificate, it may not exist", __FUNCTION__, __LINE__);
    }
    NAL_FREE(outFilePath);

    /* Suppress CLOUD cert file if one */
    if (tools_buildFilePath(rootPath, ENTMGT_DIR_CERTIFICATE_CLOUD, ENTMGT_NAME_CERTIFICATE_CLOUD, &outFilePath) != TOOLS_SUCCESS)
    {
        TRACE_GEN(ERROR, "Building local certificate file path failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_NO_MEMORY;
        goto exit;     
    }
    ret = nal_streamRemove (outFilePath);
    if (NAL_SUCCESS != ret)
    {
        TRACE_GEN(TRACE, "Unable to delete CLOUD certificate, it may not exist", __FUNCTION__, __LINE__);
    }
    NAL_FREE(outFilePath);

    /* Suppress CA ROOT cert file if one */
    if (tools_buildFilePath(rootPath, ENTMGT_DIR_CERTIFICATE_CAROOT, ENTMGT_NAME_CERTIFICATE_CAROOT, &outFilePath) != TOOLS_SUCCESS)
    {
        TRACE_GEN(ERROR, "Building local certificate file path failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_NO_MEMORY;
        goto exit;     
    }
    ret = nal_streamRemove (outFilePath);
    if (NAL_SUCCESS != ret)
    {
        TRACE_GEN(TRACE, "Unable to delete CA ROOT certificate, it may not exist", __FUNCTION__, __LINE__);
    }
    NAL_FREE(outFilePath);


exit:
    END_FUNCTION();
    return;
}

  

/* Generate local certificate file and public/private keys in 2 files. Algo is RSA2048 with SHA256, used for signing and TLS client or server.
 * We have all information inside it, and name an path of files.
 * This is done when no certificate exists at init, it will be replaced by a signed one when enrolled by the nomad tool.
 * It is assumed target directory exists.
 * 
 * return COM_SUCCESS if execution is Ok,  otherwise nothing is done.
*/
T_com_status cryptoal_generate_autosigned_local_certificate_and_keys ()
{
    T_com_status returnCode = COM_SUCCESS;
    int ret = 0;
    int ret2 = 0;
    const char * rootPath = entmgt_launcher_getAppRootPath();
    char * cryptoCertificatePath = NULL;
    char * cryptoKeyPath = NULL;

    /* For keys: */
    mbedtls_pk_context key;
    char buf[64];
    char *p, *q, *r;
    mbedtls_mpi N, P, Q, D, E, DP, DQ, QP;
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    const unsigned char *pers = "gen_key_certificate_autosigned";

    /* For certificate: */
    int exit_code = MBEDTLS_EXIT_FAILURE;
    mbedtls_x509_crt issuer_crt;
    mbedtls_pk_context loaded_issuer_key, loaded_subject_key;
    mbedtls_pk_context * issuer_key = &loaded_issuer_key;
    mbedtls_pk_context * subject_key = &loaded_subject_key;
    int i;

#if defined(MBEDTLS_X509_CSR_PARSE_C)
    mbedtls_x509_csr csr;
#endif
    mbedtls_x509write_cert crt;
    mbedtls_mpi serial;

    START_FUNCTION();

    if (tools_buildFilePath(rootPath, ENTMGT_DIR_CERTIFICATE_LOCAL, ENTMGT_NAME_CERTIFICATE_LOCAL, & cryptoCertificatePath) != TOOLS_SUCCESS)
    {
        TRACE_GEN(ERROR, "Building crypto folder path failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_NO_MEMORY;
        goto exit;
    }
    if (tools_buildFilePath(rootPath, ENTMGT_DIR_CERTIFICATE_LOCAL, ENTMGT_NAME_KEY_LOCAL, & cryptoKeyPath) != TOOLS_SUCCESS)
    {
        TRACE_GEN(ERROR, "Building crypto folder path failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_NO_MEMORY;
        goto exit;
    }

    /* Init values for keys */
    mbedtls_mpi_init(&N);
    mbedtls_mpi_init(&P);
    mbedtls_mpi_init(&Q);
    mbedtls_mpi_init(&D);
    mbedtls_mpi_init(&E);
    mbedtls_mpi_init(&DP);
    mbedtls_mpi_init(&DQ);
    mbedtls_mpi_init(&QP);
    mbedtls_pk_init(&key);
    mbedtls_entropy_init(&entropy);
    mbedtls_ctr_drbg_init(&ctr_drbg);
    (void) memset(buf, 0, sizeof ( buf));

    /* Init values for certificate */
    mbedtls_x509write_crt_init(&crt);
    mbedtls_pk_init(&loaded_issuer_key);
    mbedtls_pk_init(&loaded_subject_key);
    mbedtls_mpi_init(&serial);
    mbedtls_x509_crt_init(&issuer_crt);
#if defined(MBEDTLS_X509_CSR_PARSE_C)
    mbedtls_x509_csr_init(&csr);
#endif

    ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, pers, strlen(pers));
    if (0 != ret)
    {
        mbedtls_printf(" failed in mbedtls_ctr_drbg_seed, returned -0x%04x\n", -ret);
        returnCode = COM_ERR_SSL_CERTIFICATE_ERROR;
        goto exit;
    }

    /* Generate the key */
    ret = mbedtls_pk_setup(&key, mbedtls_pk_info_from_type(MBEDTLS_PK_RSA));
    if (0 != ret)
    {
        mbedtls_printf(" failed in  mbedtls_pk_setup, returned -0x%04x", -ret);
        returnCode = COM_ERR_SSL_CERTIFICATE_ERROR;
        goto exit;
    }

    mbedtls_rsa_context * rsa = mbedtls_pk_rsa(key);
    ret = mbedtls_rsa_gen_key(rsa, mbedtls_ctr_drbg_random, &ctr_drbg, 2048, 65537);
    if (ret != 0)
    {
        mbedtls_printf(" failed in mbedtls_rsa_gen_key, returned -0x%04x", -ret);
        returnCode = COM_ERR_SSL_CERTIFICATE_ERROR;
        goto exit;
    }

    ret = mbedtls_rsa_export(rsa, &N, &P, &Q, &D, &E);
    ret2 = mbedtls_rsa_export_crt(rsa, &DP, &DQ, &QP);
    if ((0 != ret) || (0 != ret2))
    {
        mbedtls_printf(" failed, could not export RSA parameters\n\n");
        returnCode = COM_ERR_SSL_CERTIFICATE_ERROR;
        goto exit;
    }

    mbedtls_printf("  . Writing key to file...");
    ret =cryptoal_write_private_key(& key, cryptoKeyPath);
    if (0 != ret)
    {
        mbedtls_printf(" failed to write keys file, in cryptoal_write_private_key returned -0x%04x", -ret);
        returnCode = COM_ERR_SSL_CERTIFICATE_ERROR;
        goto exit;
    }

    /* Generate certificate */

    if ((ret = mbedtls_mpi_read_string(&serial, 10, "1")) != 0)
    {
        mbedtls_printf(" failed in  mbedtls_mpi_read_string, returned -0x%04x\n", -ret);
        returnCode = COM_ERR_SSL_CERTIFICATE_ERROR;
        goto exit;
    }

    /* for self signed certificate, no need to parse a CA certificate */
    issuer_key = &key;
    subject_key = issuer_key;

    mbedtls_x509write_crt_set_subject_key(&crt, subject_key);
    mbedtls_x509write_crt_set_issuer_key(&crt, issuer_key);

    /* Check the names for validity */
    ret = mbedtls_x509write_crt_set_subject_name(& crt, GENERATED_CERT_SUBJECT);
    if (0 != ret)
    {
        mbedtls_printf(" failed in  mbedtls_x509write_crt_set_subject_name, returned -0x%04x\n", -ret);
        returnCode = COM_ERR_SSL_CERTIFICATE_ERROR;
        goto exit;
    }

    ret = mbedtls_x509write_crt_set_issuer_name(& crt, GENERATED_CERT_SUBJECT); /* issuer is subject for self-signed*/
    if (0 != ret)
    {
        mbedtls_printf(" failed in  mbedtls_x509write_crt_set_issuer_name, returned -0x%04x\n", -ret);
        returnCode = COM_ERR_SSL_CERTIFICATE_ERROR;
        goto exit;
    }

    mbedtls_x509write_crt_set_version(& crt, 2);
    mbedtls_x509write_crt_set_md_alg(& crt, MBEDTLS_MD_SHA256);

    ret = mbedtls_x509write_crt_set_serial(& crt, & serial);
    if (ret != 0)
    {
        mbedtls_printf(" failed in  mbedtls_x509write_crt_set_serial, returned -0x%04x\n", -ret);
        returnCode = COM_ERR_SSL_CERTIFICATE_ERROR;
        goto exit;
    }

    ret = mbedtls_x509write_crt_set_validity(& crt, "20200101120000", "20300101120000"); /* 10 years */
    if (ret != 0)
    {
        mbedtls_printf(" failed in  mbedtls_x509write_crt_set_validity, returned -0x%04x\n", -ret);
        returnCode = COM_ERR_SSL_CERTIFICATE_ERROR;
        goto exit;
    }

    mbedtls_printf("  . Adding the Basic Constraints extension ...");
    fflush(stdout);

    ret = mbedtls_x509write_crt_set_basic_constraints(&crt, 0, -1);
    if (ret != 0)
    {
        mbedtls_printf(" failed in  mbedtls_x509write_crt_set_basic_constraints, returned -0x%04x\n", -ret);
        returnCode = COM_ERR_SSL_CERTIFICATE_ERROR;
        goto exit;
    }

    /* Key usage */
    ret = mbedtls_x509write_crt_set_key_usage(& crt, MBEDTLS_X509_KU_DIGITAL_SIGNATURE |
                                              MBEDTLS_X509_KU_DATA_ENCIPHERMENT);
    if (ret != 0)
    {
        mbedtls_printf(" failed in  mbedtls_x509write_crt_set_key_usage, returned -0x%04x\n", -ret);
        returnCode = COM_ERR_SSL_CERTIFICATE_ERROR;
        goto exit;
    }

    /* Cert type, or key usage extensions */
    ret = mbedtls_x509write_crt_set_ns_cert_type(&crt, MBEDTLS_X509_NS_CERT_TYPE_SSL_CLIENT |
                                                 MBEDTLS_X509_NS_CERT_TYPE_SSL_SERVER |
                                                 MBEDTLS_X509_NS_CERT_TYPE_OBJECT_SIGNING);
    if (ret != 0)
    {
        mbedtls_printf(" failed in  mbedtls_x509write_crt_set_ns_cert_type, returned -0x%04x\n", -ret);
        returnCode = COM_ERR_SSL_CERTIFICATE_ERROR;
        goto exit;
    }

    ret = cryptoal_write_certificate(& crt, cryptoCertificatePath, mbedtls_ctr_drbg_random, &ctr_drbg);
    if (0 != ret)
    {
        mbedtls_printf(" failed in  cryptoal_write_certificate, returned -0x%04x\n", -ret);
        returnCode = COM_ERR_SSL_CERTIFICATE_ERROR;
        goto exit;
    }

    returnCode = COM_SUCCESS;

exit:
    if (returnCode != COM_SUCCESS)
    {
#ifdef MBEDTLS_ERROR_C
        mbedtls_strerror(ret, buf, sizeof ( buf));
        mbedtls_printf(" - %s\n", buf);
#else
        mbedtls_printf("Ko in cryptoal_generate_autosigned_local_certificate_and_keys\n");
#endif
    }

    if (cryptoCertificatePath != NULL)
    {
        NAL_FREE(cryptoCertificatePath);
        cryptoCertificatePath = NULL;
    }
    if (cryptoKeyPath != NULL)
    {
        NAL_FREE(cryptoKeyPath);
        cryptoKeyPath = NULL;
    }

    mbedtls_mpi_free(&N);
    mbedtls_mpi_free(&P);
    mbedtls_mpi_free(&Q);
    mbedtls_mpi_free(&D);
    mbedtls_mpi_free(&E);
    mbedtls_mpi_free(&DP);
    mbedtls_mpi_free(&DQ);
    mbedtls_mpi_free(&QP);

    mbedtls_pk_free(&key);
    mbedtls_ctr_drbg_free(&ctr_drbg);
    mbedtls_entropy_free(&entropy);

    END_FUNCTION();
    return returnCode;
}



/* Check if first certificate has well signed the second one. Algo is RSA2048 with SHA256 for both certificates, as
 * currently used in the cyber solution. Complete absolute or relative path ended with name file shall be given for both.
 * Third parameter is a boolean returned, true if first certficate are really signed the second one..
 * 
 * return COM_SUCCESS if execution is Ok,  otherwise nothing is done nor returned.
*/
T_com_status cryptoal_check_signature_2cert (const char * ca_signing_cert,  const char * signed_cert, bool * pValid)
{
    T_com_status returnCode = COM_SUCCESS;
    int ret = 0;
    char buf[64];
    mbedtls_x509_crt  cacert;
    mbedtls_x509_crt  cert;
    uint32_t flags = 0u;

    START_FUNCTION();

     mbedtls_x509_crt_init( &cert );
     mbedtls_x509_crt_init( &cacert );

     VERIF_ARGS("ca_signing_cert pointer shall not be Null", ca_signing_cert != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("string in ca_signing_cert shall not be Null", ca_signing_cert[0] != 0, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("signed_cert pointer shall not be Null", signed_cert != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("string in signed_cert shall not be Null", signed_cert[0] != 0, returnCode, COM_ERR_PARAM);
     VERIF_ARGS("pValid pointer  shall be Null", pValid != NULL, returnCode, COM_ERR_PARAM);
    if (returnCode == COM_ERR_PARAM)
    {
        goto exit;
    }

     ret = mbedtls_x509_crt_parse_file( & cacert, ca_signing_cert );
    if( ret < 0 )
    {
        mbedtls_printf( " failed in  cryptoal_check_signature_2cert for CA certificate,  mbedtls_x509_crt_parse_file returned -0x%x\n", -ret );
        returnCode = COM_ERR_INTERNAL;
        goto exit;
    }
    ret = mbedtls_x509_crt_parse_file( & cert, signed_cert );
    if( ret < 0 )
    {
        mbedtls_printf( " failed in  cryptoal_check_signature_2cert for signed certificate,  mbedtls_x509_crt_parse_file returned -0x%x\n", -ret );
        returnCode = COM_ERR_INTERNAL;
        goto exit;
    }
    
    ret = mbedtls_x509_crt_verify( & cert, & cacert, NULL /*&cacrl*/ , NULL, & flags,  NULL /*my_verify*/, NULL );
    if (0 == ret)
    {
        TRACE_GEN(TRACE, "cryptoal_check_signature_2cert: signature is good and isued by CA file", __FUNCTION__, __LINE__);
        *pValid = true;
    }
    else
    {
        TRACE_GEN(WARN, "cryptoal_check_signature_2cert: signature is NOT good", __FUNCTION__, __LINE__);
        *pValid = false;
    }

exit:
    mbedtls_x509_crt_free( & cert );
    mbedtls_x509_crt_free( & cacert );

    if (returnCode != COM_SUCCESS)
    {
#ifdef MBEDTLS_ERROR_C
        mbedtls_strerror(ret, buf, sizeof ( buf));
        mbedtls_printf(" - %s\n", buf);
#else
        mbedtls_printf("Ko in cryptoal_check_signature_2cert\n");
#endif
    }
    END_FUNCTION();
    return returnCode;
}


/* Check chain of confidence of all  certificates, assuming here they all exist.
 * See detail in documention on CA-chain.
 * It is assumed target directories also exist.
 *
 * If  one or more signature is not correct, false is returned in out parameter.
 * 
 * return COM_SUCCESS if execution is Ok no matter of CA status,  otherwise nothing is done.
*/
T_com_status cryptoal_check_ca_chain_certificates (bool * ca_chain_correct)
{
    T_com_status returnCode = COM_SUCCESS;
    const char * rootPath = entmgt_launcher_getAppRootPath();
    char * outFilePath = NULL;
    char * outFilePathChildren = NULL;
    T_tools_boolean fileExists = TOOLS_FALSE;
    bool signatureValid = false;

/*
 * Scheme of CA chain :
 * 
 *    ROOT  signs  CLOUD
 *    ROOT  signs  TOOL
 *    TOOL  signs  CYLBOX
 * 
 *    ROOT       CN = CA_ROOT
 *    CLOUD    CN = CA_CLOUD
 *    TOOL        CN = CA_TOOL
 *    CYLBOX  CN = CYLBOX
 * 
 * CA Chain is considerered valid if:
 *      all certificates exist, and local keypair exists
 *      chain of signatures is correct
 *      time validity is good for all certificates
 *      CN of issuer and subject is good in all certificates
 */

    START_FUNCTION();
    VERIF_ARGS (" ca_chain_correct pointer shall not be Null", ca_chain_correct != NULL, returnCode, COM_ERR_PARAM);
    if (returnCode == COM_ERR_PARAM)
    {
        goto exit;
    }

    /* Build the outFilePath for each certificates or keys files */
    * ca_chain_correct = false;

    /* Local cert and keypair */
    if (tools_buildFilePath(rootPath, ENTMGT_DIR_CERTIFICATE_LOCAL, ENTMGT_NAME_CERTIFICATE_LOCAL, &outFilePath) != TOOLS_SUCCESS)
    {
        TRACE_GEN(ERROR, "Building local certificate file path failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_NO_MEMORY;
        goto exit;     
    }
    fileExists = tools_isFileExist (outFilePath);
    NAL_FREE(outFilePath);
    if (TOOLS_FALSE == fileExists)
    {
        TRACE_GEN(WARN, "Local certificate is missing", __FUNCTION__, __LINE__);
        goto exit;
    }

    if (tools_buildFilePath(rootPath, ENTMGT_DIR_CERTIFICATE_LOCAL, ENTMGT_NAME_KEY_LOCAL, &outFilePath) != TOOLS_SUCCESS)
    {
        TRACE_GEN(ERROR, "Building local certificate file path failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_NO_MEMORY;
        goto exit;     
    }
    fileExists = tools_isFileExist (outFilePath);
    NAL_FREE(outFilePath);
    if (TOOLS_FALSE == fileExists)
    {
        TRACE_GEN(WARN, "Local keypair file is missing", __FUNCTION__, __LINE__);
        goto exit;
    }

    /* TOOL  cert */
    if (tools_buildFilePath(rootPath, ENTMGT_DIR_CERTIFICATE_TOOL, ENTMGT_NAME_CERTIFICATE_TOOL, &outFilePath) != TOOLS_SUCCESS)
    {
        TRACE_GEN(ERROR, "Building local certificate file path failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_NO_MEMORY;
        goto exit;     
    }
    fileExists = tools_isFileExist (outFilePath);
    NAL_FREE(outFilePath);
    if (TOOLS_FALSE == fileExists)
    {
        TRACE_GEN(WARN, "Tool certificate is missing", __FUNCTION__, __LINE__);
        goto exit;
    }

    /* CLOUD  cert */
    if (tools_buildFilePath(rootPath, ENTMGT_DIR_CERTIFICATE_CLOUD, ENTMGT_NAME_CERTIFICATE_CLOUD, &outFilePath) != TOOLS_SUCCESS)
    {
        TRACE_GEN(ERROR, "Building local certificate file path failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_NO_MEMORY;
        goto exit;     
    }
    fileExists = tools_isFileExist (outFilePath);
    NAL_FREE(outFilePath);
    if (TOOLS_FALSE == fileExists)
    {
        TRACE_GEN(WARN, "Cloud certificate is missing", __FUNCTION__, __LINE__);
        goto exit;
    }

    /* CA-ROOT  cert */
    if (tools_buildFilePath(rootPath, ENTMGT_DIR_CERTIFICATE_CAROOT, ENTMGT_NAME_CERTIFICATE_CAROOT, &outFilePath) != TOOLS_SUCCESS)
    {
        TRACE_GEN(ERROR, "Building local certificate file path failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_NO_MEMORY;
        goto exit;     
    }
    fileExists = tools_isFileExist (outFilePath);
    NAL_FREE(outFilePath);
    if (TOOLS_FALSE == fileExists)
    {
        TRACE_GEN(WARN, "Root certificate is missing", __FUNCTION__, __LINE__);
        goto exit;
    }

    /* Here all certificates and keys are present */

    /*  ROOT signs TOOL, verifying it here: */
    if (tools_buildFilePath(rootPath, ENTMGT_DIR_CERTIFICATE_CAROOT, ENTMGT_NAME_CERTIFICATE_CAROOT, &outFilePath) != TOOLS_SUCCESS)
    {
        TRACE_GEN(ERROR, "Building local certificate file path failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_NO_MEMORY;
        goto exit;     
    }
    if (tools_buildFilePath(rootPath, ENTMGT_DIR_CERTIFICATE_TOOL, ENTMGT_NAME_CERTIFICATE_TOOL, &outFilePathChildren) != TOOLS_SUCCESS)
    {
        TRACE_GEN(ERROR, "Building local certificate file path failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_NO_MEMORY;
        goto exit;     
    }
    returnCode = cryptoal_check_signature_2cert ( outFilePath, outFilePathChildren, & signatureValid);
    if (COM_SUCCESS != returnCode)
    {
        TRACE_GEN(ERROR, "Testing CA Chain:  cryptoal_check_signature_2cert file path failed", __FUNCTION__, __LINE__);
        goto exit;     
    }
    if (false == signatureValid)
    {
        TRACE_GEN(WARN, "Root signing Tool certificate: Ko", __FUNCTION__, __LINE__);
        goto exit;
    }
    NAL_FREE(outFilePath);
    NAL_FREE(outFilePathChildren);
    
    
    /*  ROOT signs CLOUD, verifying it here: */
    if (tools_buildFilePath(rootPath, ENTMGT_DIR_CERTIFICATE_CAROOT, ENTMGT_NAME_CERTIFICATE_CAROOT, &outFilePath) != TOOLS_SUCCESS)
    {
        TRACE_GEN(ERROR, "Building local certificate file path failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_NO_MEMORY;
        goto exit;
    }
    if (tools_buildFilePath(rootPath, ENTMGT_DIR_CERTIFICATE_CLOUD, ENTMGT_NAME_CERTIFICATE_CLOUD, &outFilePathChildren) != TOOLS_SUCCESS)
    {
        TRACE_GEN(ERROR, "Building local certificate file path failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_NO_MEMORY;
        goto exit;     
    }
    returnCode = cryptoal_check_signature_2cert ( outFilePath, outFilePathChildren, & signatureValid);
    if (COM_SUCCESS != returnCode)
    {
        TRACE_GEN(ERROR, "Testing CA Chain:  cryptoal_check_signature_2cert file path failed", __FUNCTION__, __LINE__);
        goto exit;     
    }
    if (false == signatureValid)
    {
        TRACE_GEN(WARN, "Root signing Cloud certificate: Ko", __FUNCTION__, __LINE__);
        goto exit;
    }
    NAL_FREE(outFilePath);
    NAL_FREE(outFilePathChildren);

    /*  TOOL signs CYLBOX, verifying it here: */
    if (tools_buildFilePath(rootPath, ENTMGT_DIR_CERTIFICATE_TOOL, ENTMGT_NAME_CERTIFICATE_TOOL, &outFilePath) != TOOLS_SUCCESS)
    {
        TRACE_GEN(ERROR, "Building local certificate file path failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_NO_MEMORY;
        goto exit;     
    }
    if (tools_buildFilePath(rootPath, ENTMGT_DIR_CERTIFICATE_LOCAL, ENTMGT_NAME_CERTIFICATE_LOCAL, &outFilePathChildren) != TOOLS_SUCCESS)
    {
        TRACE_GEN(ERROR, "Building local certificate file path failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_NO_MEMORY;
        goto exit;
    }
    returnCode = cryptoal_check_signature_2cert ( outFilePath, outFilePathChildren, & signatureValid);
    if (COM_SUCCESS != returnCode)
    {
        TRACE_GEN(ERROR, "Testing CA Chain:  cryptoal_check_signature_2cert file path failed", __FUNCTION__, __LINE__);
        goto exit;     
    }
    if (false == signatureValid)
    {
        TRACE_GEN(WARN, "Tool signing Local certificate: Ko", __FUNCTION__, __LINE__);
        goto exit;
    }
    NAL_FREE(outFilePath);
    NAL_FREE(outFilePathChildren);

    /* Here all signatures are good in CA Chain, and implictly CN et dates */
    * ca_chain_correct = true;

exit:
    END_FUNCTION();
    return returnCode;
}



/*This function maybe be suppressed in operational mode. Used for certificate TU
 * 
 *  Put certificate in a malloced buffer and in a file cert.info (in /tmp). Buffer shall be freed bycaller.
 * return COM_SUCCESS if execution is Ok,  otherwise nothing is done.
*/
T_com_status cryptoal_put_certificate_in_readable_buffer (const char * fileName, unsigned char ** ppBuffer, size_t *pLen)
{
    T_com_status returnCode = COM_SUCCESS;
    int ret = 0;
    mbedtls_x509_crt crt;
    mbedtls_x509_crt_init( &crt );
    mbedtls_pk_context * pk = NULL;
    mbedtls_mpi N, P, Q, D, E, DP, DQ, QP;
    unsigned char buffer[1500];
    size_t data_size = 0u;

    START_FUNCTION();
    VERIF_ARGS (" fileName not NULL", fileName != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS (" fileName[0] not NULL", fileName[0] != 0, returnCode, COM_ERR_PARAM);
    VERIF_ARGS (" ppBuffer not NULL", ppBuffer != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS (" * ppBuffer NULL", *ppBuffer == NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS (" pLen  not NULL", pLen != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS (" *pLen  NULL", *pLen == 0u, returnCode, COM_ERR_PARAM);
    if (returnCode == COM_ERR_PARAM)
    {
        goto exit;
    }

    /* parse then print buffer, malloc buffer and copy certificate in it */
    
    ret = mbedtls_x509_crt_parse_file( &crt, fileName );
    if (0 != ret)
    {
        mbedtls_printf( " failed in   cryptoal_put_certificate_in_readable_buffer,  mbedtls_x509_crt_parse_file returned %d\n\n", ret );
        mbedtls_x509_crt_free( &crt );
        returnCode = COM_ERR_INTERNAL;
        goto exit;
    }
    
    /* Get public key in certificate */
    pk = & crt.pk;
    mbedtls_rsa_context *rsa = mbedtls_pk_rsa( * pk );

    mbedtls_mpi_init( &N );
    mbedtls_mpi_init( &E );
    ret = mbedtls_rsa_export    ( rsa, &N, NULL, NULL, NULL, &E );
    if (0 != ret)
    {
        mbedtls_printf( " failed in mbedtls_rsa_export for params N & E of public key, could not export RSA parameters\n\n" );
        returnCode = COM_ERR_INTERNAL;
        goto exit;
    }

    /* Get certificate content */
    data_size = mbedtls_x509_crt_info( (char *) buffer, sizeof( buffer ) - 1, "  ",  & crt );
    if( data_size <= 0u )     
    {
        mbedtls_printf( " failed in  cryptoal_put_certificate_in_readable_buffer,   mbedtls_x509_crt_info returned %d\n", ret );
        mbedtls_x509_crt_free( &crt );
        returnCode = COM_ERR_INTERNAL;
        goto exit;
    }

    /* Put info in a result file */
    FILE * f=fopen ("/tmp/cert.info", "w");

    fwrite( "\nCertificate public key:\n", 1, 25, f );
    ret = mbedtls_mpi_write_file( "N: ", &N, 16, f );
    ret = mbedtls_mpi_write_file( "E: ", &E, 16, f );
    fwrite( "Certificate content:\n", 1, 21, f );
    fwrite( buffer, 1, data_size, f );

    /*  malloc & Fill the output buffer*/
    memset (buffer, 0, sizeof(buffer) );
    fclose (f);
    f = fopen("/tmp/cert.info", "r");
    //fseek (f, 0L, SEEK_SET);
    
    *pLen = fread(buffer, 1, sizeof(buffer), f);
    * ppBuffer = NAL_MALLOC( * pLen );
    memset (*ppBuffer, 0, * pLen);
    memcpy (* ppBuffer, buffer, *pLen);
    
    fclose (f);

exit:
    END_FUNCTION();
    return returnCode;
}

/*This function maybe be suppressed in operational mode. Used for certificate TU
 * 
 *  Put keypair in a malloced buffer and in a file keypair.info (in /tmp). Buffer shall be freed bycaller.
 * return COM_SUCCESS if execution is Ok,  otherwise nothing is done.
*/
T_com_status cryptoal_put_keypair_in_readable_buffer (const char * fileName, unsigned char ** ppBuffer, size_t *pLen)
{
    T_com_status returnCode = COM_SUCCESS;
    int ret = 0;
    mbedtls_pk_context  pk;
    mbedtls_mpi N, P, Q, D, E, DP, DQ, QP;
    mbedtls_rsa_context *rsa = NULL;
    unsigned char buffer[2500];
    size_t data_size = 0u;

    START_FUNCTION();
    VERIF_ARGS (" fileName not NULL", fileName != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS (" fileName[0] not NULL", fileName[0] != 0, returnCode, COM_ERR_PARAM);
    VERIF_ARGS (" ppBuffer not NULL", ppBuffer != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS (" * ppBuffer NULL", *ppBuffer == NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS (" pLen  not NULL", pLen != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS (" *pLen  NULL", *pLen == 0u, returnCode, COM_ERR_PARAM);
    if (returnCode == COM_ERR_PARAM)
    {
        goto exit;
    }

    /* parse then print buffer, malloc buffer and copy certificate in it */
    
    mbedtls_pk_init( & pk );
    ret = mbedtls_pk_parse_keyfile( &pk, fileName, "" );
    if (0 != ret)
    {
        mbedtls_printf( " failed in   cryptoal_put_keypai_in_readable_buffer,  mbedtls_pk_parse_keyfile returned %d\n\n", ret );
        returnCode = COM_ERR_INTERNAL;
        goto exit;
    }
     rsa = mbedtls_pk_rsa( pk );

    mbedtls_mpi_init( &N ); mbedtls_mpi_init( &P ); mbedtls_mpi_init( &Q );
    mbedtls_mpi_init( &D ); mbedtls_mpi_init( &E ); mbedtls_mpi_init( &DP );
    mbedtls_mpi_init( &DQ ); mbedtls_mpi_init( &QP );

    ret = mbedtls_rsa_export    ( rsa, &N, &P, &Q, &D, &E );
    if (0 != ret)
    {
        mbedtls_printf( " failed doing mbedtls_rsa_export for params N & P & Q D & E of  key, could not export RSA parameters\n\n" );
        returnCode = COM_ERR_INTERNAL;
        goto exit;
    }
    ret = mbedtls_rsa_export_crt    ( rsa, &DP, &DQ, &QP );
    if (0 != ret)
    {
        mbedtls_printf( " failed doing mbedtls_rsa_export for params DP & DQ & QP of  key, could not export RSA parameters\n\n" );
        returnCode = COM_ERR_INTERNAL;
        goto exit;
    }

    /* Put info in a result file */
    FILE * f=fopen ("/tmp/keypair.info", "w");

    fwrite( "\nkey pair content:\n", 1, 19, f );
    ret = mbedtls_mpi_write_file( "N: ", &N, 16, f );
    ret = mbedtls_mpi_write_file( "E: ", &E, 16, f );
    ret = mbedtls_mpi_write_file( "D: ", &D, 16, f );
    ret = mbedtls_mpi_write_file( "P: ", &P, 16, f );
    ret = mbedtls_mpi_write_file( "Q: ", &Q, 16, f );
    ret = mbedtls_mpi_write_file( "DP: ", &DP, 16, f );
    ret = mbedtls_mpi_write_file( "DQ: ", &DQ, 16, f );
    ret = mbedtls_mpi_write_file( "QP: ", &QP, 16, f );

    /*  malloc & Fill the output buffer*/
    memset (buffer, 0, sizeof(buffer) );
    fclose (f);
    f = fopen("/tmp/keypair.info", "r");
    //fseek (f, 0L, SEEK_SET);
    
    *pLen = fread(buffer, 1, sizeof(buffer), f);
    * ppBuffer = NAL_MALLOC( * pLen );
    memset (*ppBuffer, 0, * pLen);
    memcpy (* ppBuffer, buffer, *pLen);
    
    fclose (f);

exit:
    END_FUNCTION();
    return returnCode;
}



#endif  // CRYPTO_MBEDTLS


