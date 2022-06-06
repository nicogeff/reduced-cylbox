
#ifndef NAL_STOR_H_
#define NAL_STOR_H_

#include "nal_common.h"

#if defined NAL_STOR_STDIO
#include "nal_stor_stdio.h"
#else
#error "No OS defined ..."
#endif

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------*\
 *                             Stream API                                     *
\*----------------------------------------------------------------------------*/

/** 
 * Define the position seek 
 */
#define NAL_SEEK_SET                0  /* Beginning of file */
#define NAL_SEEK_CUR                1  /* Current position of the file pointer */
#define NAL_SEEK_END                2  /* End of file */

/**
 * The stream is open for reading.
 */
#define NAL_STREAM_READ             1

/**
 * The stream is open for writing starting at the beginning of the stream.
 */
#define NAL_STREAM_WRITE_OVERWRITE  2

/**
 * The stream is open for writing starting at the end of the stream.
 */
#define NAL_STREAM_WRITE_APPEND     3
    
/**
 * The directory permission definition
 */
#define NAL_DIR_READ    0400
#define NAL_DIR_WRIT    0200
#define NAL_DIR_EXEC    0100
 

extern int nal_streamInit
(
 T_nal_stream *stream,
 const char *id,
 int mode
 );

extern int nal_streamRead
(
 T_nal_stream *in_stream,
 void *buf,
 unsigned long *size
 );

extern int nal_streamWrite
(
 T_nal_stream *out_stream,
 const void *buf,
 unsigned long *size
 );

extern int nal_streamFlush
(
 T_nal_stream *stream
 );

extern int nal_streamClose
(
 T_nal_stream *stream
 );

extern int nal_streamGetLastError
(
 T_nal_stream *stream
 );

extern int nal_streamSeek
(
 T_nal_stream *stream,
 long offset,
 int origin
 );

extern int nal_streamRemove
(
 const char *filename
 );

extern int nal_streamTell
(
 T_nal_stream *stream
 );

extern int nal_streamRewind
(
 T_nal_stream *stream
 );

extern int nal_streamGetFileSize
(
 const char *filename,
 unsigned long *fileSize
 );

extern int nal_streamMove
(
 const char *src,
 const char *dst
 );

extern int nal_isDirExist
(
 const char *dirname
 );

extern int nal_dirCreate
(
 const char *dirname,
 int mode
 );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* NAL_STOR_H_ */
