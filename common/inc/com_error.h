/*
 * \file
 * \brief All error status in software
 * \specifics
 *
 */


#ifndef COM_ERROR_H
#define COM_ERROR_H

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * \brief Description of all error code
     */
    typedef enum com_error_enum {
        COM_SUCCESS = 0, /* Operation successful, ONLY this one is not an error */

        /* Generic errors */
        COM_ERR_PARAM = 1,          /* One or more given parameter(s) is(are) invalid */
        COM_ERR_NO_MEMORY = 2,      /* Memory error */
        COM_ERR_SIZE = 3,           /* Size error */
        COM_ERR_TASK = 4,           /* Task error */
        COM_ERR_THREAD = 5,         /* Thread error */
        COM_ERR_QUEUE = 6,          /* Queue error */
        COM_ERR_SEMAPHORE = 7,      /* Semaphore error */
        COM_ERR_MUTEX = 8,          /* Mutex error */
        COM_ERR_FILE = 9,           /* File error */
        COM_ERR_ADDR = 10,          /* Address error */
        COM_ERR_NOT_ALLOWED = 11,   /* Not allowed action */
        COM_ERR_BUSY = 13,          /* Busy error */

        /* Network */
        COM_ERR_SOCKET = 20,
        COM_ERR_BIND = 21,
        COM_ERR_CONNECT = 22,
        COM_ERR_RECV = 23,
        COM_ERR_SND = 24,
        COM_ERR_SSL_CNX_CLOSED = 25,
        COM_ERR_SSL_CERTIFICATE_ERROR = 26,
        COM_ERR_SSL_BIND_ERROR = 27,
        COM_ERR_SSL_CRYPTO_ERROR = 28,
        COM_ERR_SSL_HANDSHAKE_ERROR = 29,

        /* Element error */
        COM_ERR_UNKNOWN_ELEMENT = 30,
        COM_ERR_MAX_ELEMENTS = 31,
        COM_ERR_MAX_PARAMETERS = 32,
        COM_ERR_PATH = 33,
        COM_ERR_CONFIG = 34,
        COM_ERR_MALFORMED_ELEMENT = 36,
        COM_ERR_MISSING_ELEMENT = 37,
        COM_ERR_JSON = 38,

        /* Document error */
        COM_ERR_DOCUMENT = 40,
        COM_ERR_STRUCTURE = 41,
        COM_ERR_TUPLE_MISSING = 42,         /* For the "Document" */
        COM_ERR_TUPLE_BAD_KEY = 43,
        COM_ERR_TUPLE_BAD_VALUE = 44,

        /* About status of a SW entity */
        COM_ERR_UNINITIALIZED = 50,         /* Entity should be initialized */
        COM_ERR_UNCONFIGURED = 51,          /* Entity should be configured */
        COM_ERR_UNSTARTED = 52,             /* Entity should be started */
        COM_ERR_UNSTOPPED = 53,             /* Entity should be stopped */

        /* Protocol error */
        COM_ERR_WEB_SERVICE = 51,
        COM_ERR_SOAP = 52,
        COM_ERR_REST = 53,
        COM_ERR_AUTHENTICATION = 54,
        COM_ERR_CRYPTO = 55,
        COM_ERR_IP = 56,
        COM_ERR_UDP = 57,
        COM_ERR_TCP = 58,
        COM_ERR_ICMP = 59,
        COM_ERR_TLS = 60,
        COM_ERR_SSH = 61,
        COM_ERR_SYSLOG = 62,

        /* HW device error */
        COM_ERR_GPIO   = 70,
        COM_ERR_RFID   = 71,
        COM_ERR_NETIF = 72,

                
        /* Unclassifiable error */
        COM_ERR_INTERNAL = 80           /* Internal error */

    } T_com_status;

#ifdef __cplusplus
}
#endif

#endif /* COM_ERROR_H */
