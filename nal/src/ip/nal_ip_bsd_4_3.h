
#ifndef NAL_IP_BSD_4_3_H_
#define NAL_IP_BSD_4_3_H_

#include <fcntl.h>
#include <sys/socket.h>
/* #include <sys/filio.h> */
#include <sys/ioctl.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <sys/select.h>

#ifdef NAL_HAVE_IPV4
#define NAL_SOCKADDR_STORAGE_SIZE   sizeof(struct sockaddr)
#define NAL_SOCKADDR_DATA_SIZE     (sizeof(struct sockaddr) - sizeof(short))
#else /* IPv6 */
#define NAL_SOCKADDR_STORAGE_SIZE   sizeof(struct sockaddr_storage)
#define NAL_SOCKADDR_DATA_SIZE     (sizeof(struct sockaddr_storage) - sizeof(short))
#endif

# define NAL_INVALID_SOCKET  (unsigned int)(~0)

typedef fd_set nal_fd_set;

#define NAL_FD_ZERO     FD_ZERO
#define NAL_FD_SET      FD_SET
#define NAL_FD_ISSET    FD_ISSET

/**
 * Portable command flag for specifying non-blocking IOs.
 */
#define NAL_FIONBIO	FIONBIO

#endif /* NAL_IP_BSD_4_3_H_ */
