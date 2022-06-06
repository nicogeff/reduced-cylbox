
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "nal_stor.h"
#include "nal_stor_stdio.h"
#include "nal_mem.h"

#include "tools_trace.h"
#include "tools_type.h"
#include "tools_strManip.h"
#include "tools_fileManip.h"

/*-----------------------------  File Management -----------------------------*/

/**
 * @brief This function check if the file filename exist
 * @param[in] filename The file to check
 * @return \n
 * TOOLS_TRUE: If the file exist
 * TOOLS_FALSE: If the file non-exist
 */
T_tools_boolean tools_isFileExist(const char * filename)
{
    T_tools_status res = TOOLS_SUCCESS;
    T_tools_boolean bRes = TOOLS_FALSE;
    T_nal_stream stream = {0};

    START_FUNCTION();

    /* Sanity check */
    VERIF_ARGS("filename", filename != NULL, res, TOOLS_INVALID_PARAM);
    if (res != TOOLS_SUCCESS)
    {
        TRACE_GEN(TRACE, "Invalid parameter", __FUNCTION__, __LINE__);
        goto exit;
    }

    /* Initialize stream */
    if (nal_streamInit(&stream, filename, NAL_STREAM_READ) != NAL_SUCCESS)
        goto exit;
        
    bRes = TOOLS_TRUE;

exit:
    
    /* Close file */
    (void) nal_streamClose(&stream);
    
    END_FUNCTION();
    return bRes;
}

/**
 * @brief This function read a file from parameter filename and return 
 * the data through the parameter byte
 * @warning The caller must release memory byte by using AL_FREE()
 * @param[in] filename The file to read
 * @param[out] byte The read data
 * @param[out] length The size of read data
 * @return \n
 * TOOLS_SUCCESS: On success
 * TOOLS_FILE_ERROR 
 * TOOLS_MEM_ERROR
 * TOOLS_ERROR
 */
T_tools_status tools_readFile(const char* filename, uint8_t ** byte, size_t * length)
{
    T_tools_status res = TOOLS_SUCCESS;
    T_nal_stream stream = {0};
    size_t byteRead = 0u;

    START_FUNCTION();

    /* Sanity check */
    VERIF_ARGS("filename", filename != NULL, res, TOOLS_INVALID_PARAM);
    VERIF_ARGS("byte", byte != NULL, res, TOOLS_INVALID_PARAM);
    VERIF_ARGS("*byte", *byte == NULL, res, TOOLS_INVALID_PARAM);
    VERIF_ARGS("length", length != NULL, res, TOOLS_INVALID_PARAM);
    if (res != TOOLS_SUCCESS)
    {
        TRACE_GEN(TRACE, "Invalid parameter", __FUNCTION__, __LINE__);
        goto exit;
    }

    /* Initialize stream */
    if (nal_streamInit(&stream, filename, NAL_STREAM_READ) != NAL_SUCCESS)
    {
        TRACE_GEN(ERROR, "Stream initialization failed", __FUNCTION__, __LINE__);
        res = TOOLS_FILE_ERROR;
        goto exit;
    }

    /* Get file size */
    if (nal_streamGetFileSize(filename, length) != NAL_SUCCESS)
    {
        TRACE_GEN(ERROR, "Getting file size failed", __FUNCTION__, __LINE__);
        res = TOOLS_FILE_ERROR;
        goto exit;
    }

    /* Allocate memory before reading the file */
    *byte = NAL_MALLOC((*length) * sizeof (uint8_t));
    if (*byte == NULL)
    {
        TRACE_GEN(ERROR, "Memory allocation error", __FUNCTION__, __LINE__);
        res = TOOLS_MEM_ERROR;
        goto exit;
    }
    memset(*byte, 0, (*length) * sizeof (uint8_t));

    /* Size to read */
    byteRead = *length;

    /* Read file */
    if (nal_streamRead(&stream, *byte, &byteRead) != NAL_SUCCESS)
    {
        TRACE_GEN(ERROR, "Reading stream failed", __FUNCTION__, __LINE__);
        res = TOOLS_FILE_ERROR;
        goto exit;
    }

    /* Compare the file size and the reading size */
    if (byteRead != *length)
    {
        TRACE_GEN(ERROR, "File size and reading size is different", __FUNCTION__, __LINE__);
        res = TOOLS_ERROR;
        goto exit;
    }

exit:

    /* Close file */
    (void) nal_streamClose(&stream);

    END_FUNCTION();
    return res;
}

/**
 * @brief This function write (override mode) data byte into file filename
 * @param[in] filename The file to write
 * @param[in] byte The data to write
 * @param[in/out] length The size of data to write. Return the size of data write 
 * @return \n
 * TOOLS_SUCCESS: On success
 * TOOLS_FILE_ERROR 
 * TOOLS_MEM_ERROR
 * TOOLS_ERROR
 */
T_tools_status tools_writeFile(const char * filename, uint8_t * byte, size_t * length)
{
    T_tools_status res = TOOLS_SUCCESS;
    T_nal_stream stream = {0};
    unsigned long lengthBeforeWrite = 0u;
    unsigned long lengthAfterWrite = 0u;

    START_FUNCTION();

    /* Sanity check */
    VERIF_ARGS("filename", filename != NULL, res, TOOLS_INVALID_PARAM);
    VERIF_ARGS("byte", byte != NULL, res, TOOLS_INVALID_PARAM);
    VERIF_ARGS("length", length != NULL, res, TOOLS_INVALID_PARAM);
    if (res != TOOLS_SUCCESS)
    {
        TRACE_GEN(TRACE, "Invalid parameter", __FUNCTION__, __LINE__);
        goto exit;
    }

    /* Initialize stream */
    if (nal_streamInit(&stream, filename, NAL_STREAM_WRITE_OVERWRITE) != NAL_SUCCESS)
    {
        TRACE_GEN(ERROR, "Stream initialization failed", __FUNCTION__, __LINE__);
        res = TOOLS_FILE_ERROR;
        goto exit;
    }
    
    /* Kept size before write action */
    lengthBeforeWrite = *length;
    
    /* Write stream */
    if (nal_streamWrite(&stream, byte, (unsigned long *)length) != NAL_SUCCESS)
    {
        TRACE_GEN(ERROR, "Write stream failed", __FUNCTION__, __LINE__);
        res = TOOLS_FILE_ERROR;
        goto exit;
    }
    
    /* Kept size after write action */
    lengthAfterWrite = *length;
    
    if (lengthBeforeWrite != lengthAfterWrite)
    {
        TRACE_GEN(ERROR, "Difference size between before and after write action", __FUNCTION__, __LINE__);
    }
    
exit:
    
    /* Close file */
    (void) nal_streamClose(&stream);
    
    END_FUNCTION();
    return res;
}

/** 
 * @brief This function provide a way to copy a file
 * @detail The function open the source file srcFilename, read it, save data into
 * a local memory, then open the destination file in write mode (if the 
 * destination file is already present he is erased), write the local memory.
 * @param[in] srcFilename The source file name
 * @param[in] dstFilename The destination file name
 * @return \n
 * TOOLS_SUCCESS: On success \n
 * TOOLS_FILE_ERROR 
 * TOOLS_MEM_ERROR
 * TOOLS_ERROR
 */
T_tools_status tools_copyFile(const char* srcFilename, const char* dstFilename)
{
    T_tools_status res = TOOLS_SUCCESS;
    
    uint8_t * byte = NULL;
    size_t length = 0u;
    
    START_FUNCTION();

    /* Sanity check */
    VERIF_ARGS("srcFilename", srcFilename != NULL, res, TOOLS_INVALID_PARAM);
    VERIF_ARGS("dstFilename", dstFilename != NULL, res, TOOLS_INVALID_PARAM);
    if (res != TOOLS_SUCCESS)
    {
        TRACE_GEN(TRACE, "Invalid parameter", __FUNCTION__, __LINE__);
        goto exit;
    }
    
    /* Check if source file exist */
    if (tools_isFileExist(srcFilename) == TOOLS_TRUE)
    {
        /* Read source file */
        res = tools_readFile(srcFilename, &byte, &length);
        if (res != TOOLS_SUCCESS)
        {
            goto exit;
        }
        
        /* Write destination file */
        res = tools_writeFile(dstFilename, byte, &length);
        if (res != TOOLS_SUCCESS)
        {
            goto exit;
        }
    }
    else
    {
        TRACE_GEN(TRACE, "File not found", __FUNCTION__, __LINE__);
        res = TOOLS_FILE_ERROR;
    }
    
exit:
    
    END_FUNCTION();
    return res;
}

/*-----------------------------  Path Management -----------------------------*/

/**
 * 
 * @param dir
 * @return 
 */
T_tools_status tools_getWorkingDirectory(char ** dir)
{
    T_tools_status res = TOOLS_SUCCESS;
    char cwd[PATH_MAX] = {0};

    START_FUNCTION();

    /* Sanity check */
    VERIF_ARGS("dir", dir != NULL, res, TOOLS_INVALID_PARAM);
    VERIF_ARGS("*dir", *dir == NULL, res, TOOLS_INVALID_PARAM);
    if (res != TOOLS_SUCCESS)
    {
        TRACE_GEN(TRACE, "Invalid parameter", __FUNCTION__, __LINE__);
        goto exit;
    }

    getcwd(cwd, sizeof (cwd));

    (*dir) = NAL_MALLOC(strlen(cwd) * sizeof (char) + 1);
    if (*dir == NULL)
    {
        TRACE_GEN(TRACE, "Invalid parameter", __FUNCTION__, __LINE__);
        res = TOOLS_MEM_ERROR;
        goto exit;
    }
    memset(*dir, 0, strlen(cwd) * sizeof (char) + 1);
    memcpy(*dir, cwd, strlen(cwd) * sizeof (char));

exit:
    END_FUNCTION();
    return res;
}

/**
 * Build a file path
 * @remark The memory must be clean by the caller by using: free()
 * @note This function manage \\ or /
 * @param[in] firstFolderPath First part of folder path
 * @param[in] secondFolderPath Second part of folder path. This parameter can be NULL
 * that means filename will be concatenate to first path folder
 * @param[in] filename Filename to concatenate to folders
 * @param[out] filePath Result of the concatenation
 * @return An error status code.
 */
T_tools_status tools_buildFilePath(const char * firstFolderPath, const char * secondFolderPath, const char * filename, char ** filePath)
{
    T_tools_status returnCode = TOOLS_SUCCESS;
    char * lRootPath = NULL;
    char * lIntermediatePath = NULL;

    START_FUNCTION();

    /* Sanity check */
    VERIF_ARGS("rootPath", firstFolderPath != NULL, returnCode, TOOLS_INVALID_PARAM);
    VERIF_ARGS("filename", filename != NULL, returnCode, TOOLS_INVALID_PARAM);
    VERIF_ARGS("fileFullPath", filePath != NULL, returnCode, TOOLS_INVALID_PARAM);
    VERIF_ARGS("*fileFullPath", *filePath == NULL, returnCode, TOOLS_INVALID_PARAM);
    if (returnCode != TOOLS_SUCCESS)
    {
        TRACE_GEN(ERROR, "Invalid parameter", __FUNCTION__, __LINE__);
        goto exit;
    }

    /* Manage end of character of rootPath */
    if (tools_isCharacterPresentAtEnd(firstFolderPath, '\\') == TOOLS_TRUE || tools_isCharacterPresentAtEnd(firstFolderPath, '/') == TOOLS_TRUE)
    {
        if (tools_concatStrings(&lRootPath, 1, firstFolderPath) != TOOLS_SUCCESS)
        {
            *filePath = NULL;
            returnCode = TOOLS_ERROR;
            TRACE_GEN(ERROR, "Concat string failed", __FUNCTION__, __LINE__);
            goto exit;
        }
    }
    else
    {
        if (tools_concatStrings(&lRootPath, 2, firstFolderPath, "/") != TOOLS_SUCCESS)
        {
            *filePath = NULL;
            returnCode = TOOLS_ERROR;
            TRACE_GEN(ERROR, "Concat string failed", __FUNCTION__, __LINE__);
            goto exit;
        }
    }

    /* Manage end of character of intermediate*/
    if (secondFolderPath != NULL)
    {
        if (tools_isCharacterPresentAtEnd(secondFolderPath, '\\') == TOOLS_TRUE || tools_isCharacterPresentAtEnd(secondFolderPath, '/') == TOOLS_TRUE)
        {
            if (tools_concatStrings(&lIntermediatePath, 1, secondFolderPath) != TOOLS_SUCCESS)
            {
                *filePath = NULL;
                returnCode = TOOLS_ERROR;
                TRACE_GEN(ERROR, "Concat string failed", __FUNCTION__, __LINE__);
                goto exit;
            }
        }
        else
        {
            if (tools_concatStrings(&lIntermediatePath, 2, secondFolderPath, "/") != TOOLS_SUCCESS)
            {
                *filePath = NULL;
                returnCode = TOOLS_ERROR;
                TRACE_GEN(ERROR, "Concat string failed", __FUNCTION__, __LINE__);
                goto exit;
            }
        }

        if (tools_concatStrings(filePath, 3, lRootPath, lIntermediatePath, filename) != TOOLS_SUCCESS)
        {
            *filePath = NULL;
            returnCode = TOOLS_ERROR;
            TRACE_GEN(ERROR, "Concat string failed", __FUNCTION__, __LINE__);
            goto exit;
        }
    }
    else
    {
        if (tools_concatStrings(filePath, 2, lRootPath, filename) != TOOLS_SUCCESS)
        {
            *filePath = NULL;
            returnCode = TOOLS_ERROR;
            TRACE_GEN(ERROR, "Concat string failed", __FUNCTION__, __LINE__);
            goto exit;
        }
    }

exit:

    /* Free memory */
    if (lRootPath != NULL)
    {
        free(lRootPath);
        lRootPath = NULL;
    }
    if (lIntermediatePath != NULL)
    {
        free(lIntermediatePath);
        lIntermediatePath = NULL;
    }

    END_FUNCTION();
    return returnCode;
}

/**
 * Build a folder path
 * @remark The memory must be clean by the caller by using: free()
 * @note This function manage \\ or /
 * @param[in] firstFolderPath First part of folder path
 * @param[in] secondFolderPath Second part of folder path. it will be concatenate to
 * the first folder path (cant be NULL)
 * @param[out] folderPath Result of the concatenation
 * @return An error status code.
 */
T_tools_status tools_buildFolderPath(const char * firstFolderPath, const char * secondFolderPath, char ** folderPath)
{
    T_tools_status returnCode = TOOLS_SUCCESS;
    char * lFirstPartFolderPath = NULL;
    char * lsecondPartFolderPath = NULL;

    START_FUNCTION();

    /* Sanity check */
    VERIF_ARGS("firstPartFolderPath", firstFolderPath != NULL, returnCode, TOOLS_INVALID_PARAM);
    VERIF_ARGS("secondPartFolderPath", secondFolderPath != NULL, returnCode, TOOLS_INVALID_PARAM);
    VERIF_ARGS("folderPath", folderPath != NULL, returnCode, TOOLS_INVALID_PARAM);
    VERIF_ARGS("*folderPath", *folderPath == NULL, returnCode, TOOLS_INVALID_PARAM);
    if (returnCode != TOOLS_SUCCESS)
    {
        TRACE_GEN(ERROR, "Invalid parameter", __FUNCTION__, __LINE__);
        goto exit;
    }

    /* Manage end of character of lFirstPartFolderPath */
    if (tools_isCharacterPresentAtEnd(firstFolderPath, '\\') == TOOLS_TRUE || tools_isCharacterPresentAtEnd(firstFolderPath, '/') == TOOLS_TRUE)
    {
        if (tools_concatStrings(&lFirstPartFolderPath, 1, firstFolderPath) != TOOLS_SUCCESS)
        {
            *folderPath = NULL;
            returnCode = TOOLS_ERROR;
            TRACE_GEN(ERROR, "Concat string failed", __FUNCTION__, __LINE__);
            goto exit;
        }
    }
    else
    {
        if (tools_concatStrings(&lFirstPartFolderPath, 2, firstFolderPath, "/") != TOOLS_SUCCESS)
        {
            *folderPath = NULL;
            returnCode = TOOLS_ERROR;
            TRACE_GEN(ERROR, "Concat string failed", __FUNCTION__, __LINE__);
            goto exit;
        }
    }

    /* Manage end of character of lsecondPartFolderPath */

    if (tools_isCharacterPresentAtEnd(secondFolderPath, '\\') == TOOLS_TRUE || tools_isCharacterPresentAtEnd(secondFolderPath, '/') == TOOLS_TRUE)
    {
        if (tools_concatStrings(&lsecondPartFolderPath, 1, secondFolderPath) != TOOLS_SUCCESS)
        {
            *folderPath = NULL;
            returnCode = TOOLS_ERROR;
            TRACE_GEN(ERROR, "Concat string failed", __FUNCTION__, __LINE__);
            goto exit;
        }
    }
    else
    {
        if (tools_concatStrings(&lsecondPartFolderPath, 2, secondFolderPath, "/") != TOOLS_SUCCESS)
        {
            *folderPath = NULL;
            returnCode = TOOLS_ERROR;
            TRACE_GEN(ERROR, "Concat string failed", __FUNCTION__, __LINE__);
            goto exit;
        }
    }

    /* Concat first and second */
    if (tools_concatStrings(folderPath, 2, lFirstPartFolderPath, lsecondPartFolderPath) != TOOLS_SUCCESS)
    {
        *folderPath = NULL;
        returnCode = TOOLS_ERROR;
        TRACE_GEN(ERROR, "Concat string failed", __FUNCTION__, __LINE__);
        goto exit;
    }

exit:

    /* Free memory */
    if (lFirstPartFolderPath != NULL)
    {
        free(lFirstPartFolderPath);
        lFirstPartFolderPath = NULL;
    }
    if (lsecondPartFolderPath != NULL)
    {
        free(lsecondPartFolderPath);
        lsecondPartFolderPath = NULL;
    }

    END_FUNCTION();
    return returnCode;
}