#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "com_error.h"
#include "com_entity.h"
#include "tools_trace.h"
#include "tools_fileManip.h"
#include "nal_mem.h"
#include "com_entity.h"

#include "com_cryptolib_header.h"
#include "cryptoal_hash.h"
#include "cryptoal_tls.h"

#ifdef  CRYPTO_MBEDTLS

/************************  Static functions  *****************************/
#define DEBUG_LEVEL 0

static void cryptoal_my_debug(void *ctx, int level, const char *file, int line, const char *str)
{
    ((void) level);
    mbedtls_fprintf((FILE *) ctx, "  %s:%05d: %s", file, line, str);
    fflush((FILE *) ctx);
}


//========================  GLoball functions  =========================//


/* Generate and wait for ssl connection as a server, on given IP/port, on TCP transport.
 * Third param is boolean "two_ways_authentication", true if client's certificate is required at handshake for both auth.
 * A ssl context malloced inside is given back, as referece for futur usage, and shall be freed by caller with close service.
 * Calling this function is blocking the caller, until a safeconnection is done.
 * This uses the local certificate and key, as this is dedicated for a local "server".
 * 
 *   TODO: time Out to exit or NULL.
 * 
 * return COM_SUCCESS if execution is Ok and a client is connected, otherwise nothing is done.
* Return COM_ERR_SSL_CNX_CLOSED if connection is closed, in this case you ctx has to be freed
*/
T_com_status cryptoal_create_connect_tls_server_ctx ( char * IP,  char * port, bool two_ways_authentication,
                                                     Ssl_context ** ppCtx  )
{
    T_com_status returnCode = COM_SUCCESS;
    int ret = 0;
    mbedtls_entropy_context * pEntropy = NAL_MALLOC (sizeof(mbedtls_entropy_context));
    mbedtls_ctr_drbg_context * p_ctr_drbg = NAL_MALLOC (sizeof(mbedtls_ctr_drbg_context));
    mbedtls_ssl_context * pSsl = NAL_MALLOC (sizeof (mbedtls_ssl_context));
    mbedtls_ssl_config * pConf = NAL_MALLOC (sizeof(mbedtls_ssl_config));
    mbedtls_x509_crt * pSrvcert = NAL_MALLOC (sizeof(mbedtls_x509_crt));
    mbedtls_x509_crt * pCacert = NAL_MALLOC (sizeof(mbedtls_x509_crt));
    mbedtls_pk_context * pPkey = NAL_MALLOC (sizeof(mbedtls_pk_context));
    size_t nb_handshake_retries = 0u;
    mbedtls_net_context listen_fd, * pClient_fd = NAL_MALLOC (sizeof(mbedtls_net_context));
    const unsigned char * pers = "ssl_server";
    const char * rootPath = NULL;
    char * outFilePath = NULL;

    START_FUNCTION();

    if ( (NULL == pEntropy) || (NULL == p_ctr_drbg) || (NULL == pSsl) || (NULL == pConf) || (NULL == pSrvcert) ||
          (NULL == pCacert) || (NULL == pPkey) || (NULL == pClient_fd) )
    {
        TRACE_GEN(ERROR, "Error mallocing for anything in server context ", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_NO_MEMORY;
        goto exit;     
    }

    VERIF_ARGS("IP pointer shall not be Null", IP != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("port pointer shall not be Null", port != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("ppCtx pointer shall be not null", ppCtx != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS(" * ppCtx pointer shall be null", * ppCtx == NULL, returnCode, COM_ERR_PARAM);
    if (returnCode == COM_ERR_PARAM)
    {
        goto exit;
    }

#if defined(MBEDTLS_SSL_CACHE_C)
    mbedtls_ssl_cache_context * pCache = NAL_MALLOC (sizeof(mbedtls_ssl_cache_context));
    mbedtls_ssl_cache_init( pCache );
#endif
    mbedtls_net_init(&listen_fd);
    mbedtls_net_init( pClient_fd );
    mbedtls_ssl_init( pSsl );
    mbedtls_ssl_config_init( pConf );

    mbedtls_x509_crt_init( pSrvcert );
    mbedtls_x509_crt_init( pCacert );
    mbedtls_pk_init( pPkey);
    mbedtls_entropy_init( pEntropy);
    mbedtls_ctr_drbg_init( p_ctr_drbg);

#if defined(MBEDTLS_DEBUG_C)
    mbedtls_debug_set_threshold(DEBUG_LEVEL);
#endif

    rootPath = entmgt_launcher_getAppRootPath();

    /* Build the outFilePath */
    if (tools_buildFilePath(rootPath, ENTMGT_DIR_CERTIFICATE_LOCAL, ENTMGT_NAME_CERTIFICATE_LOCAL, &outFilePath) != TOOLS_SUCCESS)
    {
        TRACE_GEN(ERROR, "Building local certificate file path failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_NO_MEMORY;
        goto exit;     
    }
    ret = mbedtls_x509_crt_parse_file( pSrvcert, outFilePath);
    if (NULL != outFilePath)
    {
        NAL_FREE (outFilePath);
        outFilePath = NULL;
    }
    if( ret != 0 )
    {
        TRACE_GEN(WARN, "Unable to parse local server certificate, mbedtls_x509_crt_parse_file failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_SSL_CERTIFICATE_ERROR;
        goto exit;
    }

    /* Build the outFilePath */
    if (tools_buildFilePath(rootPath, ENTMGT_DIR_CERTIFICATE_LOCAL, ENTMGT_NAME_KEY_LOCAL, &outFilePath) != TOOLS_SUCCESS)
    {
        TRACE_GEN(ERROR, "Building local keypaire file path failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_NO_MEMORY;
        goto exit;     
    }
    ret = mbedtls_pk_parse_keyfile( pPkey, outFilePath, "" );
    if (NULL != outFilePath)
    {
        NAL_FREE (outFilePath);
        outFilePath = NULL;
    }
    if( ret != 0 )
    {
        TRACE_GEN (WARN, "Unable to parse local keypaire file, mbedtls_x509_crt_parse_file failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_SSL_CERTIFICATE_ERROR;
        goto exit;
    }

    /* Build the outFilePath */
    if (tools_buildFilePath(rootPath, ENTMGT_DIR_CERTIFICATE_TOOL, ENTMGT_NAME_CERTIFICATE_TOOL, &outFilePath) != TOOLS_SUCCESS)
    {
        TRACE_GEN(ERROR, "Building tool certificate file path failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_NO_MEMORY;
        goto exit;     
    }
    ret = mbedtls_x509_crt_parse_file( pCacert, outFilePath);
    if (NULL != outFilePath)
    {
        NAL_FREE (outFilePath);
        outFilePath = NULL;
    }
    if( ret != 0 )
    {
        TRACE_GEN(WARN, "Unable to parse CA certificate, mbedtls_x509_crt_parse_file failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_SSL_CERTIFICATE_ERROR;
        goto exit;
    }

    ret = mbedtls_net_bind(&listen_fd, IP, port, MBEDTLS_NET_PROTO_TCP);
    if (ret != 0)
    {
        TRACE_GEN(WARN, "Unable to bind ssl socket on IP/port, mbedtls_net_bind failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_SSL_BIND_ERROR;
        goto exit;
    }

    /* Seeding the entropy RNG */
    ret = mbedtls_ctr_drbg_seed( p_ctr_drbg, mbedtls_entropy_func, pEntropy, pers, strlen(pers));
    if (ret != 0)
    {
        TRACE_GEN(WARN, "Unable to seed entropy RNG, mbedtls_ctr_drbg_seed failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_SSL_CRYPTO_ERROR;
        goto exit;
    }

    ret = mbedtls_ssl_config_defaults( pConf, MBEDTLS_SSL_IS_SERVER, MBEDTLS_SSL_TRANSPORT_STREAM,
                                      MBEDTLS_SSL_PRESET_DEFAULT);
    if (ret != 0)
    {
        TRACE_GEN(WARN, " mbedtls_ssl_config_defaults failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_SSL_CRYPTO_ERROR;
        goto exit;
    }

    if (true == two_ways_authentication)
        mbedtls_ssl_conf_authmode( pConf, MBEDTLS_SSL_VERIFY_REQUIRED);

    /* Set entropy */
    mbedtls_ssl_conf_rng( pConf, mbedtls_ctr_drbg_random, p_ctr_drbg);
    mbedtls_ssl_conf_dbg( pConf, cryptoal_my_debug, stdout);

#if defined(MBEDTLS_SSL_CACHE_C)
    mbedtls_ssl_conf_session_cache( pConf, pCache, mbedtls_ssl_cache_get, mbedtls_ssl_cache_set);
#endif
    mbedtls_ssl_conf_ca_chain( pConf, pCacert, NULL);

    ret = mbedtls_ssl_conf_own_cert( pConf, pSrvcert, pPkey);
    if (ret != 0)
    {
        TRACE_GEN(WARN, " mbedtls_ssl_conf_own_cert failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_SSL_CERTIFICATE_ERROR;
        goto exit;
    }

    ret = mbedtls_ssl_setup( pSsl, pConf);
    if (ret != 0)
    {
        TRACE_GEN(WARN, " mbedtls_ssl_setup failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_SSL_CRYPTO_ERROR;
        goto exit;
    }

reset:
#ifdef MBEDTLS_ERROR_C
    if (ret != 0)
    {
        char error_buf[512];
        mbedtls_strerror(ret, error_buf, 512-1);
        TRACE_GEN(WARN, error_buf, __FUNCTION__, __LINE__);
    }
#endif

    mbedtls_net_free( pClient_fd );
    mbedtls_ssl_session_reset( pSsl );

    /* Wait until a client connects */
    ret = mbedtls_net_accept(&listen_fd, pClient_fd, NULL, 0, NULL);
    if (ret != 0)
    {
        TRACE_GEN(WARN, " mbedtls_net_accept failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_SSL_CRYPTO_ERROR;
        goto exit;
    }

    /* Set read/write callback in ssl context */
    mbedtls_ssl_set_bio( pSsl, pClient_fd, mbedtls_net_send, mbedtls_net_recv, NULL);

    /* Handshake */
    ret = mbedtls_ssl_handshake( pSsl );
    while ( 0 != ret )
    {
      printf ("\nafter ssl_handshake KO, ret = -%x\n", -ret);
        if ((ret != MBEDTLS_ERR_SSL_WANT_READ) && (ret != MBEDTLS_ERR_SSL_WANT_WRITE))
        {
            if (nb_handshake_retries == NB_SSL_HANDSHAKE_RETRIES_MAX)
            {
                returnCode = COM_ERR_SSL_HANDSHAKE_ERROR;
                goto exit;
            }
            nb_handshake_retries++;
            goto reset;
        }
        ret = mbedtls_ssl_handshake( pSsl );
    }
    (*(mbedtls_ssl_context**)ppCtx) = pSsl;
    
exit:
    mbedtls_net_free(&listen_fd);
    
    if (COM_SUCCESS != returnCode)     // if function fails
    {
      if (NULL != pEntropy)
          NAL_FREE (pEntropy);
      if (NULL != p_ctr_drbg)
          NAL_FREE (p_ctr_drbg);
      if (NULL != pSsl)
          NAL_FREE (pSsl);
      if (NULL != pConf)
          NAL_FREE (pConf);
      if (NULL != pSrvcert)
          NAL_FREE (pSrvcert);
      if (NULL != pCacert)
          NAL_FREE (pCacert);
      if (NULL != pPkey)
          NAL_FREE (pPkey);
      if (NULL != pClient_fd)
          NAL_FREE (pClient_fd);
#if defined(MBEDTLS_SSL_CACHE_C)
      if (NULL != pCache)
          NAL_FREE (pCache);
#endif
    }

    END_FUNCTION();
    return returnCode;
}


/* Generate and connect a client ssl connection to a server, targetting an IP/port, on TCP transport.
 * This connexion always verify server certificate received at handshake, and send its own to the server of eventual check.
 * 
 * An ssl context is given back, as referece for futur usage, in last param.
 * Calling this function is never blocking.
 * This uses the local certificate and key, as this is dedicated for a local "client" or "server".
 * 
 * return COM_SUCCESS if execution is Ok and local client is connected, otherwise nothing is done.
* Return COM_ERR_SSL_CNX_CLOSED if connection is closed, in this case your ctx has to be freed
*/
T_com_status cryptoal_create_connect_tls_client_ctx ( char * IP,  char * port,  Ssl_context ** ppCtx )
{
    T_com_status returnCode = COM_SUCCESS;
    int ret = 0;
    mbedtls_entropy_context * pEntropy = NAL_MALLOC (sizeof(mbedtls_entropy_context));
    mbedtls_ctr_drbg_context * p_ctr_drbg = NAL_MALLOC (sizeof(mbedtls_ctr_drbg_context));;

    mbedtls_ssl_context * pSsl = NAL_MALLOC (sizeof (mbedtls_ssl_context));
    mbedtls_ssl_config * pConf = NAL_MALLOC(sizeof(mbedtls_ssl_config));;
    mbedtls_x509_crt * pClicert = NAL_MALLOC (sizeof(mbedtls_x509_crt));
    mbedtls_x509_crt * pCacert = NAL_MALLOC (sizeof(mbedtls_x509_crt));
    mbedtls_pk_context * pPkey = NAL_MALLOC (sizeof(mbedtls_pk_context));
    mbedtls_net_context * pServer_fd = NAL_MALLOC (sizeof(mbedtls_net_context));
    uint32_t flags = 0;
    const unsigned char * pers = "create_connect_tls_client";
    char error_buf[512];
    const char * rootPath = NULL;
    char * outFilePath = NULL;
    
    START_FUNCTION();

    if (NULL == pSsl)
    {
        TRACE_GEN(ERROR, "Error mallocing for a SSL client context ", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_NO_MEMORY;
        goto exit;     
    }

    VERIF_ARGS("IP pointer shall not be Null", IP != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("port pointer shall not be Null", port != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("ppCtx pointer shall be not null", ppCtx != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS(" * ppCtx pointer shall be null", * ppCtx == NULL, returnCode, COM_ERR_PARAM);
    if (returnCode == COM_ERR_PARAM)
    {
        goto exit;
    }
    mbedtls_net_init( pServer_fd );
    mbedtls_ssl_init( pSsl );
    mbedtls_ssl_config_init( pConf );

    mbedtls_x509_crt_init( pClicert );
    mbedtls_x509_crt_init( pCacert );
    mbedtls_pk_init( pPkey );
    mbedtls_ctr_drbg_init( p_ctr_drbg );
    mbedtls_entropy_init( pEntropy );

#if defined(MBEDTLS_DEBUG_C)
    mbedtls_debug_set_threshold(DEBUG_LEVEL);
#endif

    /* Seeding the entropy RNG */
    ret = mbedtls_ctr_drbg_seed( p_ctr_drbg, mbedtls_entropy_func, pEntropy, pers, strlen(pers) );
    if (ret != 0)
    {
        TRACE_GEN(WARN, "Unable to seed entropy RNG, mbedtls_ctr_drbg_seed failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_SSL_CRYPTO_ERROR;
        goto exit;
    }
    mbedtls_ssl_conf_rng( pConf, mbedtls_ctr_drbg_random, p_ctr_drbg);
    mbedtls_ssl_conf_dbg( pConf, cryptoal_my_debug, stdout);

    /* Build the outFilePath */
    rootPath = entmgt_launcher_getAppRootPath();
    if (tools_buildFilePath(rootPath, ENTMGT_DIR_CERTIFICATE_LOCAL, ENTMGT_NAME_CERTIFICATE_LOCAL, &outFilePath) != TOOLS_SUCCESS)
    {
        TRACE_GEN(ERROR, "Building local certificate file path failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_NO_MEMORY;
        goto exit;     
    }
    ret = mbedtls_x509_crt_parse_file( pClicert, outFilePath);
    if (NULL != outFilePath)
    {
        NAL_FREE (outFilePath);
        outFilePath = NULL;
    }
    if( ret != 0 )
    {
        TRACE_GEN (WARN, "Unable to parse local server certificate, mbedtls_x509_crt_parse_file failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_SSL_CERTIFICATE_ERROR;
        goto exit;
    }

    /* Build the outFilePath */
    if (tools_buildFilePath(rootPath, ENTMGT_DIR_CERTIFICATE_LOCAL, ENTMGT_NAME_KEY_LOCAL, &outFilePath) != TOOLS_SUCCESS)
    {
        TRACE_GEN(ERROR, "Building local keypaire file path failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_NO_MEMORY;
        goto exit;     
    }
    ret = mbedtls_pk_parse_keyfile( pPkey, outFilePath, ""  /* GENERATED_KEYFILE_PASSWORD */  );
    if (NULL != outFilePath)
    {
        NAL_FREE (outFilePath);
        outFilePath = NULL;
    }
    if( ret != 0 )
    {
        TRACE_GEN (WARN, "Unable to parse local keypaire file, mbedtls_x509_crt_parse_file failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_SSL_CERTIFICATE_ERROR;
        goto exit;
    }

    /* Build the outFilePath */
    if (tools_buildFilePath(rootPath, ENTMGT_DIR_CERTIFICATE_TOOL, ENTMGT_NAME_CERTIFICATE_TOOL, &outFilePath) != TOOLS_SUCCESS)
    {
        TRACE_GEN(ERROR, "Building tool certificate file path failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_NO_MEMORY;
        goto exit;     
    }
    ret = mbedtls_x509_crt_parse_file( pCacert, outFilePath);
    if (NULL != outFilePath)
    {
        NAL_FREE (outFilePath);
        outFilePath = NULL;
    }
    if( ret != 0 )
    {
        TRACE_GEN (WARN, "Unable to parse CA certificate, mbedtls_x509_crt_parse_file failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_SSL_CERTIFICATE_ERROR;
        goto exit;
    }

    ret = mbedtls_ssl_conf_own_cert( pConf, pClicert, pPkey);
    if (0 != ret)
    {
        TRACE_GEN(WARN, "failedto set own certificate and ca in context, mbedtls_ssl_conf_own_cert failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_SSL_CERTIFICATE_ERROR;
        goto exit;
    }
    mbedtls_ssl_conf_ca_chain( pConf, pCacert, NULL);

   /* Start connexion */
    ret = mbedtls_net_connect( pServer_fd, IP, port, MBEDTLS_NET_PROTO_TCP );
    if( 0 != ret )
    {
        TRACE_GEN(WARN, "Unable to connect to Server, mbedtls_net_connect failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_CONNECT;
        goto exit;
    }

    ret = mbedtls_ssl_config_defaults( pConf, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_PRESET_DEFAULT);
    if (0 != ret)
    {
        TRACE_GEN(WARN, "Unable to ssl context default vues, mbedtls_ssl_config_defaults failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_SSL_CRYPTO_ERROR;
        goto exit;
    }
    mbedtls_ssl_conf_authmode( pConf, MBEDTLS_SSL_VERIFY_REQUIRED);

    ret = mbedtls_ssl_setup( pSsl,  pConf);
    if (0 != ret)
    {
        TRACE_GEN(WARN, "Unable to ssl context with conf, mbedtls_ssl_setup failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_SSL_CRYPTO_ERROR;
        goto exit;
    }

    if ((ret = mbedtls_ssl_set_hostname( pSsl,  CRYPTOAL_CERTIFICATE_TOOLBOX_CN )) != 0)
    {
        TRACE_GEN(WARN, "Unable to ssl context hostname, mbedtls_ssl_set_hostname failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_SSL_CRYPTO_ERROR;
        goto exit;
    }

    mbedtls_ssl_set_bio( pSsl, pServer_fd, mbedtls_net_send, mbedtls_net_recv, NULL);

    /* Manage Handshake */
    while ((ret = mbedtls_ssl_handshake( pSsl )) != 0)
    {
        if ( (ret != MBEDTLS_ERR_SSL_WANT_READ) && (ret != MBEDTLS_ERR_SSL_WANT_WRITE) )
        {
            TRACE_GEN(WARN, "Unable to perform SSL handshake, mbedtls_ssl_handshake failed", __FUNCTION__, __LINE__);
            returnCode = COM_ERR_SSL_HANDSHAKE_ERROR;
            goto exit;
        }
    }

    /* verify server certificate, like a navigator does */
    flags = mbedtls_ssl_get_verify_result( pSsl );
    if (0 != flags)
    {
        TRACE_GEN(WARN, "After handshake server certificate is bad, mbedtls_ssl_get_verify_result failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_SSL_CERTIFICATE_ERROR;
        mbedtls_x509_crt_verify_info(error_buf, sizeof ( error_buf)-1, "  ! ", flags);
        TRACE_GEN(WARN, error_buf, __FUNCTION__, __LINE__);
        goto exit;
    }
    (*(mbedtls_ssl_context**)ppCtx) = pSsl;

exit:

    if (COM_SUCCESS != returnCode)     // if function fails
    {
      if (NULL != pEntropy)
          NAL_FREE (pEntropy);
      if (NULL != p_ctr_drbg)
          NAL_FREE (p_ctr_drbg);
      if (NULL != pSsl)
          NAL_FREE (pSsl);
      if (NULL != pConf)
          NAL_FREE (pConf);
      if (NULL != pClicert)
          NAL_FREE (pClicert);
      if (NULL != pCacert)
          NAL_FREE (pCacert);
      if (NULL != pPkey)
          NAL_FREE (pPkey);
      if (NULL != pServer_fd)
          NAL_FREE (pServer_fd);
    }

    END_FUNCTION();
    return returnCode;
}


/* Read on ssl connection, client OR server no matter, using context as reference.
 * Here a client is already connected, and has performed ssl handshake.
 * 
 * Calling this function may be blocking for a while, until data are read.
 * 
 * TODO: propose a time-out
 * 
 * return COM_SUCCESS if execution is Ok and data are read, otherwise nothing is done.
 * If error COM_ERR_SSL_CNX_CLOSED is returned, that means connection has been closed for
 * any reason so caller shall free resources of connection.
 */
T_com_status cryptoal_read_tls_ctx ( Ssl_context * pCtx, void * pBuf, size_t lenBuf, size_t * pReadLen)
{
    T_com_status returnCode = COM_SUCCESS;
    int ret = 0;
    bool cnx_closed = false;
    bool retry = true;

    START_FUNCTION();

    VERIF_ARGS("pCtx pointer shall be not null", pCtx != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("pBuf pointer shall be not null", pBuf != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("lenBuf shall be not null", lenBuf != 0u, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("pReadLen pointer shall be not null", pReadLen != NULL, returnCode, COM_ERR_PARAM);
    if (returnCode == COM_ERR_PARAM)
    {
        goto exit;
    }
    * pReadLen = 0u;

    while (true == retry)
    {
        ret = mbedtls_ssl_read((mbedtls_ssl_context*)pCtx, pBuf, lenBuf);

        if (ret > 0) /* Read OK */
        {
            * pReadLen = ret;
            retry = false;
        }
        else if ((MBEDTLS_ERR_SSL_WANT_READ == ret) || (MBEDTLS_ERR_SSL_WANT_WRITE == ret)) /* read again */
        {
            /* re-negotiating is in progress.., do nothing here to read again in loop */
        }
        else if ((MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY == ret) ||
                (MBEDTLS_ERR_NET_CONN_RESET == ret) ||
                (/*EOF*/ 0 == ret)) /* cnx has been globally closed */
        {
            returnCode = COM_ERR_SSL_CNX_CLOSED;
            mbedtls_printf(" Connection has been closed or reset or EOF was received\n");
            retry = false;
        }
        else /* Other problem */
        {
            returnCode = COM_ERR_RECV;
            mbedtls_printf(" mbedtls_ssl_read error returned -0x%x\n", (unsigned int) -ret);
            retry = false;
        }

    } /* End while  */

exit:
    END_FUNCTION();
    return returnCode;
}

/* Write on ssl connection, client OR server no matter, using context as reference
 * Calling this function cannot be blocking, data are written immediatly except if failing. Written size is given back.
 * 
 * returns COM_SUCCESS if execution is Ok and some data are written (total or partial), otherwise nothing is done.
 * returns COM_ERR_SSL_CNX_CLOSED if ssl session was peviously closed, in this case caller shall free resources after.
*/
    T_com_status cryptoal_write_tls_ctx ( Ssl_context * pCtx, void * pBuf, size_t lenBuf, size_t * pWrittenLen)
{
    T_com_status returnCode = COM_SUCCESS;
    int ret = 0;
    bool retry = true;

    START_FUNCTION();

    VERIF_ARGS("pCtx pointer shall be not null", pCtx != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("pBuf pointer shall be not null", pBuf != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("lenBuf shall be not null", lenBuf != 0u, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("pWrittenLen pointer shall be not null", pWrittenLen != NULL, returnCode, COM_ERR_PARAM);
    if (returnCode == COM_ERR_PARAM)
    {
        goto exit;
    }
    * pWrittenLen = 0u;

    while (true == retry)
    {
        ret = mbedtls_ssl_write((mbedtls_ssl_context*)pCtx, pBuf, lenBuf);

        if (ret == MBEDTLS_ERR_NET_CONN_RESET)
        {
            returnCode = COM_ERR_SSL_CNX_CLOSED;
            retry = false;
            mbedtls_printf(" failed  ! peer closed the connection\n\n");
        }
            /* ssl connection in currently re-negociating, trying again.. */
        else if ((ret == MBEDTLS_ERR_SSL_WANT_READ) || (ret == MBEDTLS_ERR_SSL_WANT_WRITE))
        {
            mbedtls_printf(" failed  mbedtls_ssl_write returned %d\n\n", ret); //TODO a litle tempo
        }
        else if (ret <= 0)
        {
            returnCode = COM_ERR_SSL_CRYPTO_ERROR;
            retry = false;
            mbedtls_printf("  mbedtls_ssl_write failed, -%d ou -0x%x\n", -ret, -ret);
        }
        else /* ret positive Ok, data written at least partially */
        {
            * pWrittenLen = ret;
            retry = false;
        }
    }

    
    
    
exit:
    END_FUNCTION();
    return returnCode;
}

/* Close tls connexion (if needed), for client or server no matter,  and Free all context. No problem if connection was previously
 * closed for any reason.  Calling this function returns immediatly.
 * 
 * return COM_SUCCESS if execution is Ok, otherwise nothing is done.
*/
T_com_status cryptoal_close_free_tls_ctx ( Ssl_context * pCtx )
{
    T_com_status returnCode = COM_SUCCESS;
    int ret = 0;
    bool retry_close = true;
    void * pToFree = NULL;
    
    START_FUNCTION();

    VERIF_ARGS("pCtx pointer shall be not null", pCtx != NULL, returnCode, COM_ERR_PARAM);
    if (returnCode == COM_ERR_PARAM)
    {
        goto exit;
    }

    while (true == retry_close)
    {
        retry_close = false;
        ret = mbedtls_ssl_close_notify((mbedtls_ssl_context*)pCtx);

        if ((ret == MBEDTLS_ERR_SSL_WANT_READ) || (ret == MBEDTLS_ERR_SSL_WANT_WRITE))
            retry_close = true; /* try to close again only in this case */ 
        else if  (ret > 0)   //(MBEDTLS_ERR_NET_INVALID_CONTEXT == ret)
          TRACE_GEN(TRACE, " Anomaly trying to close ssl server connection, might be already closed", __FUNCTION__, __LINE__);
        
    }
exit:

    mbedtls_entropy_free  (((mbedtls_ctr_drbg_context *) (((mbedtls_ssl_context*)pCtx)->conf->p_rng))->p_entropy);
    NAL_FREE  (((mbedtls_ctr_drbg_context *) (((mbedtls_ssl_context*)pCtx)->conf->p_rng))->p_entropy);
    mbedtls_ctr_drbg_free ( ((mbedtls_ssl_context*)pCtx)->conf->p_rng) ;
    pToFree = (void*) ((mbedtls_ssl_context*)pCtx)->conf->p_rng;
    NAL_FREE    ( pToFree ) ;
    mbedtls_pk_free( ((mbedtls_ssl_key_cert*)(((mbedtls_ssl_context*)pCtx)->conf->key_cert))->key );
    NAL_FREE( ((mbedtls_ssl_key_cert*)(((mbedtls_ssl_context*)pCtx)->conf->key_cert))->key );
    mbedtls_x509_crt_free( ((mbedtls_ssl_key_cert*)(((mbedtls_ssl_context*)pCtx)->conf->key_cert))->cert );
    NAL_FREE( ((mbedtls_ssl_key_cert*)(((mbedtls_ssl_context*)pCtx)->conf->key_cert))->cert );
    mbedtls_x509_crt_free( ((mbedtls_ssl_context*)pCtx)->conf->ca_chain );
    pToFree = ((mbedtls_ssl_context*)pCtx)->conf->ca_chain;
    NAL_FREE( pToFree );
#if defined(MBEDTLS_SSL_CACHE_C)
    if (NULL != (((mbedtls_ssl_context*)pCtx)->conf->p_cache) )
    {
        mbedtls_ssl_cache_free(  ((mbedtls_ssl_context*)pCtx)->conf->p_cache );
        pToFree = ((mbedtls_ssl_context*)pCtx)->conf->p_cache;
        NAL_FREE ( pToFree );
    }
#endif

    mbedtls_net_free( ((mbedtls_ssl_context*)pCtx)->p_bio );
    NAL_FREE( ((mbedtls_ssl_context*)pCtx)->p_bio );

    mbedtls_ssl_config_free((mbedtls_ssl_config *)((mbedtls_ssl_context*)pCtx)->conf);
    mbedtls_ssl_free((mbedtls_ssl_context*)pCtx);
    pToFree = (void *) ((mbedtls_ssl_context*)pCtx)->conf;
    NAL_FREE( pToFree );
    NAL_FREE( pCtx );

    END_FUNCTION();
    return returnCode;
}


#endif  // CRYPTO_MBEDTLS

