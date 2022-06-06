/*
 * \file com_entity.h
 * \brief Public API of Entity management
 * \specifics API to describe all functions and type accessible to the other entity
 * \details.  Life cycle is:  STOPPED / INITIALIZED / CONFIGURED / STARTED / STOPPED / INITIALIZED / CONFIGURED / STARTED / STOPPED ...
 * 
 */

#ifndef COM_ENTITY_H
#define COM_ENTITY_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------ Definitions -------------------------------*/

/** 
 * Indicate the crypto library that is linked with the application.
 * @note  this definition should be in in "target def" header located in "common"
 * It is managed in crypto-al entity.
 * Only one shall be un-commented
 */

//#define CRYPTO_OPENSSL
#define CRYPTO_MBEDTLS


    /**
     * \brief All possible states of a Software entity
     */
    typedef enum com_entity_status_enum {
        /* SW Entity status */
        COM_ENTITY_UNKNOWN = 0,             /* The entity status is "unknown" */
        COM_ENTITY_IS_INITIALIZED = 1,      /* The entity status is "initialized" */
        COM_ENTITY_IS_CONFIGURED  = 2,      /* The entity status is "configured" */
        COM_ENTITY_IS_STARTED = 3,          /* The entity status is "started" */
        COM_ENTITY_IS_STOPPED = 4,          /* The entity status is "stopped". It is too the INITIAL state in the life os a software entity */
    } T_com_entity_status;

    /**
     * \brief Structure with conf
     */
    typedef struct com_entity_setting_struct {
        char* key;
        char* value;
    } T_com_entity_setting, *T_com_entity_setting_ptr;


    extern const char * entmgt_launcher_getAppRootPath(void);

    /*------------------------------ Functions pointers -------------------------------*/

    /**
     * \brief Callback type to get the status of all entities
     * \specifics This type describes the kind of function entities should implement to
     * return the status of the entity
     */
    typedef T_com_status(*T_com_entity_getStatusCallback)
    (
            T_com_entity_status * entityStatus
            );

    /**
     * \brief Initialization Callback type
     * \specifics This type describes the kind of function entities should implement for initialization phase.
     */
    typedef T_com_status(*T_com_entity_initializeCallback)
    (
            void
    );

    /**
     * \brief Local configuration callback type
     * \specifics  This type describes the kind of function entities should implement for local configuration and security settings phase.
     */
    typedef T_com_status(*T_com_entity_configurationCallback)
    (
            size_t nbSettings,
            T_com_entity_setting_ptr settings
    );

    /**
     * \brief start Callback type
     * \specifics This type describes the kind of function entities should implement for start phase.
     */
    typedef T_com_status(*T_com_entity_startCallback)
    (
            void
    );

    /**
     * \brief Stop Callback type
     * \specifics This type describes the kind of function entities should implement for stop phase.
     */
    typedef T_com_status(*T_com_entity_stopCallback)
    (
            void
    );

    /**
     * \brief Back to factory Callback type
     * \specifics This type describes the kind of function entities should implement for back to factory phase.
     */
    typedef T_com_status(*T_com_entity_backToFactory)
    (
            void
    );

    /**
     * \brief Structure used to store all callbacks function of a entity
     * \specifics This objects are used by the launcher API to automatically launch all the registered entities.
     */
    typedef struct com_entity_launch_struct {
        char * nameSpace;
        T_com_entity_getStatusCallback getStatusCallback;
        T_com_entity_initializeCallback initializeCallback;
        T_com_entity_configurationCallback localConfigurationCallback;
        T_com_entity_configurationCallback dynamicConfigurationCallback;
        T_com_entity_startCallback startCallback;
        T_com_entity_stopCallback stopCallback;
        T_com_entity_backToFactory backToFactoryCallback;
    } T_com_entity_launch, *T_com_entity_launch_ptr;
    


/*------------------------------ Functions -----------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* COM_ENTITY_H */

