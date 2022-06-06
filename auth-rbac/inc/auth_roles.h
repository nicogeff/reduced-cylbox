/**
 * \file
 * \brief inside AUTH-RBAC in Box project
 *
 * \date 12/01/2019 NGE creation
 */

#ifndef C_auth_rbac_roles_h_included
#define C_auth_rbac_roles_h_included

/*------------------------------ headers included -------------------------------*/
#include "com_error.h"


/*------------------------------ Definitions --------------------------------------*/

/*------------------------------ Structuress --------------------------------------*/

/* ... */

typedef struct T_auth_rbac_role_struct
{
    char  name[AUTH_STRING_SIZE];      // name of role in a a string. Role is unused if name is "" (name[0] == 0)
    T_auth_rbac_authorization_string * pAuthorizations[AUTH_MAX_AUTHORIZATIONS_IN_ROLE];   //array of pointer on authorizations. Pointer is not used if NULL.
}
T_auth_rbac_role_struct,  *T_auth_rbac_role_struct_p;

/* static structure */
EXTERN_AUTH  T_auth_rbac_role_struct  auth_roles[AUTH_MAX_ROLES];



/*------------------------------ Functions -----------------------------------*/

/**
 * \brief Clean and initialize static sessions
 * 
 * Input:
 * 
 * Nothing is malloced
 * returns: T_COM_STATUS, Do nothing if not SUCCESS
 */
T_com_status auth_init_all_roles ();


/**
 * \brief Create one session, with just ID inside.
 * Adresse of string containing Id is returned. Only zeros after.
 * A place is choosen in static structure of sessions (fixed array).
 * If no more place, an error is retuned.
 * 
 * Input: string name of the new role
 * Input: array of authorizations, 1 to max entries in array
 * Input: number of authorizations in previous array, at least one
 * Output: N/A
 * 
 * returns: T_COM_STATUS, Do nothing if not SUCCESS
 */
T_com_status auth_create_role (char * name, T_auth_rbac_authorization_string  * authorizations_array, size_t nb_authorizations);


/**
 * \brief Delete session whom Id is given.
 * RAZ the place in static structure.
 * If Id does not exist, an error is returned
 * 
 * Input: adresse of a string Id that is NOT freed iside.
 * Output: N/A
 * 
 * returns: T_COM_STATUS, Do nothing if not SUCCESS
 */
T_com_status auth_delete_role (char * id);


/**
 * \brief give address of a role element
 * 
 * Input: adresse of a string "name of role" that is NOT freed iside.
 * Output: adress of user element
 * 
 * This is useful to build Rbac tables with real pointer, giving maximum efficience when parsing
 * returns: T_COM_STATUS if success, Do nothing if not SUCCESS
 * returns error if name is not found, or internal problem
 */
T_com_status auth_role_getAddress (char * name, T_auth_rbac_role_struct ** ppRole);


/**
 * \brief suppress an authorization pointer, in all roles where it appears (0 or N times)
 * 
 * Input: adresse of an authorization.
 * Output: N/A
 * 
 * returns: T_COM_STATUS since it always succed.
 */
T_com_status auth_delete_authorization_from_all_roles (T_auth_rbac_authorization_string  pAuthorization);


#ifdef __cplusplus
extern "C"
{
#endif




#ifdef __cplusplus
}
#endif


#endif /* C_auth_rbac_roles_h_include */
