
#ifndef NAL_COMMON_H_
#define NAL_COMMON_H_

#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include <stdarg.h>

/**
 * \brief Platform specific definitions include.
 */
#include "nal_config.h"

/*----------------------------------------------------------------------------*\
 *                             Platform detection                             *
\*----------------------------------------------------------------------------*/

#define NAL_ARG_UNUSED( x )

/**
 * The following lines provides a way to manage the portability
 */
#if defined(__linux__)
#ifndef NAL_MEM_ALLOC
#define NAL_MEM_ALLOC
#endif
#ifndef NAL_RTOS_POSIX
#define NAL_RTOS_POSIX
#endif
#ifndef NAL_LIBC_POSIX
#define NAL_LIBC_POSIX
#endif
#ifndef NAL_STOR_STDIO
#define NAL_STOR_STDIO
#endif
#ifndef NAL_IP_BDS_4_3
#define NAL_IP_BDS_4_3
#endif
#ifndef NAL_NET_LINUX
#define NAL_NET_LINUX
#endif
#else
#error "No platform defined!"
#endif

/*----------------------------------------------------------------------------*\
 *                             General error codes                            *
\*----------------------------------------------------------------------------*/

/** 
 * \brief NAL general error codes.
 */
#define NAL_SUCCESS           (0)   /**< No error                                           */
#define NAL_ERROR            (-1)   /**< Generic error: see implementation error code       */
#define NAL_ENOMEM           (-2)   /**< No memory                                          */
#define NAL_EOPTION          (-3)   /**< Flag/option not supported by the implementation    */
#define NAL_ENORES           (-4)   /**< The resource does not exist                        */
#define NAL_EEOS             (-5)   /**< End of stream                                      */
#define NAL_EACCES           (-6)   /**< Permission denied                                  */
#define NAL_EINVAL           (-7)   /**< Invalid argument                                   */
#define NAL_EIMPL            (-8)   /**< Not supported by the implementation                */
#define NAL_EINTR            (-9)   /**< Operation was interrupted                          */
#define NAL_EAGAIN           (-10)  /**< Operation would block, try again later             */
#define NAL_EINTERNAL        (-11)  /**< Non-recoverable internal error                     */
#define NAL_ERANGE           (-12)  /**< Out of range value                                 */
#define NAL_EEXIST           (-13)  /**< Ressource already existing                         */
#define NAL_EBUSY            (-14)  /**< Ressource is busy, maybe used by another one       */

/*----------------------------------------------------------------------------*\
 *                              IP API error codes                            *
\*----------------------------------------------------------------------------*/

/** 
 * \brief NAL IP API error codes
 * @note These error codes are produced by the BSD-like socket API and are direct
 * translations of the BSD standard codes. They are retrieved using al_ip_error().
 */
#define NAL_EIO              (-101)  /**< Input/output error */
#define NAL_EBADF            (-102)  /**< Bad descriptor */
#define NAL_EFAULT           (-103)  /**< Bad address */
#define NAL_EMFILE           (-104)  /**< Out of descriptors */
#define NAL_EINPROGRESS      (-105)  /**< Operation now in progress */
#define NAL_EALREADY         (-106)  /**< Operation already in progress */
#define NAL_ENOTSOCK         (-107)  /**< Not a socket descriptor */
#define NAL_EDESTADDRREQ     (-108)  /**< Destination address required */
#define NAL_EMSGSIZE         (-109)  /**< Message too big */
#define NAL_EPROTOTYPE       (-110)  /**< Wrong protocol type */
#define NAL_ENOPROTOOPT      (-111)  /**< Not a valid protocol option */
#define NAL_EPROTONOSUPPORT  (-112)  /**< Protocol not supported */
#define NAL_ESOCKTNOSUPPORT  (-113)  /**< Socket type not supported */
#define NAL_EOPNOTSUPP       (-114)  /**< Operation not supported */
#define NAL_EPFNOSUPPORT     (-115)  /**< Protocol family not supported */
#define NAL_EAFNOSUPPORT     (-116)  /**< Address family not supported */
#define NAL_EADDRINUSE       (-117)  /**< Address already in use */
#define NAL_EADDRNOTAVAIL    (-118)  /**< Address not available from this host */
#define NAL_ENETDOWN         (-119)  /**< Network down */
#define NAL_ENETUNREACH      (-120)  /**< Network unreachable */
#define NAL_ENETRESET        (-121)  /**< Network dropped connection on reset */
#define NAL_ECONNABORTED     (-122)  /**< Connection aborted by user */
#define NAL_ECONNRESET       (-123)  /**< Connection reseted by peer */
#define NAL_ENOBUFS          (-124)  /**< Out of resources */
#define NAL_EISCONN          (-125)  /**< Already connected */
#define NAL_ENOTCONN         (-126)  /**< Socket not connected */
#define NAL_ESHUTDOWN        (-127)  /**< Can't send after shutdown */
#define NAL_ETOOMANYREFS     (-128)  /**< Too many references */
#define NAL_ETIMEDOUT        (-129)  /**< Connection timed out */
#define NAL_ECONNREFUSED     (-130)  /**< Connection refused */
#define NAL_ELOOP            (-131)  /**< Too many levels of symbolic links */
#define NAL_ENAMETOOLONG     (-132)  /**< Component of the path name too long */
#define NAL_EHOSTDOWN        (-133)  /**< Host down */
#define NAL_EHOSTUNREACH     (-134)  /**< Host unreachable */
#define NAL_ENOTEMPTY        (-135)  /**< Directory not empty */
#define NAL_ENXIO            (-136)  /**< Device not configured */
#define NAL_EHOSTNOTFOUND    (-137)  /**< Host not found */
#define NAL_ENODATA          (-138)  /**< No data for the entry */
#define NAL_ENOSPC           (-139)  /**< No enough space in the supplied buffer */

/*----------------------------------------------------------------------------*\
 *                             General constants definitions                  *
\*----------------------------------------------------------------------------*/

/** 
 * \brief NAL general definitions.
 * @note These values and types can be used in any of the AL interfaces
 */
#define NAL_BOOL              int       /**< Booleans definitions */
#define NAL_TRUE              (1)       /**< True */
#define NAL_FALSE             (0)       /**< False */

/**
 * Multi-purpose fine-grained time structure.
 */
typedef struct
{
    uint32_t sec; /**< Number of seconds. */
    uint32_t usec; /**< Number of micro-seconds. */
} T_nal_time;

#endif /* NAL_COMMON_H_ */
