/**
 * \file
 * \brief AUTH  Entity management API
 * \details as all, this entity is managed by the Entity Mgt
 * Entity management API is an internal API and is used to initialize, configure, start, and stop the entity.
 * 
 * This entity has no system tool except a mutex, this is like dead code called by other entities with an API.
 * No setting s are managed here.
 *
 * \date 04/2020 NGE Creation
 *
 */

#ifndef C_auth_rbac_mgt_h_included
#define C_auth_rbac_mgt_h_included


/*------------------------------ Definitions -------------------------------*/

/* \brief API given to the "entity launcher", as a list  */
extern T_com_entity_launch authRbacLauncher;

/**
 * \brief entity namespace
 */
#define AUTH_RBAC_NS  "http://www.devsec.com/devsec/auth_rbac"

/* Thread Priorityis medium */
#define MEDIUM_TASK_PRIO 112

#define DEFAULT_AUTH_SESSION_DURATION    20u
#define DEFAULT_AUTH_LOCKACCOUNT_DURATION    60u

/*------------------------------ externs -----------------------------------*/

/* Mutex id's */
extern T_nal_mutex auth_rbac_mutex_id;

/* Thread id's */
extern T_nal_task auth_duration_thread_id;
extern NAL_BOOL   auth_duration_thread_status;


/*------------------------------ Functions -----------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif


/* ----------------- API for the global launcher  --------------- */

/* \brief Get status of Entity */
T_com_status auth_rbac_getStatus
(
    T_com_entity_status * entityStatus
);


/**
 * \brief Initialize Entity and settings to default values, create all system tools like queue, mutex, threads
 * \details 
 * If this function failed, nothing is created and state is STOPPED. Else state is INITIALIZED
 * \return A status code
 */
T_com_status auth_rbac_init
(
    void
);

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
);

/**
 * \brief Give the Dynamic settings to the Entity, 0 or more tuples. Shall happen in CONFIGURED or STARTED mode
 * \details 
 * If this function failed, settings are switched back to their default values and nothing else happen
 * \param[in] nbSettings
 * \param[in] points a table of tuple key/value
 * \return A status code
 */
T_com_status auth_rbac_setSecurityPolicies
(
    size_t nbSettings,
    T_com_entity_setting_ptr settings
);


/**
 * \brief Starts the entity.
 * \details This function starts to work the whole Entity Orchestrator.
 * The entity must be fully initialized and configured before a call to this function.
 * \return A status code
 */
T_com_status auth_rbac_start
(
    void
);

/**
 * \brief Stops the entity.
 * \details The entity must be started before calling this function.
 * This function stops the entity, and destroy or free any resources
 * \return A status code
 */
T_com_status auth_rbac_stop
(
    void
);

/* \brief 2 Setting Accessors to read */
uint32_t get_auth_rbac_session_duration ( void );
uint32_t get_auth_rbac_lockaccount_duration ( void );

#ifdef __cplusplus
}
#endif


#endif /* C_auth_rbac_mgt_h_included */
