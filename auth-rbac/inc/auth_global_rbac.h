/**
 * \file
 * \brief inside AUTH-RBAC in Box project
 *
 * \date 12/01/2019 NGE creation
 */

#ifndef  C_auth_rbac_global_rbac_h_included
#define C_auth_rbac_global_rbac_h_included


/*------------------------------ Definitions --------------------------------------*/
#define  AUTH_MAX_SESSIONS  32
#define  AUTH_MAX_USERS        32
#define  AUTH_MAX_ROLES        32
#define  AUTH_MAX_AUTHORIZATIONS_IN_ROLE       32
#define  AUTH_MAX_AUTHORIZATIONS  256
#define  AUTH_USER_AUTH_FAILED_MAX        3    /* before locking */

#define  AUTH_RBAC_FILEPATH_LEN_MAX      64    /* given path max length, relative or absolute */


/*------------------------------ headers included -------------------------------*/
#include "com_error.h"
#include "auth_mgt.h"
#include "auth_authorizations.h"
#include "auth_roles.h"
#include "auth_users.h"
#include "auth_sessions.h"

/*------------------------------ Structuress --------------------------------------*/

/* ... */


/*------------------------------ Functions -----------------------------------*/

/**
 * \brief Clean and initialize global rbac with 3 factory rbac files
 *  If factory tables are unconsistent, all tables are let empty
 * 
 * Input: N/A
 * Output: N/A
 * 
 * Nothing is malloced
 * returns: T_COM_STATUS, Do nothing if not SUCCESS
 */
T_com_status auth_initialize_rbac ();


/**
 * \brief re-Buid complete rbac, given a json buffer (coming from a web-service for example).
 *   Write directly in Ram structures figuring current Rbac. Radical action w/o mutex.
 *  
 * Input: pointer on the json buffer, containing authorizations, roles and users. Length is not useful
 * Output: N/A
 * 
 * If buffer gives an unconsistent rbac, nothing is done and and an error is returned
 * Nothing is malloced or freed inside
 * returns: T_COM_STATUS, Do nothing if not SUCCESS
 */
/* 
 * Exported in common header: T_com_status auth_loadJsonBuffer_in_rbac_ram (void * p_json_buffer);
 */


/**
 * \brief Core of thread in AUTH_RBAC: Handles all time to live in Rbac structure, like session duration and temporary locked account
 * \details This function enter an infinite loop which basically:
 * \li wake up each 10 second
 * \li take or wait for the mutex
 * \li decrement time to live for session and locking status of an account
 * \li Take decision if a time to live as reach zero
 * \li give back the mutex
 * \li Sleep again, to wake up lately
 * 
 * PARAM data: not used
 * \return void
 */
void auth_duration_thread ( void *data );

#ifdef __cplusplus
extern "C"
{
#endif




#ifdef __cplusplus
}
#endif


#endif /* C_auth_rbac_global_rbac_h_include */
