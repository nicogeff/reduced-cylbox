
/*------------------------------ Include files -------------------------------*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "com_error.h"
#include "com_entity.h"

#include "com_cryptolib_header.h"

#include "tools_trace.h"
#include "tools_parserJson.h"
#include "tools_strManip.h"
#include "tools_fileManip.h"

#include "nal_libc.h"
#include "nal_stor.h"

#include "cryptoal_mgt.h"
/* TODO should be in common/inc and agnostic */
#include "cryptoal_certificate.h"

#include "entmgt_utils.h"
#include "entmgt_target.h"
#include "entmgt_initialize.h"
#include "entmgt_launcher.h"

#include "entmgt_factory_file_conf.h"
#include "entmgt_factory_file_rbac.h"

/*------------------------------ Pragma --------------------------------------*/
/*------------------------------ Macros --------------------------------------*/
/*------------------------------ Structures ----------------------------------*/
/*------------------------------ Constants -----------------------------------*/

/**
 * \brief List of all entities that should be triggered by this launcher
 * \Warning The function entmgt_initializeHimself must be also updated!
 */
#ifdef AUTH_RBAC_ENTITY
extern T_com_entity_launch authRbacLauncher;
#endif
#ifdef WEBSERVER_ENTITY
extern T_com_entity_launch webserverLauncher;
#endif

/*------------------------------ Variables -----------------------------------*/

/*-----------------------------  Private Functions ---------------------------*/

static T_com_status entmgt_buildFolderArboFromRootPath (const char * folderPath)
{
    T_com_status returnCode = COM_SUCCESS;
    const char * rootPath = NULL;
    char * outFolderPath = NULL;

    START_FUNCTION();
    
    /* Get root path */
    rootPath = entmgt_launcher_getAppRootPath();
    
    /* Build the outFolderPath (= concatenation of rootPath + folderPath) */
    if (tools_buildFolderPath(rootPath, folderPath, &outFolderPath) != TOOLS_SUCCESS)
    {
        TRACE_GEN(ERROR, "Building folder path failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_NO_MEMORY;
        goto exit; 
    }
    
    /* Check if outFolderPath exist */
    if (nal_isDirExist(outFolderPath) != NAL_SUCCESS)
    {
        /* Non existing folder! Trying to create the outFolderPath */
        if (nal_dirCreate(outFolderPath, 0770) != NAL_SUCCESS)
        {
            returnCode = COM_ERR_NOT_ALLOWED;
            goto exit;
        }
    }
    
exit:
    
    if (outFolderPath != NULL) { free(outFolderPath); outFolderPath = NULL; }
    
    END_FUNCTION();
    return returnCode;
}

/* Treat 1 between 2 possible files: CONF or RBAC in concerned macros of names */
static T_com_status entmgt_buildFileArboFromRootPath (const char * folderPath, const char * filename)
{
    T_com_status returnCode = COM_SUCCESS;

    const char * rootPath = NULL;
    char * outFolderPath = NULL;
    char * outFilePath = NULL;
    size_t streamSize = 0u;

    START_FUNCTION();
    
    /* Get root path */
    rootPath = entmgt_launcher_getAppRootPath();
    
    /* Build the outFolderPath (= concatenation of rootPath + folderPath) */
    if (tools_buildFolderPath(rootPath, folderPath, &outFolderPath) != TOOLS_SUCCESS)
    {
        TRACE_GEN(ERROR, "Building folder path failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_NO_MEMORY;
        goto exit; 
    }
    
    /* Check if outFolderPath exist */
    if (nal_isDirExist(outFolderPath) != NAL_SUCCESS)
    {
        TRACE_GEN(ERROR, "Non existing folder, the file can be checked or created", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_NO_MEMORY;
        goto exit; 
    }
    
    /* Build the outFilePath */
    if (tools_buildFilePath(outFolderPath, NULL, filename, &outFilePath) != TOOLS_SUCCESS)
    {
        TRACE_GEN(ERROR, "Building file path failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_NO_MEMORY;
        goto exit;     
    }
    
    /* Check if file exist */
    if (tools_isFileExist(outFilePath) != TOOLS_TRUE)
    {
        /* Create the file for 2 cases only: CONF or RBAC. Returns an error for other files like certificates */
        if ( 0 == strncmp(filename, ENTMGT_FILE_FACTORY_CONF,  sizeof(ENTMGT_FILE_FACTORY_CONF) ) )
        {
            streamSize = strlen(g_factory_file_conf);
            if (TOOLS_SUCCESS != tools_writeFile(outFilePath, (uint8_t *)g_factory_file_conf, &streamSize))
            {
                TRACE_GEN(ERROR, "Creating Conf file failed", __FUNCTION__, __LINE__);
                returnCode = COM_ERR_NO_MEMORY;
                goto exit;
            }
        }
        else if ( 0 == strncmp(filename, ENTMGT_FILE_FACTORY_RBAC,  sizeof(ENTMGT_FILE_FACTORY_RBAC) ) )
        {
            streamSize = strlen(g_factory_file_rbac);
            if (TOOLS_SUCCESS != tools_writeFile(outFilePath, (uint8_t *)g_factory_file_rbac, &streamSize))
            {
                TRACE_GEN(ERROR, "Creating Rbac file failed", __FUNCTION__, __LINE__);
                returnCode = COM_ERR_NO_MEMORY;
                goto exit;
            }
        }
        else
        {
            TRACE_GEN(TRACE, "FIle of other type is missing (keypair or certificate for example)", __FUNCTION__, __LINE__);
            returnCode = COM_ERR_NO_MEMORY;
            goto exit;
        }
          
    }
    
exit:
    
    if (outFolderPath != NULL) { free(outFolderPath); outFolderPath = NULL; }
    if (outFilePath != NULL) { free(outFilePath); outFilePath = NULL; }

    END_FUNCTION();
    return returnCode;
}

/** 
 * \brief This function initialize the entity management him self 
 */
static T_com_status entmgt_initializeHimself(
                                              T_com_entity_launch * launcherList[],
                                              size_t * launcherListSize)
{
    T_com_status returnCode = COM_SUCCESS;

    char * rootPath = NULL;
    char * factoryPath = NULL;
    bool is_ca_correct = true;
    
    START_FUNCTION();
    
    /* Set root folder with default path if not already set */
    if (entmgt_launcher_getAppRootPath() == NULL)
    {
        TRACE_GEN(TRACE, "Setting root path with default value...", __FUNCTION__, __LINE__);
        
        /** 
         * Build a full root path according the environment variable "HOME"
         * 
         * \note In the future this value (working root space, will be give by 
         * the local configuration (if the value is not set we can continue to
         * use HOME)
         */
        if (tools_buildFolderPath(getenv("HOME"), ENTMGT_DIR_ROOT_PATH_NAME, &rootPath) != TOOLS_SUCCESS)
        {
            returnCode = COM_ERR_NOT_ALLOWED;
            goto exit;
        }
        
        /* Check if the full root path exist */
        if(nal_isDirExist(rootPath) != NAL_SUCCESS)
        {
            /* Trying to create the root path */
            if(nal_dirCreate(rootPath, 0770) != NAL_SUCCESS)
            {
                returnCode = COM_ERR_NOT_ALLOWED;
                goto exit;
            }
        }
        
        /* Set internal root path */
        (void)entmgt_launcher_setAppRootPath (rootPath);
    }

    /** 
     * Build all needed folders (if needed) to run properly the application
     */
    entmgt_buildFolderArboFromRootPath(ENTMGT_DIR_FACTORY_CONF);
    entmgt_buildFolderArboFromRootPath(ENTMGT_DIR_FACTORY_RBAC);
    entmgt_buildFolderArboFromRootPath(ENTMGT_DIR_RUNNING_CONF);
    entmgt_buildFolderArboFromRootPath(ENTMGT_DIR_RUNNING_RBAC);
    entmgt_buildFolderArboFromRootPath(ENTMGT_DIR_WWW);

    entmgt_buildFolderArboFromRootPath(ENTMGT_DIR_CERTIFICATE_CAROOT);
    entmgt_buildFolderArboFromRootPath(ENTMGT_DIR_CERTIFICATE_CLOUD);
    entmgt_buildFolderArboFromRootPath(ENTMGT_DIR_CERTIFICATE_TOOL);
    entmgt_buildFolderArboFromRootPath(ENTMGT_DIR_CERTIFICATE_LOCAL);

    entmgt_buildFileArboFromRootPath(ENTMGT_DIR_FACTORY_CONF, ENTMGT_FILE_FACTORY_CONF);
    entmgt_buildFileArboFromRootPath(ENTMGT_DIR_FACTORY_RBAC, ENTMGT_FILE_FACTORY_RBAC);
    entmgt_buildFileArboFromRootPath(ENTMGT_DIR_RUNNING_CONF, ENTMGT_FILE_RUNNING_CONF);
    entmgt_buildFileArboFromRootPath(ENTMGT_DIR_RUNNING_RBAC, ENTMGT_FILE_RUNNING_RBAC);

    /* Verify CA chain, cryptoal knows its structure */
    returnCode = cryptoal_check_ca_chain_certificates ( & is_ca_correct );
    if (COM_SUCCESS != returnCode)
    {
        TRACE_GEN(WARN, "Unable to evaluate certificates chain of confidence", __FUNCTION__, __LINE__);
        is_ca_correct = false;
        goto exit;
    }
    if (false == is_ca_correct)
    {
        /* Run CA in degraded mode, clean and regenerate an autosigned local certificate and keypair */
        TRACE_GEN(TRACE, "CA is regenerated in degraded mode", __FUNCTION__, __LINE__);
        cryptoal_ca_chain_clean_certificates ();
        returnCode = cryptoal_generate_autosigned_local_certificate_and_keys ();
        if (COM_SUCCESS != returnCode)
        {
            TRACE_GEN(ERROR, "Unable to generate an autosigned certificate/keypair", __FUNCTION__, __LINE__);
            goto exit;
        }
    }


    /* Build the list of registered entity */
#ifdef AUTH_RBAC_ENTITY
    launcherList[*launcherListSize] = &authRbacLauncher;
    (*launcherListSize)++;
#endif
#ifdef WEBSERVER_ENTITY
    launcherList[*launcherListSize] = &webserverLauncher;
    (*launcherListSize)++;
#endif

exit:
    
    if (rootPath != NULL) { free(rootPath); rootPath = NULL; }
    if (factoryPath != NULL) { free(factoryPath); factoryPath = NULL; }

    END_FUNCTION();
    return returnCode;
}

/**
 * \brief Initialize other entity
 */
static T_com_status entmgt_initializeEntities(
                                              T_com_entity_launch * launcherList[],
                                              size_t launcherListSize)
{
    size_t i = 0u;
    T_com_status returnCode = COM_SUCCESS;
    char errMsg [ENTMGT_STRING_SIZE] = {0};
    size_t wSize = 0u;

    START_FUNCTION();

    for (i = 0u; i < launcherListSize; i++)
    {
        if (launcherList[ i ]->initializeCallback != NULL)
        {
            returnCode = launcherList[ i ]->initializeCallback();
            if (returnCode != COM_SUCCESS)
            {
                nal_snprintf_s(errMsg, ENTMGT_STRING_SIZE, ENTMGT_STRING_SIZE, &wSize, "Could not initialize the %s", launcherList[ i ]->nameSpace);
                TRACE_GEN(ERROR, errMsg, __FUNCTION__, __LINE__);
                break;
            }
        }
    }

    END_FUNCTION();
    return returnCode;
}

/*-----------------------------  Public Functions ----------------------------*/

/**
 * \brief Initialization function
 * @detail This function initialize the entity management first, then 
 * all entities registered
 * @param launcherList
 * @param launcherListSize
 * @return 
 */
T_com_status entmgt_initialize(T_com_entity_launch * launcherList[], size_t * launcherListSize)
{
    T_com_status returnCode = COM_SUCCESS;

    START_FUNCTION();

    /* Sanity check */
    VERIF_ARGS("launcherList[]", launcherList != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("launcherListSize", launcherListSize != NULL, returnCode, COM_ERR_PARAM);
    if (returnCode != COM_SUCCESS)
    {
        TRACE_GEN(TRACE, "Invalid parameter", __FUNCTION__, __LINE__);
        goto exit;
    }

    /* Initialize  "crypto_al " sub-entity and its library  (No usual cycle, no NameSpace and no settings) */
    returnCode = cryptoal_initialize  ();
    if (returnCode != COM_SUCCESS)
    {
        TRACE_GEN(ERROR, "Starting sub-module crypto_al failed", __FUNCTION__, __LINE__);
        goto exit;
    }

    /* Initialize entity management himself */
    returnCode = entmgt_initializeHimself(launcherList, launcherListSize)/*, rootPath, scriptPath)*/;
    if (returnCode != COM_SUCCESS)
    {
        TRACE_GEN(ERROR, "Initialization of Entity Management failed", __FUNCTION__, __LINE__);
        goto exit;
    }

    /* Initialize all entity */
    returnCode = entmgt_initializeEntities(launcherList, *launcherListSize);
    if (returnCode != COM_SUCCESS)
    {
        TRACE_GEN(ERROR, "Initialization of other entities failed", __FUNCTION__, __LINE__);
        goto exit;
    }

exit:

    END_FUNCTION();
    return returnCode;
}

