
/*------------------------------ Include files -------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "com_error.h"
#include "com_entity.h"

#include "nal_mem.h"

#include "tools_trace.h"
#include "tools_strManip.h"
#include "tools_parserJson.h"
#include "tools_parserJsonTools.h"
#include "tools_fileManip.h"

#include "entmgt_launcher.h"
#include "entmgt_utils.h"

/*------------------------------ Pragma --------------------------------------*/
/*------------------------------ Macros --------------------------------------*/
/*------------------------------ Structures ----------------------------------*/
/*------------------------------ Constants -----------------------------------*/
/*------------------------------ Variables -----------------------------------*/
/*-----------------------------  Private Functions ---------------------------*/

/**
 * \brief Get all data from an array type
 * @warning This function manage array only if the element in the array are the same type
 * @param valueArray
 * @param dataType
 * @param dataValue
 */
static void entmgt_convertRawConfigurationDataToBusinessData(const T_tools_json_value * rawConfigurationData, T_com_entity_setting * setting)
{
    size_t length = 0u;

    switch (rawConfigurationData->type)
    {
    case json_integer:
        setting->value = rawConfigurationData->u.integer;   /* @todo need to fix it! */
        break;
    case json_boolean:
        setting->value = rawConfigurationData->u.boolean;   /* @todo need to fix it! */
        break;
    case json_string:
        length = strlen(rawConfigurationData->u.string.ptr);
        setting->value = NAL_MALLOC(length * sizeof (char) + 1);
        memset(setting->value, 0, length * sizeof (char) + 1);
        memcpy(setting->value, rawConfigurationData->u.string.ptr, length * sizeof (char));
        break;
    case json_object:
        length = strlen(rawConfigurationData->u.object.values[rawConfigurationData->u.object.length - 1].name);
        setting->key = NAL_MALLOC(length * sizeof (char) + 1);
        memset(setting->key, 0, length * sizeof (char) + 1);
        memcpy(setting->key, rawConfigurationData->u.object.values[rawConfigurationData->u.object.length - 1].name, length * sizeof (char));
        /* Again! */
        entmgt_convertRawConfigurationDataToBusinessData(rawConfigurationData->u.object.values[rawConfigurationData->u.object.length - 1].value, setting);
        break;
    default:
        TRACE_GEN(ERROR, "You should not be here!", __FUNCTION__, __LINE__);
        break;
    }
    return;
}

/*-----------------------------  Public Functions ----------------------------*/

/**
 * \brief Read an internal file
 * @param configurationFilePath
 * @param buffer
 * @param bufferSize
 * @return 
 */
T_com_status entmgt_readInternalFile(const char *internalFilePath, char **buffer, size_t *bufferSize)
{
    T_com_status returnCode = COM_SUCCESS;
    uint8_t * byte = NULL;
    size_t byteSize = 0u;

    START_FUNCTION();

    /* Sanity check */
    VERIF_ARGS("internalFilePath", internalFilePath != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("buffer", buffer != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("*buffer", *buffer == NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("bufferSize", bufferSize != NULL, returnCode, COM_ERR_PARAM);
    if (returnCode != COM_SUCCESS)
    {
        TRACE_GEN(ERROR, "Invalid parameter", __FUNCTION__, __LINE__);
        goto exit;
    }

    if (tools_readFile(internalFilePath, &byte, &byteSize) != TOOLS_SUCCESS)
    {
        TRACE_GEN(WARN, "Reading configuration file failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_FILE;
        goto exit;
    }

    /* Copy data */
    *buffer = (char*) NAL_MALLOC(byteSize * sizeof (char));
    if (*buffer == NULL)
    {
        TRACE_GEN(ERROR, "Memory allocation error for configuration data", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_NO_MEMORY;
        goto exit;
    }
    memset(*buffer, 0, byteSize * sizeof (char));
    memcpy(*buffer, byte, byteSize * sizeof (char));

    *bufferSize = byteSize;

exit:

    if (byte != NULL)
    {
        NAL_FREE(byte);
        byte = NULL;
    }

    END_FUNCTION();
    return returnCode;

}

/**
 * Write an internal file
 * @param internalFilePath
 * @param buffer
 * @param bufferSize
 * @return 
 */
T_com_status entmgt_writeInternalFile(const char *internalFilePath, const char *buffer, const size_t bufferSize)
{
    T_com_status returnCode = COM_SUCCESS;
    size_t writeSize = 0u;

    START_FUNCTION();

    /* Sanity check */
    VERIF_ARGS("internalFilePath", internalFilePath != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("buffer", buffer != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("bufferSize", bufferSize > 0, returnCode, COM_ERR_PARAM);
    if (returnCode != COM_SUCCESS)
    {
        TRACE_GEN(ERROR, "Invalid parameter", __FUNCTION__, __LINE__);
        goto exit;
    }

    writeSize = bufferSize;
    if (tools_writeFile(internalFilePath, (uint8_t *)buffer, &writeSize) != TOOLS_SUCCESS)
    {
        TRACE_GEN(WARN, "Writing configuration file failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_FILE;
        goto exit;
    }

    if (bufferSize != writeSize)
    {
        TRACE_GEN(WARN, "Writing file size different than buffer size", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_FILE;
        goto exit;
    }

exit:

    END_FUNCTION();
    return returnCode;
}

/**
 * \brief Parse an internal JSON buffer
 * @param buffer
 * @param bufferSize
 * @param rawJsonData
 * @return 
 */
T_com_status entmgt_parseInternalJsonBuffer(const char *buffer, size_t bufferSize, T_tools_json_value **rawJsonData)
{
    T_com_status returnCode = COM_SUCCESS;

    START_FUNCTION();

    /* Sanity check */
    VERIF_ARGS("buffer", buffer != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("bufferSize", bufferSize > 0, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("rawJsonData", rawJsonData != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("*rawJsonData", *rawJsonData == NULL, returnCode, COM_ERR_PARAM);
    if (returnCode != COM_SUCCESS)
    {
        TRACE_GEN(ERROR, "Invalid parameter", __FUNCTION__, __LINE__);
        goto exit;
    }

    *rawJsonData = tools_json_parse(buffer, bufferSize);
    if (*rawJsonData == NULL)
    {
        TRACE_GEN(ERROR, "Parsing buffer failed", __FUNCTION__, __LINE__);
        goto exit;
    }

exit:

    END_FUNCTION();
    return returnCode;
}

/**
 * \brief Get settings from a raw configuration data 
 * @param[in] rawConfigurationData Raw parsing data
 * @param[in] namespace Namespace of the entity
 * @param[out] settings  Settings of the entity (allocated)
 * @param[out] nbSettings Number of settings in the structure settings
 * @warning The caller must call al_free() to deallocate memory of the parameter settings
 * @return 
 */
T_com_status entmgt_getSettingsFromRawConfigurationData(const T_tools_json_value * rawConfigurationData, const char * namespace, T_com_entity_setting ** settings, size_t *nbSettings)
{
    T_com_status returnCode = COM_SUCCESS;

    T_tools_json_type dataType = json_null;
    T_tools_union_data dataValue = {0};

    int x = 0;
    int y = 0;

    size_t length = 0u;

    START_FUNCTION();

    /* Sanity check */
    VERIF_ARGS("rawConfigurationData", rawConfigurationData != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("namespace", namespace != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("settings", settings != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("*settings", *settings == NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("nbSettings", nbSettings != NULL, returnCode, COM_ERR_PARAM);
    if (returnCode != COM_SUCCESS)
    {
        TRACE_GEN(ERROR, "Invalid parameter", __FUNCTION__, __LINE__);
        goto exit;
    }

    tools_json_grab_data(rawConfigurationData, "version", &dataType, &dataValue);

    /* @todo manage version of file */
    if (dataType != json_integer)
    {
        if (dataType == json_null)
        {
            TRACE_GEN(ERROR, "Apparently this file is not the good one! Version not found", __FUNCTION__, __LINE__);
        }
        else
        {
            TRACE_GEN(ERROR, "Unexpected type", __FUNCTION__, __LINE__);
        }

        returnCode = COM_ERR_CONFIG;
        goto exit;
    }

    if (dataValue.integer < 0)
    {
        TRACE_GEN(ERROR, "Bad version!", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_INTERNAL;
        goto exit;
    }

    /* Clean */
    dataType = json_null;
    memset(&dataValue, 0, sizeof (T_tools_union_data));

    tools_json_grab_data(rawConfigurationData, namespace, &dataType, &dataValue);

    if (dataType != json_object)
    {
        if (dataType == json_null)
        {
            TRACE_GEN(WARN, "This file have no settings for this entity namespace", __FUNCTION__, __LINE__);
        }
        else
        {
            TRACE_GEN(ERROR, "Unexpected type", __FUNCTION__, __LINE__);
            returnCode = COM_ERR_INTERNAL;
        }
        goto exit;
    }

    if (dataValue.object.length > 1)
    {
        TRACE_GEN(ERROR, "Unexpected number of element", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_INTERNAL;
        goto exit;
    }

    /* For each object get settings (Normally only one object expected) */
    for (x = 0; x != dataValue.object.length; x++)
    {
        if ((strcmp(dataValue.object.values[x].name, "settings") == 0) &&
                (dataValue.object.values[x].value->type == json_array))
        {
            length = dataValue.object.values[x].value->u.array.length;

            /* Allocate setting structure */
            (*settings) = NAL_MALLOC(length * sizeof (T_com_entity_setting));
            if ((*settings) == NULL)
            {
                TRACE_GEN(ERROR, "Memory allocation error", __FUNCTION__, __LINE__);
                returnCode = COM_ERR_NO_MEMORY;
                goto exit;
            }
            memset((*settings), 0, length * sizeof (T_com_entity_setting));

            *nbSettings = length;

            /* Manage each settings for the current namespace */
            for (y = 0; y < length; y++)
                entmgt_convertRawConfigurationDataToBusinessData(dataValue.object.values[x].value->u.array.values[y], &(*settings)[y]);
        }
        else
        {
            TRACE_GEN(ERROR, "Unexpected element", __FUNCTION__, __LINE__);
            returnCode = COM_ERR_INTERNAL;
            goto exit;
        }
    }

exit:

    END_FUNCTION();
    return returnCode;
}

/**
 * @brief This function returns the running security model file path
 * @warning The function allocate filePath. Release memory must be done by the caller
 * @param[out] filePath Will contains the full file path to the security model running
 * @return A T_com_status error code
 */
T_com_status entmgt_getRunningSecurityModelFilePath(char **filePath)
{
    T_com_status returnCode = COM_SUCCESS;

    START_FUNCTION();

    /* Sanity check */
    VERIF_ARGS("filePath", filePath != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("*filePath", *filePath == NULL, returnCode, COM_ERR_PARAM);
    if (returnCode != COM_SUCCESS)
    {
        TRACE_GEN(ERROR, "Invalid parameter", __FUNCTION__, __LINE__);
        goto exit;
    }
    
    if (tools_buildFilePath(entmgt_launcher_getAppRootPath(), ENTMGT_DIR_RUNNING_RBAC, ENTMGT_FILE_RUNNING_RBAC, filePath) != TOOLS_SUCCESS)
    {
        TRACE_GEN(ERROR, "Building security model running file path failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_INTERNAL;
        goto exit;
    }
    
    if (COM_SUCCESS != returnCode && NULL != (*filePath))
    {
        free(*filePath); 
        *filePath = NULL;
    }

exit:
    END_FUNCTION();
    return returnCode;
}

/**
 * \brief This function returns the factory model file path
 * \warning The function allocate filePath. Release memory must be done by the caller
 * \param[out] filePath Will contains the full file path to the security model factory
 * \return A T_com_status error code
 */
T_com_status entmgt_getFactorySecurityModelFilePath(char **filePath)
{
    T_com_status returnCode = COM_SUCCESS;

    START_FUNCTION();

    /* Sanity check */
    VERIF_ARGS("filePath", filePath != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("*filePath", *filePath == NULL, returnCode, COM_ERR_PARAM);
    if (returnCode != COM_SUCCESS)
    {
        TRACE_GEN(ERROR, "Invalid parameter", __FUNCTION__, __LINE__);
        goto exit;
    }

    if (tools_buildFilePath(entmgt_launcher_getAppRootPath(), ENTMGT_DIR_FACTORY_RBAC, ENTMGT_FILE_FACTORY_RBAC, filePath) != TOOLS_SUCCESS)
    {
        TRACE_GEN(ERROR, "Building security model factory file path failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_INTERNAL;
        goto exit;
    }
    
    if (COM_SUCCESS != returnCode && NULL != (*filePath))
    {
        free(*filePath); 
        *filePath = NULL;
    }

exit:
    END_FUNCTION();
    return returnCode;
}