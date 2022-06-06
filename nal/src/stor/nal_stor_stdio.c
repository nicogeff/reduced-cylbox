
#include "nal_stor.h"
#include "nal_mem.h"

#include <sys/stat.h>
#include <string.h>
#include <errno.h>

#ifdef NO_NAL_STOR_STDIO
#warning "STOR module disabled..."
#else

/**
 * \brief Open a stream on a file.
 * @param[in] stream File handler.
 * @param[in] id File identifier.
 * @param[in] mode Opening mode
 * @return \n
 * NAL_SUCCESS: File successfully opened.
 * NAL_EINVAL: Invalid (NULL) parameter.
 * NAL_ENORES: flag set to AL_STREAM_READ, and file does not exist, or invalid path.
 * NAL_EACCES: Permission denied.
 * @todo Return use instead of result
 */
int nal_streamInit
(
 T_nal_stream *stream,
 const char *id,
 int mode
 )
{
    int result = NAL_SUCCESS;

    if ((stream == NULL) || (id == NULL) || (mode < 0))
    {
        result = NAL_EINVAL;
    }
    else
    {
        stream->name = NAL_MALLOC(strlen(id) + 1);
        strcpy(stream->name, id);
        stream->openflag = NAL_INVALID_FILE_HANDLE;
        switch (mode)
        {
        case NAL_STREAM_READ:
            stream->fileDesc = fopen(id, "rb");
            break;

        case NAL_STREAM_WRITE_OVERWRITE:
            stream->fileDesc = fopen(id, "wb");
            break;

        case NAL_STREAM_WRITE_APPEND:
            stream->fileDesc = fopen(id, "ab");
            break;

        default:
            stream->fileDesc = (FILE*) NAL_INVALID_FILE_HANDLE;
            stream->lastError = NAL_EINVAL;
            return NAL_EINVAL;
        }

        if (stream->fileDesc == NULL)
        {
            result = NAL_ENORES;
        }
        else
        {
            result = NAL_SUCCESS;
        }
        stream->openflag = mode;
    }

    if (stream != NULL)
    {
        stream->lastError = result;
    }
    return result;
}

/**
 * \brief Read data from an opened stream
 * @param[in] in_stream Handle on the stream to read in.
 * @param[in] buf Buffer to receive read data.
 * @param[in] size Size of the data to read in bytes.
 * @return \n
 * NAL_SUCCESS: Read success.
 * NAL_EINVAL: Invalid (NULL) parameter.
 * NAL_EEOS: End of file was encountered before size bytes were read.
 * NAL_EACCES: Permission denied.
 */
int nal_streamRead
(
 T_nal_stream *in_stream,
 void *buf,
 unsigned long *size
 )
{
    int result = NAL_SUCCESS;
    size_t wlen = 0u;

    if ((in_stream == NULL) || (buf == NULL) || (size == NULL))
    {
        if (size != NULL)
        {
            *size = 0;
        }
        result = NAL_EINVAL;
    }
    else if (in_stream->openflag != NAL_STREAM_READ)
    {
        *size = 0;
        result = NAL_EACCES;
    }
    else
    {
        wlen = (size_t) * size;
        *size = (unsigned long) fread((void *) buf, 1, wlen, (in_stream->fileDesc));
        if (*size == wlen)
        {
            result = NAL_SUCCESS;
        }
        else
        {
            /* Check if the EOF was encountered */
            if (feof(in_stream->fileDesc))
            {
                result = NAL_EEOS;
            }
            else
            {
                result = NAL_EACCES;
            }
        }
    }

    if (in_stream != NULL)
    {
        in_stream->lastError = result;
    }
    return result;
}

/**
 * \brief Write data to an opened stream
 * @param[in] out_stream Handle on the stream to write in.
 * @param[in] buf Data buffer to write.
 * @param[in] size Size of the data to write in bytes.
 * @return \n
 * NAL_SUCCESS: Write success.
 * NAL_EINVAL: Invalid (NULL) parameter or size == 0.
 * NAL_EACCES: Permission denied
 */
int nal_streamWrite
(
 T_nal_stream *out_stream,
 const void *buf,
 unsigned long *size
 )
{
    int result = NAL_SUCCESS;
    size_t wlen = 0u;

    if ((out_stream == NULL) || (buf == NULL) || (size == NULL))
    {
        if (size != NULL)
        {
            *size = 0;
        }
        result = NAL_EINVAL;
    }
    else if ((out_stream->openflag != NAL_STREAM_WRITE_OVERWRITE) &&
            (out_stream->openflag != NAL_STREAM_WRITE_APPEND))
    {
        *size = 0;
        result = NAL_EACCES;
    }
    else
    {
        wlen = *size;
        *size = (unsigned long) fwrite((void *) buf, 1, wlen, out_stream->fileDesc);
        if (*size == wlen)
        {
            result = NAL_SUCCESS;
        }
        else
        {
            /* Check if the EOF was encountered */
            if (feof(out_stream->fileDesc) == 0)
            {
                result = NAL_EEOS;
            }
            else
            {
                result = NAL_EACCES;
            }
        }
    }

    if (out_stream != NULL)
    {
        out_stream->lastError = result;
    }
    return result;
}

/**
 * \brief Force writing of all data in user-space buffers.
 * @param[in] stream Handle on the stream to flush
 * @return \n
 * NAL_SUCCESS: flush success.
 * NAL_EINVAL: Invalid (NULL) parameter or size == 0.
 * NAL_EEOS: End of stream.
 */
int nal_streamFlush
(
 T_nal_stream *stream
 )
{
    int result = NAL_SUCCESS;

    if (stream == NULL)
    {
        result = NAL_EINVAL;
    }
    else
    {
        result = fflush(stream->fileDesc);

        if (result == EOF)
        {
            result = NAL_EEOS;
        }
        else
        {
            result = NAL_SUCCESS;
        }
    }

    if (stream != NULL)
    {
        stream->lastError = result;
    }
    return result;
}

/**
 * \brief Return the last nal_streamXXX error encountered
 * @param[in] stream The stream to return the last error.
 * @return \n
 * NAL_EINVAL : Invalid (NULL) parameter.
 */
int nal_streamGetLastError
(
 T_nal_stream *stream
 )
{
    if (stream == NULL)
    {
        return NAL_EINVAL;
    }
    return stream->lastError;
}

/**
 * \brief Close a stream previously opened
 * @param[in] stream Stream to close.
 * @return \n
 * NAL_SUCCESS: Stream closed.
 * NAL_EINVAL: Invalid (NULL) parameter.
 * NAL_EACCES: Permission denied.
 */
int nal_streamClose
(
 T_nal_stream *stream
 )
{
    int result = NAL_SUCCESS;

    if (stream == NULL || stream->fileDesc == NULL)
    {
        result = NAL_EINVAL;
    }
    else
    {
        if (!fclose(stream->fileDesc))
        {
            result = NAL_SUCCESS;
        }
        else
        {
            result = NAL_EACCES;
        }
    }

    if (stream != NULL)
    {
        NAL_FREE(stream->name);
        stream->lastError = result;
        stream->openflag = NAL_INVALID_FILE_HANDLE;
    }

    return result;
}

/**
 * \brief Set the position indicator associated with the stream to a new position
 * @param[in] stream A stream initialized.
 * @param[in] offset Offset (in bytes) to add to the position specified by the origin
 * @param[in] origin Can take 3 values NAL_SEEK_SET, NAL_SEEK_CUR or NAL_SEEK_END
 * @return \n
 * NAL_SUCCESS: Position indicator modified.
 * NAL_EINVAL: Invalid (NULL) parameter.
 * NAL_EACCES: Permission denied.
 * NAL_EEOS: Offset lower than 0
 */
int nal_streamSeek
(
 T_nal_stream *stream,
 long offset,
 int origin
 )
{
    int result = NAL_SUCCESS;
    long curr_pos = 0u;
    long len_file = 0u;

    if (((stream == NULL) || (stream->fileDesc == NULL)) ||
            ((origin != NAL_SEEK_SET) && (origin != NAL_SEEK_CUR)
            && (origin != NAL_SEEK_END)))
    {
        result = NAL_EINVAL;
    }
    else
    {
        if (stream->openflag == NAL_INVALID_FILE_HANDLE)
        {
            result = NAL_EACCES;
        }
        else
        {
            fseek(stream->fileDesc, 0, NAL_SEEK_END);
            len_file = ftell(stream->fileDesc);
            if (-1L == len_file)
            {
                result = NAL_EEOS;
            }
            else
            {
                curr_pos = ftell(stream->fileDesc);
                switch (origin)
                {
                case NAL_SEEK_SET:
                    origin = NAL_SEEK_SET;
                    curr_pos = offset;
                    break;
                case NAL_SEEK_CUR:
                    origin = NAL_SEEK_CUR;
                    curr_pos = curr_pos + offset;
                    break;
                case NAL_SEEK_END:
                    curr_pos = len_file + offset;
                    origin = NAL_SEEK_END;
                    break;
                }
                if ((curr_pos > len_file) || (curr_pos) < 0)
                {
                    result = NAL_EEOS;
                }
                else
                {
                    if (fseek(stream->fileDesc, offset, origin) == 0)
                    {
                        result = NAL_SUCCESS;
                    }
                }
            }
        }
    }

    if (stream != NULL)
    {
        stream->lastError = result;
    }
    return result;
}

/**
 * \brief Delete a file referenced by it's name, or name + path
 * @param[in] filename Name (or name + path) of the file to delete.
 * @return \n
 * NAL_SUCCESS : File erased.
 * NAL_EINVAL : Invalid (NULL) parameter.
 * NAL_ENORES : File does not exist.
 */
int nal_streamRemove
(
 const char *filename
 )
{
    int result = NAL_SUCCESS;
    struct stat filestat;

    if (filename == NULL)
    {
        result = NAL_EINVAL;
    }
    else
    {
        if (stat(filename, &filestat) == 0)
        {
            result = remove(filename);
        }
        else
        {
            result = -1;
        }

        if (result == 0)
        {
            result = NAL_SUCCESS;
        }
        else
        {
            result = NAL_ENORES;
        }
    }
    return result;
}

/**
 * \brief Return the current value of the position indicator associated to the stream.
 * @param[in] stream  A stream initialized by a call to al_stream_init.
 * @return value of the position >= 0 or NAL_ERROR
 */
int nal_streamTell
(
 T_nal_stream *stream
 )
{
    int result = NAL_SUCCESS;

    if (stream == NULL)
    {
        result = NAL_EINVAL;
    }
    else
    {
        /* Test a valid stream with fseek otherwise ftell crash if the file is closed or removed before */
        result = fseek(stream->fileDesc, 0, SEEK_CUR);
        if (result != 0)
        {
            result = NAL_ERROR;
        }
        else
        {
            result = ftell(stream->fileDesc);
        }
    }

    if (stream != NULL)
    {
        stream->lastError = result;
    }
    return result;
}

/**
 * \brief Set the position indicator associated with the stream to the origin.
 * @param[in] stream A stream initialized by a call to nal_streamInit().
 * @return \n
 * NAL_SUCCESS: Position returned valid.
 * NAL_EINVAL: Invalid (NULL) parameter.
 * NAL_EACCES: Permission denied.
 */
int nal_streamRewind
(
 T_nal_stream *stream
 )
{
    int result = NAL_SUCCESS;

    if (stream == NULL)
    {
        result = NAL_EINVAL;
    }
    else
    {
        /* Test a valid stream with fseek otherwise rewind crash if the file is closed or removed before*/
        if (fseek(stream->fileDesc, 0, SEEK_CUR))
        {
            result = NAL_EACCES;
        }
        else
        {
            rewind(stream->fileDesc);
            result = NAL_SUCCESS;
        }
    }

    if (stream != NULL)
    {
        stream->lastError = result;
    }
    return result;
}

/**
 * \brief Return the size in fileSize of the file specified
 * @param[in] filename File name and path.
 * @param[out] fileSize Size of the specified file in bytes
 * @return \n
 * NAL_SUCCESS: Getting file size successful.
 * NAL_EINVAL: Invalid (NULL) parameter.
 * NAL_ENORES: File doesn't exist.
 */
int nal_streamGetFileSize
(
 const char *filename,
 unsigned long *fileSize
 )
{
    int result = NAL_SUCCESS;
    FILE *fp;

    if ((filename == NULL) || (fileSize == NULL))
    {
        result = NAL_EINVAL;
    }
    else
    {
        /* open the file */
        fp = fopen(filename, "r");
        if (fp == NULL)
        {
            /* Inexistent file */
            result = NAL_ENORES;
        }
        else
        {
            /* File opened: Get the size */
            fseek(fp, 0, SEEK_END);
            *fileSize = ftell(fp);
            result = NAL_SUCCESS;

            /* Close the file */
            fclose(fp);
        }
    }
    return result;
}

/**
 * \brief Rename / move a file
 * @param src Source file: containing the complete path and the name of the source file to be move/renamed. 
 * @param dst Destination file: containing the complete path and the name of the destinate file to be move/renamed.
 * @return \n
 * NAL_SUCCESS : Position returned valid.
 * NAL_EINVAL : Invalid (NULL) parameter.
 * NAL_EACCES : Permission denied.
 */
int nal_streamMove
(
 const char *src,
 const char *dst
 )
{
    int result = NAL_SUCCESS;

    if ((src == NULL) || (dst == NULL))
    {
        result = NAL_EINVAL;
    }
    else
    {
        /* Test a valid stream with fseek otherwise rewind crash if the file is closed or removed before*/
        result = rename(src, dst);
        if (result != 0)
        {
            result = NAL_EACCES;
        }
        else
        {
            result = NAL_SUCCESS;
        }
    }
    return result;
}

/**
 * \brief Check if dirname exist
 * @param dirname directory path who will be checked
 * @return \n
 * NAL_SUCCESS : The directory path exist
 * NAL_ENORES : The directory path unexist
 */
int nal_isDirExist
(
 const char *dirname
 )
{
    int result = NAL_SUCCESS;
    struct stat info;
  
    if ((stat(dirname, &info) != 0) || 
        (!S_ISDIR(info.st_mode)))
    {
        result = NAL_ENORES;
    } 
    
    return result;
}

/**
 * \brief Create a directory with parent option
 * \note PATH_MAX limitation (limits.h)
 * \todo need to modify errno = !!! That is unacceptable!
 * \param path
 * \param mode
 * \return 
 */
static int nal_dirCreateWithParent(const char *path, int mode)
{
    size_t len = 0u;
    char _path[PATH_MAX] = {0};
    char *p = NULL; 

    errno = 0;
    
    if (path == NULL)
        return -1;
    
    len = strlen(path);

    /* Copy string so its mutable */
    if (len > sizeof(_path)-1) {
        errno = ENAMETOOLONG;
        return -1; 
    }   
    strcpy(_path, path);

    /* Iterate the string */
    for (p = _path + 1; *p; p++) {
        if (*p == '/') {
            /* Temporarily truncate */
            *p = '\0';

            if (mkdir(_path, mode) != 0) {
                if (errno != EEXIST)
                    return -1; 
            }

            *p = '/';
        }
    }   

    if (mkdir(_path, mode) != 0) {
        if (errno != EEXIST)
            return -1; 
    }   

    return 0;
}

/**
 * \brief Create the directory dirname with parent option
 * @param dirname directory path who will be checked
 * @return \n
 * NAL_SUCCESS : The directory path exist
 * NAL_EACCES : Permission deny
 * NAL_EEXIST : The directory already existing
 */
int nal_dirCreate
(
 const char *dirname,
 int mode
 )
{
    int result = NAL_SUCCESS;
    int mkdirRes = 0;
    
    mkdirRes = nal_dirCreateWithParent(dirname, mode);
    if (mkdirRes != 0)
    {
        if (mkdirRes == EACCES)
            result = NAL_EACCES;
        else if (mkdirRes == EEXIST)
            result = NAL_EEXIST;
    }
    else
    {
        result = NAL_SUCCESS;
    }
    
    return result;
}

#endif /* NO_NAL_STOR_STDIO */
