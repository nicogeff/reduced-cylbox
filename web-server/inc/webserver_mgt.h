#ifndef WEBSERVER_H
#define WEBSERVER_H

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * \brief API given to the launcher, in a list  
 */
extern T_com_entity_launch webserverLauncher;    
    
/**
 * \brief Namespace of Web Server Entity
 */
#define WEBSERVER_ENTITY_NS "http://www.devsec.com/devsec/WEBSERVER_Module"

    
/**
 * \brief Setting names
 */
#define WEBSERVER_CONF_WWW_ROOT_DIR_NAME        "WEBSERVER_CONF_WWW_ROOT_DIR"
#define WEBSERVER_CONF_WWW_PORT_NAME            "WEBSERVER_CONF_WWW_PORT"
#define WEBSERVER_CONF_WWW_MAX_CONNECTION_NAME  "WEBSERVER_CONF_WWW_MAX_CONNECTION"

/**
 * \brief Setting default values
 */    
#define DEFAULT_WEBSERVER_CONF_WWW_ROOT_DIR             "www/"
#define DEFAULT_WEBSERVER_CONF_WWW_PORT                 "9443"
#define DEFAULT_WEBSERVER_CONF_WWW_MAX_CONNECTION       1000

#define DEFAULT_WEBSERVER_CONF_MCAST_GROUP              "239.255.255.250"   /* This group is compliant with SSDP group standard */
#define DEFAULT_WEBSERVER_CONF_MCAST_PORT               "1900"              /* This port is compliant with SSDP port standard */
#define DEFAULT_WEBSERVER_CONF_MCAST_MAX_CONNECTION     100

/* \brief Thread definitions */
#define WEBSERVER_WWW_THREAD_NAME           "WebServerWwwThread"
#define WEBSERVER_WWW_THREAD_PRIORITY       100  /* int prio : 0 = max , 255 = min */
#define WEBSERVER_WWW_THREAD_STACK_SIZE     (PTHREAD_STACK_MIN * 2)

#define WEBSERVER_MCAST_THREAD_NAME           "WebServerMcastThread"
#define WEBSERVER_MCAST_THREAD_PRIORITY       101  /* int prio : 0 = max , 255 = min */
#define WEBSERVER_MCAST_THREAD_STACK_SIZE     (PTHREAD_STACK_MIN * 6)
    
extern T_nal_task g_webserver_www_thread_id;
extern NAL_BOOL g_webserver_www_thread_status;

extern T_nal_task g_webserver_mcast_thread_id;
extern NAL_BOOL g_webserver_mcast_thread_status;

/*-------------------------- Accessor Functions  -----------------------------*/    
    
char * get_WEBSERVER_CONF_WWW_ROOT_DIR (void);

char * get_WEBSERVER_CONF_WWW_PORT (void);

uint32_t get_WEBSERVER_CONF_WWW_MAX_CONNECTION (void);

char * get_WEBSERVER_CONF_MCAST_GROUP (void);

char * get_WEBSERVER_CONF_MCAST_PORT (void);

uint32_t get_WEBSERVER_CONF_MCAST_MAX_CONNECTION (void);
    
/*------------------------ Launcher API Functions  ---------------------------*/

T_com_status webserver_init
(
    void
);

T_com_status webserver_configure(
    size_t nbSettings,
    T_com_entity_setting_ptr settings
);

T_com_status webserver_start
(
    void
);

T_com_status webserver_stop
(
 void
);
    
#ifdef __cplusplus
}
#endif

#endif /* WEBSERVER_H */

