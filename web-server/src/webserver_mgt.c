
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nal_common.h"
#include "nal_mem.h"
#include "nal_rtos.h"

#include "com_error.h"
#include "com_entity.h"

#include "entmgt_launcher.h"

#include "tools_trace.h"
#include "tools_conversion.h"
#include "tools_fileManip.h"

#include "webserver_mgt.h"
#include "webserver_www.h"
#include "webserver_mcast.h"

/*---------------------------- Thread definitions ----------------------------*/

char g_webserver_www_thread_stack[WEBSERVER_WWW_THREAD_STACK_SIZE];
char g_webserver_mcast_thread_stack[WEBSERVER_MCAST_THREAD_STACK_SIZE];

/*------------------------ Configuration definitions -------------------------*/

/**
 * Global settings
 */
static char     g_WEBSERVER_CONF_WWW_ROOT_DIR[PATH_MAX] = DEFAULT_WEBSERVER_CONF_WWW_ROOT_DIR;
static char     g_WEBSERVER_CONF_WWW_PORT[16]           = DEFAULT_WEBSERVER_CONF_WWW_PORT;
static uint32_t g_WEBSERVER_CONF_WWW_MAX_CONNECTION     = (uint32_t)DEFAULT_WEBSERVER_CONF_WWW_MAX_CONNECTION;

static char     g_WEBSERVER_CONF_MCAST_GROUP[16]          = DEFAULT_WEBSERVER_CONF_MCAST_GROUP;
static char     g_WEBSERVER_CONF_MCAST_PORT[16]           = DEFAULT_WEBSERVER_CONF_MCAST_PORT;
static uint32_t g_WEBSERVER_CONF_MCAST_MAX_CONNECTION     = (uint32_t)DEFAULT_WEBSERVER_CONF_MCAST_MAX_CONNECTION;

/**
 * \brief Object used by launcher API to manage the PHYSICAL ACCESS agent module
 */
T_com_entity_launch webserverLauncher = {
    WEBSERVER_ENTITY_NS,
    NULL,
    webserver_init,
    webserver_configure,
    NULL,
    webserver_start,
    webserver_stop,
    NULL
};

/**
 * \brief Status of the  module
 */
static T_com_entity_status WEBSERVER_Status = COM_ENTITY_IS_STOPPED;

/*------------------------------- Thread variables ---------------------------*/

/**
 * \brief Status of physical access core thread
 */
NAL_BOOL g_webserver_www_thread_status = NAL_FALSE;
NAL_BOOL g_webserver_mcast_thread_status = NAL_FALSE;

T_nal_task g_webserver_www_thread_id;
T_nal_task g_webserver_mcast_thread_id;

/*------------------------------ Private functions ---------------------------*/

/**
 * \brief Switch local configuration settings to their default values
 * \return A status code
 */
static T_com_status webserver_resetLocalConfiguration(void)

{
    T_com_status returnCode = COM_SUCCESS;
    
    const char * rootDir = entmgt_launcher_getAppRootPath();
    char * wwwRootDir = NULL;
    
    START_FUNCTION();

    TRACE_GEN(TRACE, "Setting default local configuration values for WEB SERVICE entity", __FUNCTION__, __LINE__);
    
    if (tools_buildFolderPath(rootDir, DEFAULT_WEBSERVER_CONF_WWW_ROOT_DIR, &wwwRootDir) != TOOLS_SUCCESS)
    {
       returnCode =  COM_ERR_NOT_ALLOWED;
       TRACE_GEN(TRACE, "Setting to default value failed", __FUNCTION__, __LINE__);
       goto exit;
    }
    
    (void) nal_strncpy_s(
            g_WEBSERVER_CONF_WWW_ROOT_DIR, 
            PATH_MAX, 
            wwwRootDir, 
            strlen(wwwRootDir) * sizeof(char));
    
    NAL_FREE (wwwRootDir);

    (void) nal_strncpy_s(
            g_WEBSERVER_CONF_WWW_PORT, 
            16, 
            DEFAULT_WEBSERVER_CONF_WWW_PORT, 
            strlen(DEFAULT_WEBSERVER_CONF_WWW_PORT) * sizeof(char));
    
    g_WEBSERVER_CONF_WWW_MAX_CONNECTION = DEFAULT_WEBSERVER_CONF_WWW_MAX_CONNECTION;
    
exit:
    
    END_FUNCTION();
    return returnCode;
}

/*-------------------------- Accessor Functions  -----------------------------*/

char * get_WEBSERVER_CONF_WWW_ROOT_DIR (void) 
{ 
    return g_WEBSERVER_CONF_WWW_ROOT_DIR; 
}

char * get_WEBSERVER_CONF_WWW_PORT (void) 
{ 
    return g_WEBSERVER_CONF_WWW_PORT; 
}

uint32_t get_WEBSERVER_CONF_WWW_MAX_CONNECTION (void) 
{ 
    return g_WEBSERVER_CONF_WWW_MAX_CONNECTION; 
}

char * get_WEBSERVER_CONF_MCAST_GROUP (void)
{
    return g_WEBSERVER_CONF_MCAST_GROUP;
}

char * get_WEBSERVER_CONF_MCAST_PORT (void) 
{ 
    return g_WEBSERVER_CONF_MCAST_PORT; 
}

uint32_t get_WEBSERVER_CONF_MCAST_MAX_CONNECTION (void)
{
    return g_WEBSERVER_CONF_MCAST_MAX_CONNECTION;
}

/*------------------------ Launcher API Functions  ---------------------------*/

/**
 * \brief Initialize the  module
 * \details This function initializes some resources like mutexes and memory structures.
 * This  function must be the first one to be called in this module.
 * \return A status code
 */
T_com_status webserver_init
(
    void
)
{
    T_com_status returnCode = COM_SUCCESS;

    START_FUNCTION();

    TRACE_GEN(TRACE, "Initializing WEB SERVER entity...", __FUNCTION__, __LINE__);

    /* Check module status */
    if (WEBSERVER_Status == COM_ENTITY_IS_STOPPED)
    {

        /* Initialize WWW thread status */
        g_webserver_www_thread_status = NAL_TRUE;
        
        /* Initialize multicast thread status */
        g_webserver_mcast_thread_status = NAL_TRUE;
        
        /* Initialization done! */
        WEBSERVER_Status = COM_ENTITY_IS_INITIALIZED;
        returnCode = COM_SUCCESS;
    }
    else
    {
        TRACE_GEN(WARN, "Module is already initialized", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_UNSTOPPED; /* @todo no error code for already initialized? */
    }

exit:

    if (returnCode != COM_SUCCESS)
    {
        g_webserver_www_thread_status = NAL_FALSE;
        g_webserver_mcast_thread_status = NAL_FALSE;

        if (returnCode != COM_ERR_UNSTOPPED) /* @todo no error code for already initialized? */
        {
            /* @Todo: destroy resources already created */
        }
    }

    END_FUNCTION();
    return returnCode;
}

/**
 * \brief Configuration function for module
 * \details Configuration function for module.
 * \param[in] nbSettings Number of configuration variable (can be 0)
 * \param[in] settings Name/Value pair of configuration variables (can be NULL)
 * \return A status code
 */
T_com_status webserver_configure
(
    size_t nbSettings,
    T_com_entity_setting_ptr settings
)
{
    T_com_status returnCode = COM_SUCCESS;
    uint32_t idx = 0u;

    START_FUNCTION();

    TRACE_GEN(TRACE, "Configuring WEB SERVER entity...", __FUNCTION__, __LINE__);

    /* Check server status */
    if (WEBSERVER_Status != COM_ENTITY_IS_INITIALIZED)
    {
        TRACE_GEN(WARN, "Module should be in initialized state", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_UNINITIALIZED;
        goto exit;
    }

    /* Reset configuration before apply the new one */
    webserver_resetLocalConfiguration();
    
    /* For each setting */
    if (settings != NULL)
    {
        for (idx = 0u; idx < nbSettings; idx++)
        {
            if (settings[idx].key == NULL || settings[idx].value == NULL)
            {
                returnCode = COM_ERR_NOT_ALLOWED;
                goto exit;
            }
            
            if (strcmp (WEBSERVER_CONF_WWW_ROOT_DIR_NAME, settings[idx].key) == 0)
            {
                memset(g_WEBSERVER_CONF_WWW_ROOT_DIR, 0, PATH_MAX);
                memcpy(g_WEBSERVER_CONF_WWW_ROOT_DIR, settings[idx].value, PATH_MAX);
            }
            else if (strcmp (WEBSERVER_CONF_WWW_PORT_NAME, settings[idx].key) == 0)
            {
                memset(g_WEBSERVER_CONF_WWW_PORT, 0, 16);
                memcpy(g_WEBSERVER_CONF_WWW_PORT, settings[idx].value, 16);
            }
            else if (strcmp (WEBSERVER_CONF_WWW_MAX_CONNECTION_NAME, settings[idx].key) == 0)
            {
                if (tools_text2unsigned(settings[idx].value, &g_WEBSERVER_CONF_WWW_MAX_CONNECTION) == 0)
                {
                    returnCode = COM_ERR_NOT_ALLOWED;
                    goto exit;
                }
            }
            else
            {
                TRACE_GEN (WARN, "Configuration setting is not recognized by this entity", __FUNCTION__, __LINE__);
            }
        }
    }

    WEBSERVER_Status = COM_ENTITY_IS_CONFIGURED;

exit:

    /* In case of failure use default settings */
    if (returnCode != COM_SUCCESS)
    {
        TRACE_GEN(ERROR, "Error occurred in DEVICE ACCESS entity configuration, some settings value may be invalid.", __FUNCTION__, __LINE__);
        (void) webserver_resetLocalConfiguration();
    }
    END_FUNCTION();
    return returnCode;    
}

/**
 * \brief Starts the entity.
 * \details This function starts to work the whole Entity Orchestrator.
 * The entity must be fully initialized and configured before a call to this function.
 * \return A status code
 */
T_com_status webserver_start
(
    void
)
{
    T_com_status returnCode = COM_SUCCESS;
    int al_returnCode = NAL_SUCCESS;
    
    START_FUNCTION();

    TRACE_GEN(TRACE, "Starting WEB SERVER entity...", __FUNCTION__, __LINE__);

    /* Check server status */
    if (WEBSERVER_Status != COM_ENTITY_IS_CONFIGURED)
    {
        TRACE_GEN(WARN, "Entity should be in configured state", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_UNCONFIGURED;
        goto exit;
    }
    
    /* Start WWW thread */
    al_returnCode = nal_taskCreate ( &g_webserver_www_thread_id,
                                WEBSERVER_WWW_THREAD_NAME,
                                webserver_www_thread,
                                NULL, /* void *data */
                                WEBSERVER_WWW_THREAD_PRIORITY, /* take care of OS */
                                (void *) g_webserver_www_thread_stack,
                                WEBSERVER_WWW_THREAD_STACK_SIZE
                              );
    if (NAL_SUCCESS != al_returnCode)
    {
        TRACE_GEN(TRACE, "Could not initialize Web server WWW task", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_TASK;
        goto exit;
    }    

    /* Start MCAST thread */
    al_returnCode = nal_taskCreate ( &g_webserver_mcast_thread_id,
                                WEBSERVER_MCAST_THREAD_NAME,
                                webserver_mcast_thread,
                                NULL, /* void *data */
                                WEBSERVER_MCAST_THREAD_PRIORITY, /* take care of OS */
                                (void *) g_webserver_mcast_thread_stack,
                                WEBSERVER_MCAST_THREAD_STACK_SIZE
                              );
    if (NAL_SUCCESS != al_returnCode)
    {
        TRACE_GEN(TRACE, "Could not initialize Web server WWW task", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_TASK;
        goto exit;
    }
    
    /* Set started status */
    WEBSERVER_Status = COM_ENTITY_IS_STARTED;

exit:
    
    END_FUNCTION();
    return returnCode;
}

T_com_status webserver_stop
(
 void
)
{
    T_com_status returnCode = COM_SUCCESS;
    int32_t al_result = NAL_SUCCESS;

    START_FUNCTION();

    TRACE_GEN(TRACE, "Stopping WEB SERVER entity...", __FUNCTION__, __LINE__);
    
    /* Check server status */
    if (WEBSERVER_Status == COM_ENTITY_IS_STOPPED)
    {
        TRACE_GEN(WARN, "Module is already stopped...", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_UNINITIALIZED;
        goto exit;
    }
    
    /* Stop WWW thread */
    webserver_www_stop();
    
    /* Stop MCAST thread */
    webserver_mcast_stop();
    
    /* Reset local configuration */
    returnCode = webserver_resetLocalConfiguration();
    
exit:
    
    WEBSERVER_Status = COM_ENTITY_IS_STOPPED;

    END_FUNCTION();
    return returnCode;    
}