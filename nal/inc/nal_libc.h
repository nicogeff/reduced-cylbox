
#ifndef NAL_STDLIB_H_
#define NAL_STDLIB_H_

#include "nal_common.h"

#if defined NAL_LIBC_POSIX
#else
#error "No OS defined ..."
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#define NAL_TRUNCATE     -1 

#define NAL_CONS_BUFF_SIZE  256

#define NAL_OBJ_RDONLY      00
#define NAL_OBJ_WRONLY      01
#define NAL_OBJ_RDWR        02    

extern int nal_printf
(
 const char * message,
 ...
 );

extern int nal_scanf
(
 const char * message,
 void * param,
 unsigned int sz
 );

extern int nal_snprintf_s
(
 char *buffer,
 size_t bufferSize,
 size_t nbCharMax,
 size_t *writtenSize,
 const char *fmt,
 ...
 );

extern int nal_strncpy_s
(
 char *dest,
 size_t destSize,
 const char *source,
 size_t nbCharMax
 );

extern int nal_strncat_s
(
 char *dest,
 size_t destSize,
 const char *strSource,
 size_t nbCharMax
 );

extern int nal_strnlen
(
 const char *source,
 size_t maxLen,
 size_t *size
 );

extern int nal_objectOpen
(
 int *ptr,
 const char *obj,
 const int flag
 );

extern int nal_objectClose
(
 int fd
 );

extern int nal_objectRead
(
 int fd,
 void *buf,
 size_t *count
 );

extern int nal_objectWrite
(
 int fd,
 const void *buf,
 size_t *count
 );

extern int nal_objectIoctl
(
 int fd,
 unsigned long cmd,
 int addr
 );

#ifdef __cplusplus
}
#endif

#endif /* NAL_STDLIB_H_ */
