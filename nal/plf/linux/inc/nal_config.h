
#ifndef NAL_CONFIG_H_
#define NAL_CONFIG_H_

/** 
 * Defining Endianness 
 */
#define NAL_LITTLE_ENDIAN
/* #define NAL_BIG_ENDIAN */

/** 
 * Define non-build modules to reduce footprint
 */
/* #define NO_NAL_RTOS_POSIX_THREAD_API */
/* #define NO_NAL_RTOS_POSIX_SEM_API */
/* #define NO_NAL_RTOS_POSIX_QUEUE_API */
/* #define NO_NAL_RTOS_POSIX_MUTEX_API */
/* #define NO_NAL_RTOS_POSIX_TIME_API */
/* #define NO_NAL_LIBC_POSIX */
/* #define NO_NAL_STOR_STDIO */
/* #define NO_NAL_IP_BSD_4_3 */
/* #define NO_NAL_NET_LINUX */

/**
 * Uncomment this line to use the NAL internal memory allocator
 */
/* #define NAL_INTERNAL_ALLOC */

/** 
 * IP Stack version supported by the target 
 */
#define NAL_HAVE_IPV4
/* #define NAL_HAVE_IPV6 */
#define NAL_HAVE_RECVMSG

#endif /* NAL_CONFIG_H_ */
