
#include <stdio.h>
#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "nal_common.h"
#include "nal_libc.h"

#ifdef NO_NAL_LIBC_POSIX
#warning "LIBC module disable!"
#else

/**
 * \brief Print message in console mode.
 * @param[in] message
 * @param[in] ...
 * @return Number of characters printed or an error NAL_EINVAL: invalid (NULL) message parameter.
 */
int nal_printf(const char * message, ...)
{
    int result = NAL_SUCCESS;
    char msg_buffer [NAL_CONS_BUFF_SIZE] = {0};
    va_list args;

    if (message == NULL)
    {
        result = NAL_EINVAL;
    }
    else
    {
        va_start(args, message);
        vsnprintf(msg_buffer,
                  NAL_CONS_BUFF_SIZE,
                  message,
                  args);
        va_end(args);

        msg_buffer[NAL_CONS_BUFF_SIZE - 1] = '\0';
        
        /* To force output even without a carriage return at the end of the message */
        fflush(stdout);
        
        printf("%s", msg_buffer);

        /* To force output even without a carriage return at the end of the message */
        fflush(stdout);

        result = NAL_SUCCESS;
    }
    return result;
}

/**
 * \brief Reading standard input
 * @note Format character implemented : %s and %d.
 * @param[in] format Format character.
 * @param[out] param Pointer on the variable to fill in.
 * @param[in] sz Size of the available memory in the variable param, in bytes.
 * @return \n
 *      NAL_SUCCESS; function correctly executed.
 *      NAL_EINVAL: invalid (NULL) message or param parameters.
 *      NAL_ENOMEM: Not enough memory to write the characters.
 */
int nal_scanf(const char* format, void* param, unsigned int sz)
{
    int result = NAL_SUCCESS;

    if ((format == NULL) || (param == NULL))
    {
        result = NAL_EINVAL;
    }
    else
    {
        if (scanf(format, param) == EOF)
        {
            if (errno == ENOMEM)
                return NAL_ENOMEM;
        }
    }

    return result;
}

/**
 * \brief Secured implementation of the classical snprintf() function with the 
 * following improvements:
 * @li Extended check of the input parameters.
 * @li Extended check of the format string.
 * @li Truncation capabilities.
 * @param[in] buffer Result buffer where the string is written, allocated by the caller.
 * @param[in] bufferSize Size of buffer in bytes.
 * @param[in] nbCharMax Maximum number of characters to write in buffer, including 
 * the NULL termination character. If set to AL_TRUNCATE, the result will be 
 * truncated to the bufferSize, and NULL terminated, if the resulting string is 
 * bigger than bufferSize.
 * @param[out] writtenSize Number of written characters in buffer, not counting the 
 * terminating NULL character. In case of error, other than AL_ERANGE, set to 0
 * @param[in] format Format string. Optional arguments, depending on the format string.
 * @param ...
 * @return \n
 *      NAL_SUCCESS: function correctly executed.
 *      NAL_EINVAL: invalid (NULL) format or buffer, or writtenSize parameters invalid (0 or NbCharMax > bufferSize ) parameter.
 *      NAL_ERANGE: resulting string has been truncated.
 */
int nal_snprintf_s(char *buffer,
                   size_t bufferSize,
                   size_t nbCharMax,
                   size_t *writtenSize,
                   const char *format,
                   ...)
{
    int result = NAL_SUCCESS;
    int do_trunc = 0;
    va_list args;
    size_t buf_size = 0u;

    if (nbCharMax == ((size_t) NAL_TRUNCATE))
        do_trunc = 1;

    /* Sanity checks */
    if ((format == NULL) ||
            (buffer == NULL) ||
            (writtenSize == NULL) ||
            ((nbCharMax > bufferSize) && (!do_trunc)))
    {
        result = NAL_EINVAL;
        if (writtenSize != NULL)
            *writtenSize = 0;
    }
    else
    {
        if (do_trunc)
            buf_size = bufferSize;
        else
            buf_size = nbCharMax;

        va_start(args, format);
        *writtenSize = vsnprintf(buffer, buf_size, format, args);
        va_end(args);

        if ((*writtenSize == -1) || (nbCharMax > bufferSize))
        {
            buffer[bufferSize - 1] = '\0';
            *writtenSize = bufferSize - 1;
            result = NAL_ERANGE;
        }
        else
            result = NAL_SUCCESS;
    }

    return result;
}

/**
 * \brief Secured implementation of the classical strncpy() function with the 
 * following improvements:
 * @li Extended check of the input parameters.
 * @li Truncation capabilities.
 * @param[in] dest Result buffer where the string is copied, allocated by the caller.
 * @param[in] destSize Size of dest in bytes.
 * @param[in] source Source string.
 * @param[in] nbCharMax Maximum number of characters to write in dest. If set to 
 * NAL_TRUNCATE, the result will be truncated to the destSize and NULL terminated, 
 * if the resulting string is bigger than destSize.
 * @return \n
 *      NAL_SUCCESS: function correctly executed.
 *      NAL_EINVAL: invalid (NULL) dest or source, parameters invalid (0 or NbCharMax > destSize ) parameter.  
 *      NAL_ERANGE: resulting string has been truncated.
 */
int nal_strncpy_s(char *dest,
                  size_t destSize, /* must be > 1 in case of NAL_TRUNCATE */
                  const char *source,
                  size_t nbCharMax)
{
    int result = NAL_SUCCESS;
    int do_trunc = 0;
    size_t buf_size = 0u;

    if (nbCharMax == ((size_t) NAL_TRUNCATE))
        do_trunc = 1;

    /* Sanity checks */
    if ((dest == NULL) ||
            (source == NULL) ||
            (destSize <= 1) ||
            ((nbCharMax > destSize) && (!do_trunc)))
    {
        result = NAL_EINVAL;
    }
    else
    {
        if (do_trunc)
            buf_size = destSize;
        else
            buf_size = nbCharMax;

        strncpy(dest, source, buf_size);

        if (do_trunc)
            dest[buf_size - 1] = '\0';

        if (strlen(source) > destSize)
            result = NAL_ERANGE;
        else
            result = NAL_SUCCESS;
    }

    return result;
}

/**
 * \brief Secured implementation of the classical strncat() function with the 
 * following improvements:
 * @li Extended check of the input parameters.
 * @li Truncation capabilities.
 * @param[out] dest Result string buffer where the source string will be concatenated, allocated by the caller.
 * @param[in] destSize Size of dest in bytes.
 * @param[in] strSource Source string.
 * @param[in] nbCharMax Maximum number of characters to write in dest. If set to 
 * NAL_TRUNCATE, the result will be truncated to the destSize and NULL terminated 
 * if the resulting string is bigger than destSize.
 * @return \n
 *      NAL_SUCCESS: function correctly executed.
 *      NAL_EINVAL: invalid (NULL) dest or strSource, parameters. Invalid (0 or NbCharMax > destSize ) parameter.  
 *      NAL_ERANGE: resulting string has been truncated.
 */
int nal_strncat_s(char *dest,
                  size_t destSize,
                  const char *strSource,
                  size_t nbCharMax)
{
    int result = NAL_SUCCESS;
    int do_trunc = 0;
    size_t buf_size = 0u;
    size_t destStrSize = 0u;

    if (nbCharMax == ((size_t) NAL_TRUNCATE))
        do_trunc = 1;

    /* Sanity checks */
    if ((dest == NULL) ||
            (strSource == NULL) ||
            (destSize <= 1) ||
            ((nbCharMax > destSize) && (!do_trunc)))
    {
        result = NAL_EINVAL;
    }
    else
    {
        if (do_trunc)
            buf_size = destSize;
        else
            buf_size = nbCharMax;

        destStrSize = strlen(dest);
        strncat(dest, strSource, buf_size);

        if (do_trunc)
            dest[buf_size - 1] = '\0';

        if (destStrSize + strlen(strSource) + 1 > destSize)
            result = NAL_ERANGE;
        else
            result = NAL_SUCCESS;
    }

    return result;
}

/**
 * \brief Secured implementation of the classical strlen() function
 * @param[in] source String to get the length of.
 * @param[in] maxLen Maximum length to count
 * @param[out] size Number of characters in the string
 * @return \n
 *      NAL_SUCCESS: function correctly executed.
 *      NAL_EINVAL: invalid (NULL) parameters.
 *      NAL_ERANGE: resulting string has been truncated.
 */
int nal_strnlen(const char *source,
                size_t maxLen,
                size_t *size)
{
    int result = NAL_SUCCESS;

    if ((source == NULL) || (size == NULL))
    {
        result = NAL_EINVAL;
    }
    else
    {
        *size = strlen(source);
        if (*size > maxLen)
        {
            *size = maxLen;
        }
    }

    return result;
}

/**
 * \brief Secured implementation of classical open() function
 * This function can open files or resources
 * @param[out] fd File descriptor (valid only if the function return success)
 * @param[in] obj Object to open (file or resource)
 * @param[in] flag Opening mode (NAL_O_RDONLY, NAL_O_WRONLY, NAL_O_RDWR)
 * @return \n
 *      NAL_SUCCESS: function correctly executed.
 *      NAL_EINVAL: invalid (NULL) parameters.
 *      NAL_EACCES: access to file or resource is not possible.
 *      NAL_EEXIST: resource or file already exist or not available.
 *      NAL_EAGAIN: resource or file not available, re-try later.
 *      NAL_ERROR: other error.
 */
int nal_objectOpen(int *fd,
                   const char *obj,
                   const int flag)
{
    int result = NAL_SUCCESS;
    int openRes = 0;

    if ((fd == NULL) || (obj == NULL))
    {
        result = NAL_EINVAL;
    }
    else
    {
        *fd = 0;
        *fd = open(obj, flag);
        if (*fd < 0)
        {
            openRes = errno;
            switch (openRes)
            {
            case EACCES:
                result = NAL_EACCES;
                break;
            case EEXIST:
                result = NAL_EEXIST;
                break;
            case EWOULDBLOCK:
                result = NAL_EAGAIN;
                break;
            default:
                result = NAL_ERROR;
                break;
            }
        }
    }

    return result;
}

/**
 * \brief Secured implementation of classical close() function
 * This function can close files or resources
 * @param[in] fd File descriptor
 * @return \n
 *      NAL_SUCCESS: function correctly executed.
 *      NAL_ERROR: other error.
 */
int nal_objectClose(int fd)
{
    int result = NAL_SUCCESS;
    int closeRes = 0;

    closeRes = close(fd);
    if (closeRes < 0)
    {
        closeRes = errno;
        switch (closeRes)
        {
        case EBADF:
            result = NAL_ERROR;
            break;
        case EINTR:
            result = NAL_ERROR;
            break;
        case EIO:
            result = NAL_ERROR;
            break;
        default:
            result = NAL_ERROR;
            break;
        }
    }
    return result;
}

/**
 * \brief Secured implementation of classical read() function
 * This function can read data from file or resource
 * @param[in] fd File descriptor
 * @param[out] buf Reading data
 * @param[in, out] count Size of data to read, and return the number of data read
 * @return \n
 * NAL_SUCCESS: function correctly executed.
 * NAL_EINVAL: Invalid parameter    
 * NAL_EAGAIN: Unlock written but file descriptor set in lock mode
 * NAL_EBADF: invalid file descriptor, or resource not open
 * NAL_EFAULT: bad pointer of buf
 * NAL_EINTR: function interrupted before the written finished
 * NAL_EIO: input/output error
 * NAL_EINTERNAL: other error
 */
int nal_objectRead(int fd, void *buf, size_t *count)
{
    int result = NAL_SUCCESS;
    int readRes = 0;

    /* Sanity check */
    if (buf == NULL || count == NULL)
    {
        result = NAL_EINVAL;
    }
    else
    {
        *count = read(fd, buf, *count);
        if (*count < 0)
        {
            readRes = errno;
            switch (readRes)
            {
            case EAGAIN:
                result = NAL_EAGAIN;
                break;
            case EBADF:
                result = NAL_EBADF;
                break;
            case EFAULT:
                result = NAL_EFAULT;
                break;
            case EINTR:
                result = NAL_EINTR;
                break;
            case EINVAL:
                result = NAL_EINVAL;
                break;
            case EIO:
                result = NAL_EIO;
                break;
            case EISDIR:
            default:
                result = NAL_EINTERNAL;
                break;
            }
        }
    }
    return result;
}

/**
 * \brief Secured implementation of classic write() function
 * This function can write data to files or resource
 * @param[in] fd File descriptor
 * @param{in] buf Data to write
 * @param[in, out] count Size of data to write, and the number of data written
 * @return \n
 * NAL_SUCCESS: function correctly executed.
 * NAL_EINVAL: Invalid parameter
 * NAL_EAGAIN: Unlock written but file descriptor set in lock mode
 * NAL_EBADF: invalid file descriptor, or resource not open
 * NAL_EFAULT: bad pointer of buf
 * NAL_EINTR: function interrupted before the written finished
 * NAL_EIO: input/output error
 * NAL_ENOSPC: the resource have no space 
 * NAL_EINTERNAL: other error
 */
int nal_objectWrite(int fd, const void *buf, size_t *count)
{
    int result = NAL_SUCCESS;
    int writeRes = 0;

    /* Sanity check */
    if (buf == NULL || count == NULL)
    {
        result = NAL_EINVAL;
    }
    else
    {
        *count = write(fd, buf, *count);
        if (*count < 0)
        {
            writeRes = errno;
            switch (writeRes)
            {
            case EAGAIN:
                result = NAL_EAGAIN;
                break;
            case EBADF:
                result = NAL_EBADF;
                break;
            case EFAULT:
                result = NAL_EFAULT;
                break;
            case EINTR:
                result = NAL_EINTR;
                break;
            case EINVAL:
                result = NAL_EINVAL;
                break;
            case EIO:
                result = NAL_EIO;
                break;
            case ENOSPC:
                result = NAL_ENOSPC;
                break;
            case EPIPE:
            case EFBIG:
            default:
                result = NAL_EINTERNAL;
                break;
            }
        }
    }
    return result;
}

/**
 * @todo Must change for variable parameter in future
 * \brief Secured implementation of classical ioctl() function
 * @param[in] fd File descriptor 
 * @param[in] req Request type
 * @param[in] addr Address
 * @return \n
 *      NAL_SUCCESS: function correctly executed.
 *      NAL_EINVAL: invalid argument.
 *      NAL_ERROR: other error.
 */
int nal_objectIoctl(int fd, unsigned long cmd, int addr)
{
    int result = NAL_SUCCESS;
    int ioctlRes = 0;

    ioctlRes = ioctl(fd, cmd, addr);
    if (ioctlRes < 0)
    {
        ioctlRes = errno;
        switch (ioctlRes)
        {
        case EBADF:
            result = NAL_ERROR;
            break;
        case EFAULT:
            result = NAL_ERROR;
            break;
        case EINVAL:
            result = NAL_EINVAL;
            break;
        case ENOTTY:
            result = NAL_ERROR;
            break;
        default:
            result = NAL_ERROR;
            break;
        }
    }
    return result;
}

#endif /* NO_NAL_LIBC_POSIX */