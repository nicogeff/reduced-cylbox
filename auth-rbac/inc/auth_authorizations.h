/**
 * \file
 * \brief inside AUTH-RBAC in Box project
 *
 * \date 12/01/2019 NGE creation
 */

#ifndef C_auth_rbac_authorizations_h_included
#define C_auth_rbac_authorizations_h_included

/*------------------------------ headers included -------------------------------*/
#include "com_error.h"


/*------------------------------ Definitions --------------------------------------*/
// #define  AUTH_MAX_AUTHORIZATIONS  256

/*------------------------------ Structuress --------------------------------------*/

/* ... */

typedef char T_auth_rbac_authorization_string[AUTH_STRING_SIZE];    // type of one authorization, empty if first char is Null otherwise last char is Null (fixed size)

EXTERN_AUTH T_auth_rbac_authorization_string  authorizations[AUTH_MAX_AUTHORIZATIONS];      // complex array of authorizations






/*------------------------------ Functions -----------------------------------*/

/**
 * \brief Clean and initialize static array of authorizations
 * 
 * Input:
 * 
 * Nothing is malloced
 * returns: T_COM_STATUS, Do nothing if not SUCCESS
 */
T_com_status auth_init_all_authorizations ();


/**
 * \brief Create one authorization and put its name in array.
 * A place is choosen in static structure of sessions (fixed array), error if no more place.
 * 
 * Input: name in a string, length is between 1 and (max - 1)
 * Output: N/A
 * 
 * Nothing is malloced inside this function
 * returns: T_COM_STATUS, Do nothing if not SUCCESS.
 *               Error COM_ERR_MAX_ELEMENTS if no more place
 */
T_com_status auth_create_authorization (char * name);


/**
 * \brief Delete authorizations whom name is given.
 * RAZ the place in static structure.
 * If name does not exist, error COM_ERR_UNKNOWN_ELEMENT is returned
 * Delete this authorization from all roles where it appears
 * 
 * Input: adresse of string name that is NOT freed iside.
 * Output: N/A
 * 
 * returns: T_COM_STATUS, Do nothing if not SUCCESS
 */
T_com_status auth_delete_authorization (char * name);


/**
 * \brief give address of an authorization element           =====>       NOT USED 
 * 
 * Input: adresse of a string "name of authorization" that is NOT freed iside.
 * Output: adress of authorization element
 * 
 * This is useful to build Rbac tables with real pointer, giving maximum efficience when parsing
 * returns: T_COM_STATUS if success, Do nothing if not SUCCESS
 * returns error if name is not found, or internal problem
 */
/*  T_com_status auth_authorization_getAddress (char * name, T_auth_rbac_authorization_string * pAuthorization);      =====>    NOT USED   */



#ifdef __cplusplus
extern "C"
{
#endif




#ifdef __cplusplus
}
#endif


#endif /* C_auth_rbac_authorizations_h_include */
