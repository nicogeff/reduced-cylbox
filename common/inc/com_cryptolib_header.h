/*
 * \file com_cryptolib_header.h
 * \brief Public API of cryptoal, managing mainly headers of choosen library. Managing to all external API provided. 
 * \specifics 
 * \details
 * 
 */

#ifndef COM_CRYPTOLIB_HEADER_H
#define COM_CRYPTOLIB_HEADER_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------ Definitions -------------------------------*/


#if defined  CRYPTO_OPENSSL

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include "openssl/rand.h"
#include "openssl/sha.h"
#include "openssl/bio.h"
#include "openssl/buffer.h"
#include "openssl/rsa.h"
#include "openssl/pem.h"
#include "openssl/pkcs12.h"

#define  ssl_context  openssl_ssl_context
#define  net_context  openssl_net_context

#elif defined CRYPTO_MBEDTLS

#include "mbedtls/config.h"
#include "mbedtls/error.h"
#include "mbedtls/debug.h"
#include "mbedtls/platform.h"
#include "mbedtls/base64.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/md.h"
#include "mbedtls/pk.h"
#include "mbedtls/ssl.h"
#include "mbedtls/ssl_cache.h"
#include "mbedtls/net_sockets.h"
#include "mbedtls/x509_csr.h"

#include "mbedtls/ssl_internal.h"


#define   Ssl_context  mbedtls_ssl_context
#define  Net_context  mbedtls_net_context

#endif


/*------------------------------ Exported Definitions -------------------------------*/

/* Certificates path and names */
#define ENTMGT_DIR_CERTIFICATE_LOCAL "certs/local/"
#define ENTMGT_DIR_CERTIFICATE_TOOL   "certs/ca_tool/"
#define ENTMGT_DIR_CERTIFICATE_CLOUD   "certs/ca_cloud/"
#define ENTMGT_DIR_CERTIFICATE_CAROOT  "certs/ca_root/"

#define ENTMGT_NAME_CERTIFICATE_LOCAL   "box.cert.pem"
#define ENTMGT_NAME_KEY_LOCAL "box.key.pem"

    /* Password on keypair creation not supported by mbedtls => THIS PASSSWORD IS NOT USED */
#define GENERATED_KEYFILE_PASSWORD   "devsec"

#define ENTMGT_NAME_CERTIFICATE_TOOL  "certificate.pem"
#define ENTMGT_NAME_CERTIFICATE_CLOUD  "certificate.pem"
#define ENTMGT_NAME_CERTIFICATE_CAROOT  "certificate.pem"

/* Certificate information */
#define  CRYPTOAL_CERTIFICATE_TOOLBOX_CN  "cylbox_local"

    
/*------------------------------  Exported Functions for Certificates  -----------------------------------*/

/* Check chain of confidence of all  certificates, assuming here they all exist.
 * See detail in documention on CA-chain.
 * It is assumed target directories also exist.
 *
 * If  one or more signature is not correct, false is returned in out parameter.
 * return COM_SUCCESS if execution is Ok,  otherwise nothing is done.
*/
T_com_status cryptoal_check_ca_chain_certificates (bool * pIs_ca_correct);


/* Suppress all certificate and keypair of the CA chain. We know its structure.
 * To bo called when CA is failing, and then generate an autosigned certificate in degraded mode.
 * returns nothing.
 */
void cryptoal_ca_chain_clean_certificates (void);


/*------------------------------  Exported Functions for TLS  -----------------------------------*/

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
T_com_status cryptoal_create_connect_tls_server_ctx ( char * IP,  char * port,
     bool two_ways_authentication,  Ssl_context ** ppCtx  );



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
T_com_status cryptoal_create_connect_tls_client_ctx ( char * IP,  char * port,  Ssl_context ** ppCtx );



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
T_com_status cryptoal_read_tls_ctx ( Ssl_context * pCtx, void * pBuf, size_t lenBuf, size_t * pReadLen);


/* Write on ssl connection, client OR server no matter, using context as reference
 * Calling this function cannot be blocking, data are written immediatly except if failing. Written size is given back.
 * 
 * returns COM_SUCCESS if execution is Ok and some data are written (total or partial), otherwise nothing is done.
 * returns COM_ERR_SSL_CNX_CLOSED if ssl session was peviously closed, in this case caller shall free resources after.
*/
    T_com_status cryptoal_write_tls_ctx ( Ssl_context * pCtx, void * pBuf, size_t lenBuf, size_t * pWrittenLen);


/* Close tls connexion (if needed), for client or server no matter,  and Free all context. No problem if connection was previously
 * closed for any reason.  Calling this function returns immediatly.
 * 
 * return COM_SUCCESS if execution is Ok, otherwise nothing is done.
*/
T_com_status cryptoal_close_free_tls_ctx ( Ssl_context * pCtx );





#ifdef __cplusplus
}
#endif

#endif /* COM_CRYPTOLIB_HEADER_H */

