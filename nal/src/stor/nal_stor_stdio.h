
#ifndef NAL_STOR_STDIO_H_
#define NAL_STOR_STDIO_H_

#include <stdint.h>
#include <stdio.h>

/**
 * \brief    Max number of characters before the '.'
 */
#define NAL_FILE_MAX_FILEBASE_SIZE   64

/**
 * \brief    Max number of characters after  the '.'
 */
#define NAL_FILE_MAX_FILEEXTN_SIZE   3

/**
 * \brief    sizeof('.') + sizeof('\0')
 */
#define NAL_FILE_MAX_FILEDOT0_SIZE   2

/**
 * \brief    Max length of filename with complete path
 */
#define NAL_FILE_MAX_FILENAME_SIZE   (NAL_FILE_MAX_FILEBASE_SIZE+NAL_FILE_MAX_FILEEXTN_SIZE+NAL_FILE_MAX_FILEDOT0_SIZE)

/**
 * \brief    Max length of filename with complete path
 */
#define NAL_FILE_MAX_FILEPATH_SIZE   256


#define NAL_INVALID_FILE_HANDLE      0xFFFFFFFF

typedef struct nal_stream_struct
{
  FILE  *fileDesc;
  int   lastError;
  int   openflag;
  char  *name;
} T_nal_stream;

#endif /* NAL_STOR_STDIO_H_ */
