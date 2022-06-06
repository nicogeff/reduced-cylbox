#ifndef ENTMGT_CONFIGURE_H
#define ENTMGT_CONFIGURE_H

/*------------------------------ Include files -------------------------------*/
/*------------------------------ Pragma --------------------------------------*/
/*------------------------------ Macros --------------------------------------*/
/*------------------------------ Structures ----------------------------------*/
/*------------------------------ Constants -----------------------------------*/

/* Key*/
#define ENTMGT_SETTING_KEY_DEVICE_HOSTNAME "ENTMGT_DEVICE_HOSTNAME"
#define ENTMGT_SETTING_KEY_DEVICE_NET_IF "ENTMGT_DEVICE_NET_IF"
#define ENTMGT_SETTING_KEY_DEVICE_IP_ADDR "ENTMGT_DEVICE_IP_ADDR"
#define ENTMGT_SETTING_KEY_DEVICE_SYSTEM "ENTMGT_DEVICE_SYSTEM"
#define ENTMGT_SETTING_KEY_DEVICE_LOCATION "ENTMGT_DEVICE_LOCATION"

#define ENTMGT_SETTING_VALUE_DFLT_DEVICE_HOSTNAME "default"
#define ENTMGT_SETTING_VALUE_DFLT_DEVICE_NET_IF "n/a"
#define ENTMGT_SETTING_VALUE_DFLT_DEVICE_IP_ADDR "n/a"
#define ENTMGT_SETTING_VALUE_DFLT_DEVICE_SYSTEM "n/a"
#define ENTMGT_SETTING_VALUE_DFLT_DEVICE_LOCATION "n/a"

/* UUID fixed values "les 2 gars" */
#define ENTMGT_UUID_FIXED_VALUE_0 "6c657320"
#define ENTMGT_UUID_FIXED_VALUE_1 "3220"
#define ENTMGT_UUID_FIXED_VALUE_2 "6761"
#define ENTMGT_UUID_FIXED_VALUE_3 "7273"

#define ENTMGT_UUID_FIXED_VALUE_NO_MAC "ffffff"

/*------------------------------ Types ---------------------------------------*/
/*------------------------------ Functions -----------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @brief Configuration function for entity management
     * @details Used to push into entity management all configuration values 
     * defined in JSON files.
     * @param[in] nbSettings Number of configuration variable (can be 0)
     * @param[in] settings Name/Value pair of configuration variables 
     * (can be NULL)
     * @return An error code
     */
    T_com_status entmgt_configure(
            T_com_entity_launch * launcherList[],
            size_t launcherListSize);


/*------------------------------ Accessors -----------------------------------*/

    /**
     * @brief Accessor to get the device UUID
     * @return Device UUID
     */
    const char * entmgt_get_device_uuid(void);
    
    /**
     * @brief Accessor to get the device hostname
     * @return Device hostname
     */
    const char * entmgt_get_device_hostname(void);
    
    /**
     * @brief Accessor to get the device network interface name
     * @return Network interface name
     */
    const char * entmgt_get_device_net_if(void);
    
    /**
     * @brief Accessor to get the device IP address
     * @return IP address
     */
    const char * entmgt_get_device_ip_addr(void);
    
    /**
     * @brief Accessor to get address of setting: device_location that represent
     * place to secure
     * @return  Device location
     */
    char * entmgt_get_device_location(void);

#ifdef __cplusplus
}
#endif

#endif /* ENTMGT_CONFIGURE_H */

