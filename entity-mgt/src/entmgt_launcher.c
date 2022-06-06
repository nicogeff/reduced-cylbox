
/*------------------------------ Include files -------------------------------*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "com_error.h"
#include "com_entity.h"

#include "tools_type.h"
#include "tools_trace.h"
#include "tools_parserJsonTools.h"
#include "tools_strManip.h"

#include "entmgt_target.h"
#include "entmgt_launcher.h"
#include "entmgt_utils.h"
#include "entmgt_initialize.h"
#include "entmgt_configure.h"
#include "entmgt_start.h"
#include "entmgt_stop.h"

#include "nal_libc.h"
#include "nal_mem.h"

/*------------------------------ Pragma --------------------------------------*/
/*------------------------------ Macros --------------------------------------*/
/*------------------------------ Structures ----------------------------------*/
/*------------------------------ Constants -----------------------------------*/
/*------------------------------ Variables -----------------------------------*/

/* Status of the application */
static T_com_entity_status g_entmgt_appStatus = COM_ENTITY_IS_STOPPED;

/* List of entities registered */
static T_com_entity_launch * g_entmgt_launcherList[ENTMGT_MAX_ENTITIES] = {0};

/* Number of entities in the list */
static size_t g_entmgt_launcherListSize = 0u;

/* The PATH to the folder containing files */
static char *g_entmgt_rootPath = NULL;


/*-----------------------------  Private Functions ---------------------------*/

/**
 * \brief Get status for all entities
 */
//static T_com_status entmgt_getStatusEntities(size_t * nbEntities, T_entmgt_launcher_entity_status_info ** entities)
//{
//    size_t i = 0u;
//    T_com_status returnCode = COM_SUCCESS;
//    T_com_entity_status status = COM_ENTITY_UNKNOWN;
//    char errMsg [ENTMGT_STRING_SIZE] = {0};
//    size_t wSize = 0u;
//
//    START_FUNCTION();
//
//    /* Allocate memory */
//    *nbEntities = g_entmgt_launcherListSize;
//    *entities = (T_entmgt_launcher_entity_status_info*) malloc((*nbEntities) * sizeof (T_entmgt_launcher_entity_status_info));
//
//    for (i = 0u; i < g_entmgt_launcherListSize; i++)
//    {
//        if (g_entmgt_launcherList[ i ]->getStatusCallback != NULL)
//        {
//            returnCode = g_entmgt_launcherList[i]->getStatusCallback(&status);
//            if (returnCode != COM_SUCCESS)
//            {
//                nal_snprintf_s(errMsg, ENTMGT_STRING_SIZE, ENTMGT_STRING_SIZE, &wSize, "Could not get status of the %s entity", g_entmgt_launcherList[ i ]->nameSpace);
//                TRACE_GEN(ERROR, errMsg, __FUNCTION__, __LINE__);
//                break;
//            }
//            entities[i]->namespace = g_entmgt_launcherList[i]->nameSpace;
//            entities[i]->status = status;
//        }
//        else
//        {
//            entities[i]->namespace = g_entmgt_launcherList[i]->nameSpace;
//            entities[i]->status = COM_ENTITY_UNKNOWN;
//        }
//    }
//
//    /* In case of error, clean memory */
//    if (returnCode != COM_SUCCESS)
//    {
//        if (*entities != NULL)
//        {
//            free(*entities);
//            *entities = NULL;
//            *nbEntities = 0u;
//        }
//    }
//
//    END_FUNCTION();
//    return returnCode;
//}

/*------------------------------  Public Functions ---------------------------*/

/**
 * \brief Get the status of the application
 * @details This function returns the current status of the application
 * @return An error code
 */
T_com_status entmgt_launcher_getAppStatus(T_com_entity_status * appStatus)
{
    T_com_status returnCode = COM_SUCCESS;

    START_FUNCTION();

    *appStatus = g_entmgt_appStatus;

    END_FUNCTION();
    return returnCode;
}

/**
 * \brief Simple accessor to get the root folder path
 * @note The result must be free by the caller with NAL_FREE()!
 * @return The root folder path
 */
const char * entmgt_launcher_getAppRootPath(void)
{
    return g_entmgt_rootPath;
}

/**
 * \brief Simple accessor to set the root folder path
 * @return An error code
 */
T_com_status entmgt_launcher_setAppRootPath(const char * rootPath)
{
    T_com_status returnCode = COM_SUCCESS;

    START_FUNCTION();

    /* Sanity check */
    VERIF_ARGS("rootPath", rootPath != NULL, returnCode, COM_ERR_PARAM);
    if (returnCode != COM_SUCCESS)
    {
        TRACE_GEN(ERROR, "Invalid parameter", __FUNCTION__, __LINE__);
        goto exit;
    }

    if (g_entmgt_rootPath != NULL)
    {
        /* Clean and add the new value is it acceptable? */
        TRACE_GEN(WARN, "rootPath have already initialized with a custom value", __FUNCTION__, __LINE__);
    }
    else
    {
        if (tools_isCharacterPresentAtEnd(rootPath, '/') == TOOLS_TRUE || tools_isCharacterPresentAtEnd(rootPath, '\\') == TOOLS_TRUE)
        {
            if (tools_concatStrings(&g_entmgt_rootPath, 1u, rootPath) != TOOLS_SUCCESS)
            {
                TRACE_GEN(WARN, "String concatenation failed", __FUNCTION__, __LINE__);
                returnCode = COM_ERR_NO_MEMORY;
                goto exit;
            }
        }
        else
        {
            if (tools_concatStrings(&g_entmgt_rootPath, 2u, rootPath, "/") != TOOLS_SUCCESS)
            {
                TRACE_GEN(WARN, "String concatenation failed", __FUNCTION__, __LINE__);
                returnCode = COM_ERR_NO_MEMORY;
                goto exit;
            }
        }
    }

exit:

    END_FUNCTION();
    return returnCode;
}

/**
 * \brief Get the status of each registered entity
 * @return An error code
 * @todo Must return a list of entities with them status
 */
//T_com_status entmgt_launcher_getEntitiesStatus(size_t * nbEntities, T_entmgt_launcher_entity_status_info ** entities)
//{
//    T_com_status returnCode = COM_SUCCESS;
//
//    START_FUNCTION();
//
//    returnCode = entmgt_getStatusEntities(nbEntities, entities);
//
//    END_FUNCTION();
//    return returnCode;
//}

/**
 * \brief Initialize internal attributes
 * @details This function:
 * @li Register the application entities
 * @li Triggers initialization phase of all entities 
 * @return An error code
 */
T_com_status entmgt_launcher_initialize(void)
{
    T_com_status returnCode = COM_SUCCESS;

    START_FUNCTION();

    TRACE_GEN(TRACE, "Initialization phase...", __FUNCTION__, __LINE__);

    /* Check entity status */
    if (g_entmgt_appStatus == COM_ENTITY_IS_INITIALIZED)
    {
        TRACE_GEN(WARN, "Application is already initialized", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_UNINITIALIZED;
        goto exit;
    }

    /* Then, initialize all entities */
    returnCode = entmgt_initialize(g_entmgt_launcherList, &g_entmgt_launcherListSize);
    if (returnCode != COM_SUCCESS)
    {
        goto exit;
    }

    g_entmgt_appStatus = COM_ENTITY_IS_INITIALIZED;

exit:

    if (returnCode != COM_SUCCESS)
    {
        TRACE_GEN(ERROR, "Launcher initialize failed!", __FUNCTION__, __LINE__);
    }

    END_FUNCTION();
    return returnCode;
}

/**
 * \brief Configure entities
 * @details This function:
 * @li Parses all variables in configuration files 
 * @li Send variables to registered entities
 * This function must be called before any entities are started
 * @return An error code
 */
T_com_status entmgt_launcher_configure(void)
{
    T_com_status returnCode = COM_SUCCESS;

    START_FUNCTION();

    TRACE_GEN(TRACE, "Configuration phase...", __FUNCTION__, __LINE__);

    /* Check server status */
    if (g_entmgt_appStatus != COM_ENTITY_IS_INITIALIZED)
    {
        TRACE_GEN(ERROR, "Application should be in initialized state", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_UNINITIALIZED;
        goto exit;
    }

    /* Configure */
    returnCode = entmgt_configure(g_entmgt_launcherList, g_entmgt_launcherListSize);
    if (returnCode != COM_SUCCESS)
    {
        goto exit;
    }

    g_entmgt_appStatus = COM_ENTITY_IS_CONFIGURED;

exit:

    if (returnCode != COM_SUCCESS)
    {
        TRACE_GEN(ERROR, "Launcher configure failed!", __FUNCTION__, __LINE__);
    }

    END_FUNCTION();
    return returnCode;
}

/**
 * \brief Start entities
 * @details This function:
 * @li Starts all entities inside the application
 * @return An error code
 */
T_com_status entmgt_launcher_start(void)
{
    T_com_status returnCode = COM_SUCCESS;

    START_FUNCTION();

    TRACE_GEN(TRACE, "Start phase...", __FUNCTION__, __LINE__);

    /* Check server status */
    if (g_entmgt_appStatus != COM_ENTITY_IS_CONFIGURED)
    {
        TRACE_GEN(WARN, "Application should be in configured state", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_UNCONFIGURED;
        goto exit;
    }

    /* Stating */
    returnCode = entmgt_start(g_entmgt_launcherList, g_entmgt_launcherListSize);
    if (returnCode != COM_SUCCESS)
    {
        goto exit;
    }

    g_entmgt_appStatus = COM_ENTITY_IS_STARTED;

exit:

    if (returnCode != COM_SUCCESS)
    {
        TRACE_GEN(ERROR, "Launcher start failed!", __FUNCTION__, __LINE__);
        /* @todo use a syslog / snmp appender */
    }
    else
    {
        /* @todo use a syslog / snmp appender */
    }

    END_FUNCTION();
    return returnCode;
}

/**
 * \brief Stop entities
 * @details This function:
 * @li Stops all entities inside the application 
 * @return An error code
 */
T_com_status entmgt_launcher_stop(void)
{
    T_com_status returnCode = COM_SUCCESS;

    START_FUNCTION();

    TRACE_GEN(TRACE, "Stop phase...", __FUNCTION__, __LINE__);

    /* Check server status */
    if (g_entmgt_appStatus == COM_ENTITY_IS_STOPPED)
    {
        TRACE_GEN(WARN, "Application is already stopped", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_UNSTARTED;
        goto exit;
    }

    returnCode = entmgt_stop(g_entmgt_launcherList, &g_entmgt_launcherListSize, &g_entmgt_rootPath);
    if (returnCode != COM_SUCCESS)
    {
        goto exit;
    }

exit:

    g_entmgt_appStatus = COM_ENTITY_IS_STOPPED;
    if (returnCode != COM_SUCCESS)
    {
        TRACE_GEN(ERROR, "Launcher stop failed!", __FUNCTION__, __LINE__);
    }

    END_FUNCTION();
    return returnCode;
}
