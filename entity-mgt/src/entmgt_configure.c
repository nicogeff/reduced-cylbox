
/*------------------------------ Include files -------------------------------*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "com_error.h"
#include "com_entity.h"

#include "tools_trace.h"
#include "tools_strManip.h"
#include "tools_parserJson.h"
#include "tools_fileManip.h"

#include "nal_libc.h"
#include "nal_stor.h"
#include "nal_mem.h"
#include "nal_net.h"
#include "nal_rtos.h"
#include "nal_stor_stdio.h"

#include "entmgt_launcher.h"
#include "entmgt_configure.h"
#include "entmgt_utils.h"

#include "cryptoal_hash.h"

/*------------------------------ Pragma --------------------------------------*/
/*------------------------------ Macros --------------------------------------*/
/*------------------------------ Structures ----------------------------------*/
/*------------------------------ Constants -----------------------------------*/
/*------------------------------ Variables -----------------------------------*/

static char g_entmgt_device_uuid[ENTMGT_MAX_SETTING_STR_SIZE] = {0};
static char g_entmgt_device_hostname[ENTMGT_MAX_SETTING_STR_SIZE] = {0};
static char g_entmgt_device_net_if[ENTMGT_MAX_SETTING_STR_SIZE] = {0};
static char g_entmgt_device_ip_addr[ENTMGT_MAX_SETTING_STR_SIZE] = {0};
static char g_entmgt_device_system[ENTMGT_MAX_SETTING_STR_SIZE] = {0};
static char g_entmgt_device_location[ENTMGT_MAX_SETTING_STR_SIZE] = {0};

/*-----------------------------  Private Functions ---------------------------*/
static T_com_status entmgt_generateDeviceUUID()
{
    T_com_status returnCode = COM_SUCCESS;

    char *luuid = NULL;
    const char *ifname = NULL;
    char *ifmac = NULL;
    int idx = 0;
    unsigned char hash[32] = {0};
    char hashStr[65] = {0};

    T_nal_netifInfo *netifs = NULL;
    size_t netifsCount = 0u;

    START_FUNCTION();

    ifname = entmgt_get_device_net_if();

    if (NAL_SUCCESS != nal_getNetifsInfo(&netifs, &netifsCount))
    {
        TRACE_GEN(ERROR, "Getting network interface information failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_NETIF;
        goto exit;
    }

    for (idx = 0; idx < netifsCount; ++idx)
    {
        if (strcmp(netifs[idx].name, ifname) == 0)
        {
            ifmac = NAL_MALLOC(12 * sizeof (char) + 1);
            if (NULL == ifmac)
            {
                returnCode = COM_ERR_NO_MEMORY;
                goto exit;
            }
            memset(ifmac, 0, 12 * sizeof (char) + 1);

            snprintf(ifmac, 12 * sizeof (char) + 1, "%02x%02x%02x%02x%02x%02x",
                     netifs[idx].mac_address[0],
                     netifs[idx].mac_address[1],
                     netifs[idx].mac_address[2],
                     netifs[idx].mac_address[3],
                     netifs[idx].mac_address[4],
                     netifs[idx].mac_address[5]);
            break;
        }
    }

    /* If mac address is not found used a random number */
    if (NULL == ifmac)
    {
        T_nal_time dly;

        ifmac = NAL_MALLOC(12 * sizeof (char) + 1);
        if (NULL == ifmac)
        {
            returnCode = COM_ERR_NO_MEMORY;
            goto exit;
        }
        memset(ifmac, 0, 12 * sizeof (char) + 1);

        nal_getPosixTime(&dly);
        snprintf(ifmac, 12 + 1, "%s%x", ENTMGT_UUID_FIXED_VALUE_NO_MAC, dly.sec + dly.usec);
    }

    /* Build complete UUID */
    tools_concatStrings(&luuid, 5,
                        ENTMGT_UUID_FIXED_VALUE_0,
                        ENTMGT_UUID_FIXED_VALUE_1,
                        ENTMGT_UUID_FIXED_VALUE_2,
                        ENTMGT_UUID_FIXED_VALUE_3,
                        ifmac);

    /* Hash UUID to flood the MAC address */
    cryptoal_SHA256(luuid, strlen(luuid), hash);
    
    /* Convert raw hash to string type */
    for(idx = 0; idx < 32; idx++)
        sprintf(hashStr + (idx * 2), "%02x", hash[idx]);
    
    /* Clear the global UUID variable */
    memset(g_entmgt_device_uuid, 0, ENTMGT_MAX_SETTING_STR_SIZE);
    
    /* Keep only 32 byte and format according the UPnP v2 standard */
    idx = 0;
    strncat(g_entmgt_device_uuid, &hashStr[idx], 8);
    strncat(g_entmgt_device_uuid, "-", 1);
    idx += 8;
    strncat(g_entmgt_device_uuid, &hashStr[idx], 4);
    strncat(g_entmgt_device_uuid, "-", 1);
    idx += 4;
    strncat(g_entmgt_device_uuid, &hashStr[idx], 4);
    strncat(g_entmgt_device_uuid, "-", 1);
    idx += 4;
    strncat(g_entmgt_device_uuid, &hashStr[idx], 4);
    strncat(g_entmgt_device_uuid, "-", 1);
    idx += 4;
    strncat(g_entmgt_device_uuid, &hashStr[idx], 12);
    
exit:
    if (NULL != netifs)
    {
        nal_releaseNetifsInfo(netifs, netifsCount);
    }
    if (NULL != ifmac)
    {
        NAL_FREE(ifmac);
        ifmac = NULL;
    }
    if (NULL != luuid)
    {
        NAL_FREE(luuid);
        luuid = NULL;
    }

    END_FUNCTION();
    return returnCode;
}

/**
 * Reset all entity management settings to default
 * @return 
 */
static T_com_status entmgt_resetLocalConfiguration(void)
{

    T_com_status returnCode = COM_SUCCESS;

    START_FUNCTION();

    memcpy(&g_entmgt_device_hostname, ENTMGT_SETTING_VALUE_DFLT_DEVICE_HOSTNAME, strlen(ENTMGT_SETTING_VALUE_DFLT_DEVICE_HOSTNAME));
    memcpy(&g_entmgt_device_net_if, ENTMGT_SETTING_VALUE_DFLT_DEVICE_NET_IF, strlen(ENTMGT_SETTING_VALUE_DFLT_DEVICE_NET_IF));
    memcpy(&g_entmgt_device_ip_addr, ENTMGT_SETTING_VALUE_DFLT_DEVICE_IP_ADDR, strlen(ENTMGT_SETTING_VALUE_DFLT_DEVICE_IP_ADDR));
    memcpy(&g_entmgt_device_system, ENTMGT_SETTING_VALUE_DFLT_DEVICE_SYSTEM, strlen(ENTMGT_SETTING_VALUE_DFLT_DEVICE_SYSTEM));
    memcpy(&g_entmgt_device_location, ENTMGT_SETTING_VALUE_DFLT_DEVICE_LOCATION, strlen(ENTMGT_SETTING_VALUE_DFLT_DEVICE_LOCATION));

    END_FUNCTION();

    return returnCode;
}

/**
 * Configure the Entity management himself
 * @param nbSettings
 * @param settings
 * @return 
 */
static T_com_status entmgt_configureHimself(const T_tools_json_value * rawConfigurationData)
{
    T_com_status returnCode = COM_SUCCESS;
    size_t i = 0u;
    size_t length = 0u;

    size_t nbSettings = 0u;
    T_com_entity_setting * settings = NULL;

    START_FUNCTION();

    /* Set all local settings to default */
    entmgt_resetLocalConfiguration();

    /* If RAW data is not empty treat him, otherwise we will took the default value*/
    if (rawConfigurationData != NULL)
    {
        returnCode = entmgt_getSettingsFromRawConfigurationData(
                                                                rawConfigurationData,
                                                                ENTMGT_NAMESPACE,
                                                                &settings,
                                                                &nbSettings);
        if (returnCode != COM_SUCCESS)
        {
            TRACE_GEN(ERROR, "Getting configuration data from RAW configuration structure failed", __FUNCTION__, __LINE__);
            goto exit;
        }
    }

    /* For each setting */
    if (settings != NULL)
    {
        for (i = 0u; i < nbSettings; i++)
        {
            if (settings[i].key == NULL || settings[i].value == NULL)
            {
                returnCode = COM_ERR_NOT_ALLOWED;
                goto exit;
            }

            if (strcmp(ENTMGT_SETTING_KEY_DEVICE_HOSTNAME, settings[i].key) == 0)
            {
                length = 0u;
                length = strlen(settings[i].value);

                if ((length + 1) < ENTMGT_MAX_SETTING_STR_SIZE)
                {
                    memset(&g_entmgt_device_hostname, 0, ENTMGT_MAX_SETTING_STR_SIZE);
                    memcpy(&g_entmgt_device_hostname, settings[i].value, length);
                }
            }
            else if (strcmp(ENTMGT_SETTING_KEY_DEVICE_NET_IF, settings[i].key) == 0)
            {
                length = 0u;
                length = strlen(settings[i].value);

                if ((length + 1) < ENTMGT_MAX_SETTING_STR_SIZE)
                {
                    memset(&g_entmgt_device_net_if, 0, ENTMGT_MAX_SETTING_STR_SIZE);
                    memcpy(&g_entmgt_device_net_if, settings[i].value, length);
                }
            }
            else if (strcmp(ENTMGT_SETTING_KEY_DEVICE_IP_ADDR, settings[i].key) == 0)
            {
                length = 0u;
                length = strlen(settings[i].value);

                if ((length + 1) < ENTMGT_MAX_SETTING_STR_SIZE)
                {
                    memset(&g_entmgt_device_ip_addr, 0, ENTMGT_MAX_SETTING_STR_SIZE);
                    memcpy(&g_entmgt_device_ip_addr, settings[i].value, length);
                }
            }
            else if (strcmp(ENTMGT_SETTING_KEY_DEVICE_LOCATION, settings[i].key) == 0)
            {
                length = 0u;
                length = strlen(settings[i].value);

                if ((length + 1) < ENTMGT_MAX_SETTING_STR_SIZE)
                {
                    memset(&g_entmgt_device_location, 0, ENTMGT_MAX_SETTING_STR_SIZE);
                    memcpy(&g_entmgt_device_location, settings[i].value, length);
                }
            }
            else if (strcmp(ENTMGT_SETTING_KEY_DEVICE_SYSTEM, settings[i].key) == 0)
            {
                length = 0u;
                length = strlen(settings[i].value);

                if ((length + 1) < ENTMGT_MAX_SETTING_STR_SIZE)
                {
                    memset(&g_entmgt_device_system, 0, ENTMGT_MAX_SETTING_STR_SIZE);
                    memcpy(&g_entmgt_device_system, settings[i].value, length);
                }
            }
            else
            {
                TRACE_GEN(WARN, "Configuration setting is not recognized by this entity", __FUNCTION__, __LINE__);
            }
        }
    }

exit:

    if (returnCode != COM_SUCCESS)
    {

        TRACE_GEN(TRACE, "Error occured in Entity Management for local configuration, some settings value may be invalid.", __FUNCTION__, __LINE__);
        (void) entmgt_resetLocalConfiguration();
    }

    END_FUNCTION();

    return returnCode;
}

/**
 * Configure all registered entities
 * @param rawConfigurationData
 * @return 
 */
static T_com_status entmgt_configureEntities(T_com_entity_launch * launcherList[], size_t launcherListSize, const T_tools_json_value * rawConfigurationData)
{
    size_t i = 0u;
    T_com_status returnCode = COM_SUCCESS;
    char errMsg [ENTMGT_STRING_SIZE] = {0};
    size_t wSize = 0u;

    size_t nbSettings = 0u;
    T_com_entity_setting * settings = NULL;

    START_FUNCTION();

    /* Register entity's local configuration callback */
    for (i = 0u; i < (uint32_t) launcherListSize; i++)
    {
        if (launcherList[i]->localConfigurationCallback != NULL)
        {
            /** 
             * Get settings of the current entity if RAW data are present
             * otherwise took the default value 
             */
            if (rawConfigurationData != NULL)
            {
                returnCode = entmgt_getSettingsFromRawConfigurationData(
                                                                        rawConfigurationData,
                                                                        launcherList[i]->nameSpace,
                                                                        &settings,
                                                                        &nbSettings);
                if (returnCode != COM_SUCCESS)
                {
                    TRACE_GEN(ERROR, "Getting configuration data from raw configuration structure failed", __FUNCTION__, __LINE__);
                    goto exit;
                }
            }

            returnCode = launcherList[i]->localConfigurationCallback(nbSettings, settings);
            if (returnCode != COM_SUCCESS)
            {
                nal_snprintf_s(errMsg, ENTMGT_STRING_SIZE, ENTMGT_STRING_SIZE, &wSize, "Could not configure the %s entity", launcherList[ i ]->nameSpace);
                TRACE_GEN(ERROR, errMsg, __FUNCTION__, __LINE__);
                break;
            }

            /* Clean for the next entity */
            if (settings != NULL)
            {
                NAL_FREE(settings);
                settings = NULL;
                nbSettings = 0;
            }
        }
    }

exit:

    if (settings != NULL)
    {

        NAL_FREE(settings);
        settings = NULL;
        nbSettings = 0;
    }

    END_FUNCTION();

    return returnCode;
}

/*-----------------------------  Public Functions ----------------------------*/

T_com_status entmgt_configure(T_com_entity_launch * launcherList[], size_t launcherListSize)
{
    T_com_status returnCode = COM_SUCCESS;
    char * configurationFilePath = NULL;
    char * configurationFactoryFilePath = NULL;
    char * buffer = NULL;
    size_t bufferSize = 0u;
    T_tools_json_value * rawConfigurationData = NULL;

    START_FUNCTION();

    /* Sanity check */
    VERIF_ARGS("launcherList[]", launcherList != NULL, returnCode, COM_ERR_PARAM);
    if (returnCode != COM_SUCCESS)
    {
        TRACE_GEN(TRACE, "Invalid parameter", __FUNCTION__, __LINE__);
        goto exit;
    }

    /* Get configuration file path */
    if (tools_buildFilePath(entmgt_launcher_getAppRootPath(), ENTMGT_DIR_RUNNING_CONF,
              ENTMGT_FILE_RUNNING_CONF, &configurationFilePath) != TOOLS_SUCCESS) 
    {
        TRACE_GEN(ERROR, "Build local configuration file path failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_INTERNAL;
        goto exit;
    }

    /* Read the file path */
    returnCode = entmgt_readInternalFile(configurationFilePath, &buffer, &bufferSize);
    if (returnCode != COM_SUCCESS)
    {
        TRACE_GEN(WARN, "Reading configuration file failed", __FUNCTION__, __LINE__);

        /** 
         * Configuration file not available in running folder
         * 
         * No exit is normal! In case of failure, default value will be apply
         * in functions: entmgt_configureHimself() and entmgt_configureEntities()
         */

        /* Build factory configuration path */
        if (tools_buildFilePath(entmgt_launcher_getAppRootPath(), ENTMGT_DIR_FACTORY_CONF,
                  ENTMGT_FILE_FACTORY_CONF, &configurationFactoryFilePath) != TOOLS_SUCCESS)
            TRACE_GEN(ERROR, "Build local configuration file path failed", __FUNCTION__, __LINE__);

        /* Copy the factory configuration file to running folder */
        if (tools_copyFile(configurationFactoryFilePath, configurationFilePath) != TOOLS_SUCCESS)
            TRACE_GEN(ERROR, "Coping factory file to running folder failed", __FUNCTION__, __LINE__);

        /* Read again the configuration file into running folder */
        returnCode = entmgt_readInternalFile(configurationFilePath, &buffer, &bufferSize);
        if (returnCode != COM_SUCCESS)
            TRACE_GEN(ERROR, "Reading configuration file after copy from factory failed", __FUNCTION__, __LINE__);

        /* Parse the buffer */
        returnCode = entmgt_parseInternalJsonBuffer(buffer, bufferSize, &rawConfigurationData);
        if (returnCode != COM_SUCCESS)
            TRACE_GEN(WARN, "Parsing local configuration buffer failed", __FUNCTION__, __LINE__);
    }
    else
    {
        /* Parse the buffer */
        returnCode = entmgt_parseInternalJsonBuffer(buffer, bufferSize, &rawConfigurationData);
        if (returnCode != COM_SUCCESS)
        {
            TRACE_GEN(WARN, "Parsing local configuration buffer failed", __FUNCTION__, __LINE__);
        }
    }


    /* Configure the Entity management himself */
    returnCode = entmgt_configureHimself(rawConfigurationData);
    if (returnCode != COM_SUCCESS)
    {
        TRACE_GEN(ERROR, "Configure the Entity management himself failed", __FUNCTION__, __LINE__);
        goto exit;
    }

    /* Configure registered entities */
    returnCode = entmgt_configureEntities(launcherList, launcherListSize, rawConfigurationData);
    if (returnCode != COM_SUCCESS)
    {
        TRACE_GEN(ERROR, "Configure registered entities failed", __FUNCTION__, __LINE__);
        goto exit;
    }

    /* Generate the UUID device */
    returnCode = entmgt_generateDeviceUUID();
    if (returnCode != COM_SUCCESS)
    {
        TRACE_GEN(ERROR, "Device UUID generation failed", __FUNCTION__, __LINE__);
        goto exit;
    }

exit:

    /* Release memory */
    if (configurationFilePath != NULL)
    {
        NAL_FREE(configurationFilePath);
        configurationFilePath = NULL;
    }
    if (configurationFactoryFilePath != NULL)
    {
        NAL_FREE(configurationFactoryFilePath);
        configurationFactoryFilePath = NULL;
    }

    if (buffer != NULL)
    {
        NAL_FREE(buffer);
        buffer = NULL;
    }
    if (rawConfigurationData != NULL)
    {

        tools_json_value_free(rawConfigurationData);
    }

    END_FUNCTION();

    return returnCode;
}

/*--------------------------------- Accessors ---------------------------------*/

const char * entmgt_get_device_uuid(void)
{

    return g_entmgt_device_uuid;
}

const char * entmgt_get_device_hostname(void)
{

    return g_entmgt_device_hostname;
}

const char * entmgt_get_device_net_if(void)
{

    return g_entmgt_device_net_if;
}

const char * entmgt_get_device_ip_addr(void)
{

    return g_entmgt_device_ip_addr;
}

char * entmgt_get_device_location(void)
{
    return g_entmgt_device_location;
}
