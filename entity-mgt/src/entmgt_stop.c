/**
 * \file
 * \brief Stopping functions
 */

/*------------------------------ Include files -------------------------------*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "com_error.h"
#include "com_entity.h"

#include "cryptoal_mgt.h"
#include "tools_trace.h"
#include "tools_parserJson.h"

#include "nal_libc.h"

#include "entmgt_utils.h"
#include "entmgt_target.h"
#include "entmgt_stop.h"

/*------------------------------ Pragma --------------------------------------*/
/*------------------------------ Macros --------------------------------------*/
/*------------------------------ Structures ----------------------------------*/
/*------------------------------ Constants -----------------------------------*/
/*------------------------------ Variables -----------------------------------*/
/*-----------------------------  Private Functions ---------------------------*/

/**
 * Stopping the entity management him self
 * @return 
 */
static T_com_status entmgt_stopHimself(T_com_entity_launch * launcherList[], size_t * launcherListSize, char ** rootPath)
{
    T_com_status returnCode = COM_SUCCESS;

    /* Reset launcher list */
    memset(launcherList, 0, (size_t) (sizeof (T_com_entity_launch_ptr) *(size_t) ENTMGT_MAX_ENTITIES));
    *launcherListSize = 0u;

    if (*rootPath != NULL)
    {
        free(*rootPath);
        *rootPath = NULL;
    }

    return returnCode;
}

/**
 * Stopping all entitys registered
 * @return
 */
static T_com_status entmgt_stopEntities(T_com_entity_launch * launcherList[], size_t launcherListSize)
{
    size_t i = 0u;
    T_com_status returnCode = COM_SUCCESS;
    char errMsg [ENTMGT_STRING_SIZE] = {0};
    size_t wSize = 0u;

    START_FUNCTION();

    for (i = launcherListSize; i-- > 0u;)
    {
        if (launcherList[ i ]->stopCallback != NULL)
        {
            returnCode = launcherList[i]->stopCallback();
            if (returnCode != COM_SUCCESS)
            {
                nal_snprintf_s(errMsg, 
                               ENTMGT_STRING_SIZE, 
                               ENTMGT_STRING_SIZE, 
                               &wSize, 
                               "Could not properly stop the %s entity", 
                               launcherList[ i ]->nameSpace);
                TRACE_GEN(ERROR, errMsg, __FUNCTION__, __LINE__);
            }
        }
    }

    END_FUNCTION();
    return returnCode;
}

/*-----------------------------  Public Functions ----------------------------*/

/**
 * Stopping function
 * @detail This function initialize the entity management first, then all entity registered
 * @param launcherList
 * @param launcherListSize
 * @param rootPath
 * @return 
 */
T_com_status entmgt_stop(T_com_entity_launch * launcherList[], size_t * launcherListSize, char ** rootPath)
{
    T_com_status returnCode = COM_SUCCESS;

    START_FUNCTION();

    /* Sanity check */
    VERIF_ARGS("launcherList[]", launcherList != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("launcherListSize", launcherListSize != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("rootPath", rootPath != NULL, returnCode, COM_ERR_PARAM);
    if (returnCode != COM_SUCCESS)
    {
        TRACE_GEN(TRACE, "Invalid parameter", __FUNCTION__, __LINE__);
        goto exit;
    }

    returnCode = entmgt_stopEntities(launcherList, *launcherListSize);
    if (returnCode != COM_SUCCESS)
    {
        TRACE_GEN(ERROR, "Starting registered entitys failed", __FUNCTION__, __LINE__);
        goto exit;
    }

    /* Stop  "crypto_al " sub-entity, only start/stop as this is not a real module (NO init/configure/start/stop, no NameSpace and no settings) */
    returnCode = cryptoal_reset  ();
    if (returnCode != COM_SUCCESS)
    {
        TRACE_GEN(ERROR, "Stopping sub-module crypto_al failed", __FUNCTION__, __LINE__);
        goto exit;
    }


    returnCode = entmgt_stopHimself(launcherList, launcherListSize, rootPath);
    if (returnCode != COM_SUCCESS)
    {
        TRACE_GEN(ERROR, "Starting entity management himself failed", __FUNCTION__, __LINE__);
        goto exit;
    }

exit:

    END_FUNCTION();
    return returnCode;
}

