#ifndef CRYPTOAL_COMMON_H
#define CRYPTOAL_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Platform specific definitions include.
 */
#include "cryptoal_config.h"
    
#if defined(__linux__)
#include <stdbool.h>
#if !defined (OPENSSL_LIB) && !defined (MBEDTLS_LIB)
//#error "No crypto lib defined!"
#endif /* OPENSSL_LIB && MBEDTLS_LIB */
#else
#error "No platform defined!"
#endif /* __linux__ */

/*----------------------------------------------------------------------------*\
 *                             General error codes                            *
\*----------------------------------------------------------------------------*/

/** 
 * \brief General error codes
 */
#define CRYPTOAL_SUCCESS    (0)     /**< No error */
#define CRYPTOAL_ERROR      (-1)    /**< Undefine error */
#define CRYPTOAL_EPARAM     (-2)    /**< Parameter error  */

#ifdef __cplusplus
}
#endif

#endif /* CRYPTOAL_COMMON_H */

