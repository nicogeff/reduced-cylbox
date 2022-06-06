
/*------------------------------ Include files -------------------------------*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "com_error.h"
#include "com_entity.h"

#include "tools_trace.h"
#include "tools_parserJson.h"

#include "nal_libc.h"

#include "entmgt_utils.h"
#include "entmgt_target.h"
#include "entmgt_start.h"

/*------------------------------ Pragma --------------------------------------*/
/*------------------------------ Macros --------------------------------------*/
/*------------------------------ Structures ----------------------------------*/
/*------------------------------ Constants -----------------------------------*/
/*------------------------------ Variables -----------------------------------*/
/*-----------------------------  Private Functions ---------------------------*/

/**
 * Starting the entity management him self
 * @return 
 */
static T_com_status entmgt_startHimself(void)
{
    T_com_status returnCode = COM_SUCCESS;

    return returnCode;
}

/**
 * Starting all entitys registered
 * @return
 */
static T_com_status entmgt_startEntities(T_com_entity_launch * launcherList[], size_t launcherListSize)
{
    size_t i = 0u;
    T_com_status returnCode = COM_SUCCESS;
    char errMsg [ENTMGT_STRING_SIZE] = {0};
    size_t wSize = 0u;

    START_FUNCTION();

    for (i = 0u; i < launcherListSize; i++)
    {
        if (launcherList[ i]->startCallback != NULL)
        {
            returnCode = launcherList[i]->startCallback();
            if (returnCode != COM_SUCCESS)
            {
                nal_snprintf_s(errMsg,
                               ENTMGT_STRING_SIZE,
                               ENTMGT_STRING_SIZE,
                               &wSize,
                               "Could not start the %s entity",
                               launcherList[ i ]->nameSpace);
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
 * Starting function
 * @detail This function initialize the entity management first, then all entity registered
 * @param launcherList
 * @param launcherListSize
 * @return 
 */
T_com_status entmgt_start( T_com_entity_launch * launcherList[], size_t launcherListSize)
{
    T_com_status returnCode = COM_SUCCESS;

    START_FUNCTION();

    /* Sanity check */
    VERIF_ARGS("launcherList[]", launcherList != NULL, returnCode, COM_ERR_PARAM);
    if (returnCode != COM_SUCCESS)
    {
        TRACE_GEN(TRACE, "Invalid parameter", __FUNCTION__, __LINE__);
        goto exit;
    }

    returnCode = entmgt_startHimself();
    if (returnCode != COM_SUCCESS)
    {
        TRACE_GEN(ERROR, "Starting entity management himself failed", __FUNCTION__, __LINE__);
        goto exit;
    }

    returnCode = entmgt_startEntities (launcherList, launcherListSize);
    if (returnCode != COM_SUCCESS)
    {
        TRACE_GEN(ERROR, "Starting registered entitys failed", __FUNCTION__, __LINE__);
        goto exit;
    }

exit:

    END_FUNCTION();
    return returnCode;
}
