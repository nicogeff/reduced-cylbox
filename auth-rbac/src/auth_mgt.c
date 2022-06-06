/**
 * \file
 * \brief utilities for AUTH-RBAC Entity management
 *
 * \date 04/2020 NGE Creation
 */

/*------------------------------ Definitions --------------------------------------*/
/* This source file DO NOT create Rbac tables: */
#define EXTERN_AUTH extern

/*------------------------------ Include files -------------------------------*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>

#include "nal_common.h"
#include "nal_rtos_posix.h"
#include "nal_rtos.h"
#include "nal_libc.h"
#include "nal_mem.h"

#include "com_error.h"
#include "com_entity.h"
#include "tools_trace.h"
#include "tools_parserJson.h"
#include "tools_conversion.h"
#include "tools_fileManip.h"
#include "entmgt_launcher.h"
#include "entmgt_utils.h"

/* Exported  API: */
#include "com_auth_rbac_api.h"

#include "auth_global_rbac.h"




/*------------------------------ Settings -----------------------------------*/

/**
 * Setting names
 */
#define AUTH_RBAC_SESSION_DURATION_NAME      "AUTH_RBAC_SESSION_DURATION_SEC"    /* Time to live for  a session, since last time is was authenticated */
#define AUTH_RBAC_LOCKACCOUNT_DURATION_NAME      "AUTH_RBAC_LOCKACCOUNT_DURATION_SEC"    /* Duration an account is locked */

/**
 * Setting globals
 */
static uint32_t gAUTH_SESSION_DURATION    = (uint32_t) DEFAULT_AUTH_SESSION_DURATION;
static uint32_t gAUTH_LOCKACCOUNT_DURATION    = (uint32_t) DEFAULT_AUTH_LOCKACCOUNT_DURATION;

/*------------------------------ Resources -----------------------------------*/

/* ORCHESTRATOR rx messages queue */
/* N/A */

/* ORCHESTRATOR rx messages queue workspace */
/* N/A */

/* ORCHESTRATOR mutex on echeancier */
T_nal_mutex auth_rbac_mutex_id;
#define AUTH_RBAC_MUTEX_NAME "AuthRbacMutex"

/* threads with stack if needed */
/* 2 threads with stack */
#define AUTH_THREAD_STACK_SIZE (16384*2)   /*Limit of machine currently ?*/
#define AUTH_DURATION_THREAD_NAME "AuthDurationThread"
char auth_duration_thread_stack[AUTH_THREAD_STACK_SIZE];

/* Identifier and status of thread, in charge to check everything that as a limited time to live in the Rbac */
T_nal_task auth_duration_thread_id;
NAL_BOOL auth_duration_thread_status = NAL_FALSE;



/*------------------------------ Structures to manage Entity -----------------------------------*/

/**
 * \brief Object used by launcher API to manage the auth-bac entity
 */
T_com_entity_launch authRbacLauncher = {
    AUTH_RBAC_NS,
    auth_rbac_getStatus,
    auth_rbac_init,
    auth_rbac_configure,
    auth_rbac_setSecurityPolicies,
    auth_rbac_start,
    auth_rbac_stop,
    NULL
};


/**
 * \brief Status of the  entity
 */
T_com_entity_status AUTH_RBAC_Status = COM_ENTITY_IS_STOPPED;

/*------------------------------ Private functions ---------------------------*/

/**
 * \brief Switch the  agent security policies settings to their default values
 * \return A cs status code.
 */

static T_com_status auth_rbac_resetSecurityPolicies(void)
{
    T_com_status returnCode = COM_SUCCESS;
    START_FUNCTION();

    END_FUNCTION();
    return returnCode;
}

/**
 * \brief Switch the  agent Local configuration settings to their default values
 * \return A cs status code.
 */
static T_com_status auth_rbac_resetLocalConfiguration(void)
{
    T_com_status returnCode = COM_SUCCESS;
    START_FUNCTION();

    gAUTH_SESSION_DURATION       = (uint32_t) DEFAULT_AUTH_SESSION_DURATION;
    gAUTH_LOCKACCOUNT_DURATION       = (uint32_t) DEFAULT_AUTH_LOCKACCOUNT_DURATION;

    END_FUNCTION();
    return returnCode;
}


/*------------------------------ Functions -----------------------------------*/


/* ----------------- API Functions for Entity Mgt  --------------- */


/**
 * \brief Initialize Entity and settings to default values, create all system tools like queue, mutex, threads
 * \details 
 * If this function failed, nothing is created and state is STOPPED. Else state is INITIALIZED
 * \return A status code
 */
T_com_status auth_rbac_init
(
 void
)
{
    T_com_status returnCode = COM_SUCCESS;
    int alreturn = 0;

    START_FUNCTION();

    TRACE_GEN(TRACE, "Initializing AUTH_RBAC entity..." , __FUNCTION__, __LINE__ );

    /* Check entity status */
    if (AUTH_RBAC_Status == COM_ENTITY_IS_STOPPED)
    {
        /* Init of settings */
        (void)auth_rbac_resetLocalConfiguration ();

        /* Initialize Thread to all time to live in the Rbac */
        auth_duration_thread_status = NAL_TRUE;
        /*   - rx message read thread */
        alreturn = nal_taskCreate ( & auth_duration_thread_id,
                                    AUTH_DURATION_THREAD_NAME,
                                    auth_duration_thread,
                                    NULL, /* void *data */
                                    MEDIUM_TASK_PRIO, /* take care of OS */
                                    (void *) auth_duration_thread_stack,
                                    AUTH_THREAD_STACK_SIZE
                                  );
        if (NAL_SUCCESS != alreturn)
        {
            TRACE_GEN(TRACE, "Could not initialize rx message task", __FUNCTION__, __LINE__);
            returnCode = COM_ERR_TASK;
            goto exit;
        }

        /* Create mutex for timetable access */
        if ( NAL_SUCCESS != nal_mutexInit ( & auth_rbac_mutex_id, AUTH_RBAC_MUTEX_NAME) )
        {
            TRACE_GEN (TRACE, "Could not initialize Rbac mutex, to protect access in Ram", __FUNCTION__, __LINE__);
            returnCode = COM_ERR_MUTEX;
            goto exit;
        }

        /* Initialization done, declares entity INITIALIZED! */
        AUTH_RBAC_Status = COM_ENTITY_IS_INITIALIZED;
        returnCode = COM_SUCCESS;
    }
    else
    {
        TRACE_GEN(WARN, "Entity is already initialized, it should be stopped", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_UNSTOPPED;
    }

exit:

    if (returnCode != COM_SUCCESS)   /* failing init */
    {
        AUTH_RBAC_Status = COM_ENTITY_IS_STOPPED;
        auth_duration_thread_status = NAL_FALSE;
    }

    END_FUNCTION();
    return returnCode;
}

/**
 * \brief Give the settings to the Entity, 0 or more tuples. Shall happen in INITIALIZED mode
 * \details 
 * If this function failed, settings are switched back to their default values and nothing else happen
 * \param[in] nbSettings
 * \param[in] points a table of tuple key/value
 * \return A status code
 */
T_com_status auth_rbac_configure
(
 size_t nbSettings,
 T_com_entity_setting_ptr settings
 )
{
    /* initialize */
    T_com_status returnCode = COM_SUCCESS;
    uint32_t idx = 0u;

    START_FUNCTION();

    TRACE_GEN(TRACE, "Configuring AUTH_RBAC entity...", __FUNCTION__, __LINE__);

    /* Check server status */
    if (AUTH_RBAC_Status != COM_ENTITY_IS_INITIALIZED)
    {
        TRACE_GEN(WARN, "Entity AUTH_RBAC should be in initialized state, cannot configure", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_UNINITIALIZED;
        goto exit;
    }

    /* Reset configuration before apply the new one */
    (void) auth_rbac_resetLocalConfiguration();
    
    if (settings != NULL)
    {
        for (idx = 0u; idx < nbSettings; idx++)     /* For each setting */
        {
            if ( (NULL == settings[idx].key) ||
                 (NULL == settings[idx].value)
               )
            {
                returnCode = COM_ERR_NOT_ALLOWED;
                goto exit;
            }
            
            /* Here init of all settings if any */
            if (strcmp (AUTH_RBAC_SESSION_DURATION_NAME, settings[idx].key) == 0)
            {
                gAUTH_SESSION_DURATION = settings[idx].value;
            }
            else  if (strcmp (AUTH_RBAC_LOCKACCOUNT_DURATION_NAME, settings[idx].key) == 0)
            {
                gAUTH_LOCKACCOUNT_DURATION = settings[idx].value;
            }
            else
            {
                TRACE_GEN (TRACE, "AUTH: Configuration setting is not recognized by this entity", __FUNCTION__, __LINE__);
            }


        }
    }

    AUTH_RBAC_Status = COM_ENTITY_IS_CONFIGURED;

exit:

    /* In case of failure use default settings */
    if (returnCode != COM_SUCCESS)
    {
        TRACE_GEN(ERROR, "Error occured in AUTH_RBAC agent configuration, some settings value may be invalid or malformed.", __FUNCTION__, __LINE__);
        (void) auth_rbac_resetLocalConfiguration();
    }
    END_FUNCTION();
    return returnCode;

}

/**
 * \brief Set security policies inside  entity
 * \details
 * Used to push all  security policies values defined in the security_policies.exi file.
 * To get the policy settings fro the managemeent agent, entity must register a function that will do the configuration for its own namespace.
 * The namespace will be compared to the namespace inside the exi file so the management agent can pass the right settings to the registered entity.
 * If this function failed, settings are switched back to their default values
 * This function must be called when entity is at \b configured  or \b started state.
 * \param[in] nbSettings Number of  variables (can be 0)
 * \param[in] settings Name/Value pair of configuration variables (can be NULL)
 * \return CS status code
 */
T_com_status auth_rbac_setSecurityPolicies
(
 size_t nbSettings,
 T_com_entity_setting_ptr settings
 )
{
    /* Initialize */
    T_com_status returnCode = COM_SUCCESS;


    START_FUNCTION();

    TRACE_GEN(TRACE, "Setting security policies for AUTH_RBAC  entity...", __FUNCTION__, __LINE__);

    /*TODO for all settings*/

    END_FUNCTION();
    return returnCode;
}

/**
 * \brief Starts the entity.
 * \details This function starts to work the whole Entity auth-rbac.
 * The entity must be fully initialized and configured before a call to this function.
 * \return A status code
 */
T_com_status auth_rbac_start
(
 void
 )
{
    T_com_status returnCode = COM_SUCCESS;
    char * rbac_file_path = NULL;
    char * rbac_factory_file_path = NULL;
    char * buffer = NULL;
    size_t bufferSize = 0u;
    bool on_factory_rbac = false;
    T_tools_json_value * rawRbacData = NULL;  /* json Raw Data figuring the Rbac */
    
    START_FUNCTION();

    TRACE_GEN(TRACE, "Starting AUTH_RBAC entity...", __FUNCTION__, __LINE__);

    /* Check server status */
    if (AUTH_RBAC_Status != COM_ENTITY_IS_CONFIGURED)
    {
        TRACE_GEN(WARN, "Entity AUTH_RBAC should be in configured state", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_UNCONFIGURED;
        goto exit;
    }

    /* Initilize an empty Rbac in Ram, under mutex protection */
    (void) auth_initialize_rbac ();

    
    /************ creation Rbac in Ram, starting from resource file giving a json buffer ************/
    
    /* build operational rbac file path */
    if (tools_buildFilePath(entmgt_launcher_getAppRootPath(), ENTMGT_DIR_RUNNING_RBAC, ENTMGT_FILE_RUNNING_RBAC, &rbac_file_path) != TOOLS_SUCCESS) {
        TRACE_GEN(ERROR, "auth_mgt start: Building operational rbac file path failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_INTERNAL;
        goto exit;
    }
    /* Read file in allocated buffer */
    returnCode = entmgt_readInternalFile(rbac_file_path, &buffer, &bufferSize);
    if (returnCode != COM_SUCCESS) {
        TRACE_GEN(WARN, "auth_mgt start: Read Rbac file failed, copy and start on factory rbac file", __FUNCTION__, __LINE__);

        /* build factory rbac file path */
        if (tools_buildFilePath(entmgt_launcher_getAppRootPath(), ENTMGT_DIR_FACTORY_RBAC, ENTMGT_FILE_FACTORY_RBAC, &rbac_factory_file_path) != TOOLS_SUCCESS) {
            TRACE_GEN(ERROR, "auth_mgt start: Building operational rbac file path failed", __FUNCTION__, __LINE__);
            returnCode = COM_ERR_INTERNAL;
            goto exit;
        }
        /* Read file in allocated buffer */
        returnCode = entmgt_readInternalFile(rbac_factory_file_path, &buffer, &bufferSize);
        if (returnCode != COM_SUCCESS) {
            TRACE_GEN(ERROR, "auth_mgt start: Read factory Rbac file failed", __FUNCTION__, __LINE__);
            returnCode = COM_ERR_INTERNAL;
            goto exit;
        }
        /* if true, tell to copy factory brac file to running rbac file, after building Rbac*/
        on_factory_rbac = true;

    }

    /* Apply a  json parsing on buffer */
    returnCode = entmgt_parseInternalJsonBuffer(buffer, bufferSize, &rawRbacData);
    if (returnCode != COM_SUCCESS)
    {
        TRACE_GEN(ERROR, "auth_mgt start: Parsing local Rbac buffer failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_INTERNAL;
        goto exit;
    }

    /**
     ** Build complete Rbac structures in Ram, given a json buffer, under mutex protection  **
     **/
    returnCode = auth_loadJsonBuffer_in_rbac_ram ( (void *)  rawRbacData);
    if (returnCode != COM_SUCCESS)
    {
        TRACE_GEN(ERROR, "auth_mgt start: exploiting json rbac failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_INTERNAL;
        goto exit;
    }

    /* Consider and copy now factory rbac file to running Rbac file  */
    if (true == on_factory_rbac)
    {
        if (tools_copyFile(rbac_factory_file_path, rbac_file_path) != TOOLS_SUCCESS)
            TRACE_GEN(ERROR, "Coping factory Rbac file to running folder failed", __FUNCTION__, __LINE__);
    }

    /******** Set started status ********/
    AUTH_RBAC_Status = COM_ENTITY_IS_STARTED;

exit:

    if (NULL != rbac_file_path)
      NAL_FREE(rbac_file_path);
    if (NULL != rbac_factory_file_path)
      NAL_FREE(rbac_factory_file_path);
    if (NULL != buffer)
      NAL_FREE(buffer);

    END_FUNCTION();
    return returnCode;
}

/**
 * \brief Stops the entity.
 * \details The entity must be started before calling this function.
 * This function stops the entity, and destroy or free any resources
 * \return A status code
 */
T_com_status auth_rbac_stop
(
 void
 )
{
    T_com_status returnCode = COM_SUCCESS;
    int32_t result = NAL_SUCCESS;

    START_FUNCTION();

    TRACE_GEN(TRACE, "Stopping AUTH_RBAC entity...", __FUNCTION__, __LINE__);

    /* Check server status */
    if (AUTH_RBAC_Status == COM_ENTITY_IS_STOPPED)
    {
        TRACE_GEN(WARN, "Entity AUTH_RBAC is already stopped...", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_UNINITIALIZED;
        goto exit;
    }

    result = nal_mutexDelete ( & auth_rbac_mutex_id );
    if (result != NAL_SUCCESS)
    {
        TRACE_GEN (ERROR, "Could not delete mutex of AUTH_RBAC in function %s at line %d.", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_MUTEX;
    }

    /* Reset local configuration */
    returnCode = auth_rbac_resetLocalConfiguration();

    /* Reset local configuration */
    returnCode = auth_rbac_resetSecurityPolicies();

exit:
    AUTH_RBAC_Status = COM_ENTITY_IS_STOPPED;
    END_FUNCTION();
    return returnCode;
}

/**
 * \brief give status of the  entity.
 * \details The entity may be in any state.
 * \return always COM_SUCCESS
 */
T_com_status auth_rbac_getStatus
(
 T_com_entity_status * entityStatus
 )
{
    T_com_status returnCode = COM_SUCCESS;
    START_FUNCTION();

    TRACE_GEN(TRACE, "get AUTH_RBAC entity state...", __FUNCTION__, __LINE__);

    /* gives server status */
    *entityStatus = AUTH_RBAC_Status;

    END_FUNCTION();
    return returnCode;
}

/* \brief 2 Setting Accessors to read */
uint32_t get_auth_rbac_session_duration ( void )
{
    return  gAUTH_SESSION_DURATION;
}
uint32_t get_auth_rbac_lockaccount_duration ( void )
{
  return  gAUTH_LOCKACCOUNT_DURATION;
}