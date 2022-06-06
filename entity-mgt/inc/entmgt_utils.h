#ifndef ENTMGT_UTILS_H
#define ENTMGT_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------ Include files -------------------------------*/
/*------------------------------ Pragma --------------------------------------*/
/*------------------------------ Macros --------------------------------------*/
/*------------------------------ Structures ----------------------------------*/
/*------------------------------ Constants -----------------------------------*/

/* Default string size*/
#define ENTMGT_STRING_SIZE    256u
    
/* Module management namespace */
#define ENTMGT_NAMESPACE "http://www.devsec.com/devsec/entity-entity-mgt"

/* The maximum number of entity in configuration files */
#define ENTMGT_MAX_ENTITIES 256u

/* The maximum size of string */
#define ENTMGT_MAX_SETTING_STR_SIZE 512u

/** 
 * Folder management
 */
    
/** 
 * \brief This value indicate the maximum string size we are able to
 * manage for the complete path to the root folder
 */
#define ENTMGT_ROOT_PATH_MAX_SIZE ENTMGT_STRING_SIZE

/**
 * \brief This value indicate the root path name
 * \note This value can be different according the configuration file
 */    
#define ENTMGT_DIR_ROOT_PATH_NAME  ".box/"    

#define ENTMGT_DIR_FACTORY_CONF  "factory/conf/"
#define ENTMGT_DIR_RUNNING_CONF  "running/conf/"
#define ENTMGT_DIR_WWW  "www/"

#define ENTMGT_DIR_FACTORY_RBAC  "factory/rbac/"
#define ENTMGT_DIR_RUNNING_RBAC  "running/rbac/"

#define ENTMGT_FILE_FACTORY_CONF  "conf.json"
#define ENTMGT_FILE_RUNNING_CONF  "conf.json"

#define ENTMGT_FILE_FACTORY_RBAC  "rbac.json"
#define ENTMGT_FILE_RUNNING_RBAC  "rbac.json"

#define ENTMGT_FILE_WWW  "index.html"

/*------------------------------ Types ---------------------------------------*/
/*------------------------------ Functions -----------------------------------*/

/**
 * Read an internal file
 * @param internalFilePath
 * @param buffer
 * @param bufferSize
 * @return 
 */
T_com_status entmgt_readInternalFile(
    const char *internalFilePath, 
    char **buffer, 
    size_t *bufferSize
);

/**
 * Write an internal file
 * @param internalFilePath
 * @param buffer
 * @param bufferSize
 * @return 
 */
T_com_status entmgt_writeInternalFile(
    const char *internalFilePath, 
    const char *buffer, 
    const size_t bufferSize
);

/**
 * Parse an internal JSON buffer
 * @param buffer
 * @param bufferSize
 * @param rawJsonData
 * @return 
 */
T_com_status entmgt_parseInternalJsonBuffer(
    const char *buffer,
    size_t bufferSize, 
    T_tools_json_value **rawJsonData
);

/**
 * Get settings from a raw configuration data 
 * @param[in] rawConfigurationData Raw parsing data
 * @param[in] namespace Namespace of the entity
 * @param[out] settings  Settings of the entity (allocated)
 * @param[out] nbSettings Number of settings in the structure settings
 * @warning The caller must call al_free() to deallocate memory of the parameter settings
 * @return 
 */
T_com_status entmgt_getSettingsFromRawConfigurationData(
    const T_tools_json_value * rawConfigurationData, 
    const char * namespace, 
    T_com_entity_setting ** settings, 
    size_t *nbSettings
);

/**
 * This function returns the running security model file path
 * @warning The function allocate filePath. Release memory must be done by the caller
 * @param[out] filePath Will contains the full file path to the security model running
 * @return A T_com_status error code
 */
T_com_status entmgt_getRunningSecurityModelFilePath(char **filePath);


/**
 * This function returns the factory model file path
 * @warning The function allocate filePath. Release memory must be done by the caller
 * @param[out] filePath Will contains the full file path to the security model factory
 * @return A T_com_status error code
 */
T_com_status entmgt_getFactorySecurityModelFilePath(char **filePath);
     
#ifdef __cplusplus
}
#endif

#endif /* ENTMGT_UTILS_H */

