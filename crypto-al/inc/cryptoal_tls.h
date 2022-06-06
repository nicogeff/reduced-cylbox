#ifndef CRYPTOAL_TLS_H
#define CRYPTOAL_TLS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "cryptoal_common.h"


/*==================== Definitions ====================*/

#define NB_SSL_HANDSHAKE_RETRIES_MAX 3
    
/*================= Exported functions ================*/


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
/* Header Exported in common header com_cryptolib_header.h :
 * 
 * T_com_status cryptoal_create_connect_tls_server_ctx ( char * IP,  char * port,
 *      bool two_ways_authentication,  Ssl_context ** ppCtx  );
 */


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
/* Header Exported in common header com_cryptolib_header.h :
 * 
 * T_com_status cryptoal_create_connect_tls_client_ctx ( char * IP,  char * port,  Ssl_context ** ppCtx );
 * 
 */


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
/* Header Exported in common header com_cryptolib_header.h :
 * 
 * T_com_status cryptoal_read_tls_ctx ( Ssl_context * pCtx, void * pBuf, size_t lenBuf, size_t * pReadLen);
 * 
 */

/* Write on ssl connection, client OR server no matter, using context as reference
 * Calling this function cannot be blocking, data are written immediatly except if failing. Written size is given back.
 * 
 * returns COM_SUCCESS if execution is Ok and some data are written (total or partial), otherwise nothing is done.
 * returns COM_ERR_SSL_CNX_CLOSED if ssl session was peviously closed, in this case caller shall free resources after.
*/
/* Header Exported in common header com_cryptolib_header.h :
 * 
 * T_com_status cryptoal_write_tls_ctx ( Ssl_context * pCtx, void * pBuf, size_t lenBuf, size_t * pWrittenLen);
 * 
 */

/* Close tls connexion (if needed), for client or server no matter,  and Free all context. No problem if connection was previously
 * closed for any reason.  Calling this function returns immediatly.
 * 
 * return COM_SUCCESS if execution is Ok, otherwise nothing is done.
*/
/* Header Exported in common header com_cryptolib_header.h :
 * 
 * T_com_status cryptoal_close_free_tls_ctx ( Ssl_context * pCtx );
 * 
 */


#ifdef __cplusplus
}
#endif

#endif /* CRYPTOAL_TLS_H */

