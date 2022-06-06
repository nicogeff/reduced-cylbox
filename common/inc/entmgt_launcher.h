#ifndef ENTMGT_LAUNCHER_H
#define ENTMGT_LAUNCHER_H

#ifdef __cplusplus
extern "C"
{
#endif

    
/*------------------------------ Definitions -------------------------------*/

typedef struct entmgt_launcher_entity_status_info_struct
{
    char * namespace;
    T_com_entity_status status;
} T_entmgt_launcher_entity_status_info;

/*------------------------------ Functions -----------------------------------*/

/**
 * \brief Get the status of the application
 * @details This function returns the current status of the application
 * @return An error code
 */
T_com_status entmgt_launcher_getAppStatus(T_com_entity_status * appStatus);

/**
 * \brief Simple accessor to get the root folder path
 * @return The root folder path
 */
const char * entmgt_launcher_getAppRootPath(void);

/**
 * \brief Simple accessor to set the root folder path
 * @return An error code
 */
T_com_status entmgt_launcher_setAppRootPath(const char * rootPath);

/**
 * \brief Get the status of each entity of the application
 * @return An error code
 * @todo Must return a list of entity with them status
 */
T_com_status entmgt_launcher_getEntitiesStatus(
                                               size_t * nbEntities,
                                               T_entmgt_launcher_entity_status_info ** entities); 

/**
 * \brief Initialize application
 * @details This function:
 * @li Register the application entity
 * @li Triggers initialization phase of all entity 
 * @return An error code
 */
T_com_status entmgt_launcher_initialize(void);

/**
 * \brief Configure the application
 * @details This function:
 * @li Parses all variables in configuration files 
 * @li Send variables to entity (configuration phase)
 * @note This function must be called before any entities are started
 * @return An error code
 */
T_com_status entmgt_launcher_configure(void);

/**
 * \brief Start the application
 * @details This function:
 * @li Starts all registered entities
 * @return An error code
 */
T_com_status entmgt_launcher_start(void);

/**
 * \brief Stop the application
 * @details This function:
 * @li Stops all registered entities
 * @return An error code
 */
T_com_status entmgt_launcher_stop(void);

#ifdef __cplusplus
}
#endif

#endif /* ENTMGT_LAUNCHER_H */

