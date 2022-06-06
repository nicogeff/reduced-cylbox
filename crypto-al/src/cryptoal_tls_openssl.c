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
#include "cryptoal_tls.h"

#ifdef  CRYPTO_OPENSSL


/*========================  Local functions  =========================*/



/*========================  GLobal functions  =========================*/


/* Generate and wait for ssl connection as a server, on given IP/port, on TCP transport.
 * Third param is boolean "two_ways_authentication", true if client's certificate is required at handshake for both auth.
 * A ssl context is given back, as referece for futur usage,
 *     and a client context is given back to caller so this latter can free it when closing.
 * Calling this function is blocking the caller, until connection is Ok.
 * This uses the local certificate and key, as this is dedicated for a local "server".
 * 
 *   TODO: time Out to exit or NULL.
 * 
 * return COM_SUCCESS if execution is Ok and a client is connected, otherwise nothing is done.
* Return COM_ERR_SSL_CNX_CLOSED if connection is closed, in this case you ctx has to be freed
*/
T_com_status cryptoal_create_connect_tls_server_ctx ( char * IP,  char * port, bool two_ways_authentication,
                                                     ssl_context ** ppCtx, net_context * pClientCtx )
{
    T_com_status returnCode = COM_SUCCESS;
    int ret = 0;

    START_FUNCTION();

    VERIF_ARGS("IP pointer shall not be Null", IP != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("port pointer shall not be Null", port != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("ppCtx pointer shall be not null", ppCtx != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS(" * ppCtx pointer shall be null", * ppCtx == NULL, returnCode, COM_ERR_PARAM);
    if (returnCode == COM_ERR_PARAM)
    {
        goto exit;
    }

    /* TODO */


exit:

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
 *   TODO: time Out to exit or NULL.
 * 
 * return COM_SUCCESS if execution is Ok and local client is connected, otherwise nothing is done.
* Return COM_ERR_SSL_CNX_CLOSED if connection is closed, in this case you ctx has to be freed
*/
T_com_status cryptoal_create_connect_tls_client_ctx ( char * IP,  char * port,  ssl_context ** ppCtx )
{
    T_com_status returnCode = COM_SUCCESS;
    int ret = 0;
    
    START_FUNCTION();

    VERIF_ARGS("IP pointer shall not be Null", IP != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("port pointer shall not be Null", port != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("ppCtx pointer shall be not null", ppCtx != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS(" * ppCtx pointer shall be null", * ppCtx == NULL, returnCode, COM_ERR_PARAM);
    if (returnCode == COM_ERR_PARAM)
    {
        goto exit;
    }

    /* TODO */


exit:

    END_FUNCTION();
    return returnCode;
}


/* Read on ssl connection, client OR server no matter, using context as reference.
 * Here a client is already coonected, and has performed ssl handshake.
 * 
 * Calling this function may be blocking for a while, until data are read.
 * 
 * return COM_SUCCESS if execution is Ok and data are read, otherwise nothing is done.
 * If error COM_ERR_SSL_CNX_CLOSED is returned, that means connection has been closed for
 * any reason so caller shall free resources of connection.
 */
//TODO set a timeOut
T_com_status cryptoal_read_tls_ctx ( ssl_context * pCtx, void * pBuf, size_t lenBuf, size_t * pReadLen)
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
    VERIF_ARGS(" * pReadLen shall be null", * pReadLen == 0u, returnCode, COM_ERR_PARAM);
    if (returnCode == COM_ERR_PARAM)
    {
        goto exit;
    }

    /* TODO */


exit:
    END_FUNCTION();
    return returnCode;
}

/* Write on ssl connection, client OR server no matter, using context as reference
 * Calling this function may be blocking for a limited time, until data are written. Written size is given back.
 * 
 * returns COM_SUCCESS if execution is Ok and some data are written (total or partial), otherwise nothing is done.
 * returns COM_ERR_SSL_CNX_CLOSED if ssl session was peviously closed, in this case caller shall free resources after.
*/
    T_com_status cryptoal_write_tls_ctx ( ssl_context * pCtx, void * pBuf, size_t lenBuf, size_t * pWrittenLen)
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

    /* TODO */


exit:
    END_FUNCTION();
    return returnCode;
}

/* Close tls connexion (if needed), for client or server no matter,  and Free all context. No problem if connection was previously
 * closed in or outside a command.  Calling this function returns immediatly.
 * 
 * return COM_SUCCESS if execution is Ok, otherwise nothing is done.
*/
T_com_status cryptoal_close_free_tls_ctx ( ssl_context * pCtx, net_context * pClientCtx)
{
    T_com_status returnCode = COM_SUCCESS;
    int ret = 0;
    bool retry_close = true;

    START_FUNCTION();

    VERIF_ARGS("pCtx pointer shall be not null", pCtx != NULL, returnCode, COM_ERR_PARAM);
    if (returnCode == COM_ERR_PARAM)
    {
        goto exit;
    }

    /* TODO */


exit:

    END_FUNCTION();
    return returnCode;
}


#endif  /* CRYPTO_OPENSSL */

