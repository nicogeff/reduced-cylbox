/**
 * \file
 * \brief utilities for CRYPTO_AL sub-module
 * Common to all crypto libraries, but some content of functiun should be specific.
 * Goal is to initialize module in relation with selected library, and to minimize compilation switches usage.
 *
 * \date 04/2020 NGE Creation
 */



/*------------------------------ Include files -------------------------------*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include "tools_trace.h"

#include "com_error.h"
#include "com_entity.h"
#include "com_cryptolib_header.h"


/*------------------------------ Settings -----------------------------------*/
/* N/A */



/* ----------------- API Functions for sub-module  --------------- */


/**
 * \brief DO initialization mandatory to prepare selected library to work
 *
 * \details 
 * \return A status code
 */
T_com_status cryptoal_initialize
(
 void
)
{
    T_com_status returnCode = COM_SUCCESS;

    START_FUNCTION();
    TRACE_GEN(TRACE, "Initializing selected library..." , __FUNCTION__, __LINE__ );

#if defined CRYPTO_OPENSSL
    /* Initialization specific to,OPENSSL */
    /* Load the human readable error strings for libcrypto */
    ERR_load_crypto_strings();
    /* Load all digest and cipher algorithms */
    OpenSSL_add_all_algorithms();
    /* Load config file, and other important initialisation */
    OPENSSL_config(NULL);
    
#elif defined  CRYPTO_MBEDTLS
/* There's non initialization for this library */
#endif


    /* Manages certificates, create directories if none, generate local autosigned certificate if none: */

    /* TODO */
    
    
    
    
exit:

    END_FUNCTION();
    return returnCode;
}

/**
 * \brief DO closing openssl library to stop working
 *
 * \details 
 * \return A status code
 */
T_com_status cryptoal_reset
(
 void
)
{
    T_com_status returnCode = COM_SUCCESS;

    START_FUNCTION();
    TRACE_GEN(TRACE, "Closing openssl library..." , __FUNCTION__, __LINE__ );


#if defined CRYPTO_OPENSSL
    /* Removes all digests and ciphers */
    EVP_cleanup();
    /* if you omit the next, a small leak may be left when you make use of the BIO (low level API) for e.g. base64 transformations */
    CRYPTO_cleanup_all_ex_data();
    /* Remove error strings */
    ERR_free_strings();

#elif defined CRYPTO_MBEDTLS
/*  Nothing to do for MbedTls*/
#endif

exit:

    END_FUNCTION();
    return returnCode;
}


