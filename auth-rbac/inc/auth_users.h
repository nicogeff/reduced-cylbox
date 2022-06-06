/**
 * \file
 * \brief inside AUTH-RBAC in Box project
 *
 * \date 12/01/2019 NGE creation
 */

#ifndef C_auth_rbac_users_h_included
#define C_auth_rbac_users_h_included

/*------------------------------ headers included -------------------------------*/
#include "com_error.h"


/*------------------------------ Definitions --------------------------------------*/


/*------------------------------ Structures --------------------------------------*/

/* ... */

typedef struct T_auth_rbac_user_struct
{
    char   user[AUTH_STRING_SIZE+1];                    /* user name */
    char  hash_password[AUTH_SHA256_SIZE+1];     /* sha256 algo */
    T_auth_rbac_role_struct * pRole1;              /* first role */
    T_auth_rbac_role_struct * pRole2;              /* second role, authorizations are cumulated */
    bool  isLocked;                    /*true if account has been locked and is still */
    uint32_t  ttl_locked;        /*remaining time in second, to be locked, if occuring */
    size_t  nb_auth_failed;    /*remaining time in second, to be locked, if occuring */
}
T_auth_rbac_user_struct,  *T_auth_rbac_user_struct_p;

/* static structure */
EXTERN_AUTH  T_auth_rbac_user_struct  auth_users[AUTH_MAX_USERS];



/*------------------------------ Functions -----------------------------------*/

/**
 * \brief Clean and initialize users in array
 * 
 * Input:
 * 
 * Nothing is malloced
 * returns: T_COM_STATUS, Do nothing if not SUCCESS
 */
T_com_status auth_init_all_users ();


/**
 * \brief Create one user, with password and one role at least.
 * A place is choosen in fixed array, If no more place, an error is retuned.
 * 
 * Input: string with name
 * Input: string with clear password (immediatly hashed and clean from memory). Shall not be NULL
 * Input: string with first role
 * Input: string with eventual second role
 * Output: N/A
 * 
 * String with Id is malloced inside
 * returns: T_COM_STATUS, Do nothing if not SUCCESS
 */
T_com_status auth_create_user (char * name, char * password, T_auth_rbac_role_struct * role1, T_auth_rbac_role_struct * role2);


/**
 * \brief Delete user whose name is given.
 * RAZ the place in static structure and delete possible session using this user.
 * If name does not exist, an error is returned
 * 
 * Input: adresse of a string Id that is NOT freed iside.
 * Output: N/A
 * 
 * returns: T_COM_STATUS, Do nothing if not SUCCESS
 */
T_com_status auth_delete_user (char * id);


/**
 * \brief give address of a user element
 * 
 * Input: adresse of a string "name of user" that is NOT freed iside.
 * Output: adress of user element
 * 
 * This is useful to build Rbac tables with real pointer, giving maximum efficience when parsing
 * returns: T_COM_STATUS if success, Do nothing if not SUCCESS
 * returns error if name is not found, or internal problem
 */
T_com_status auth_user_getAddress (char * name, T_auth_rbac_user_struct ** ppUser);


/**
 * \brief check for authentication of a user
 * 
 * Input: adresse of a string "name of user" that is NOT freed iside.
 * Input: adresse of a string " clear password" that is NOT freed iside.
 * Output: authentication with password is a success, as a boolean
 * 
 * This is useful when session is authenticating.
 * returns: T_COM_STATUS if no internal error (does not authentication is granted), Do nothing if not SUCCESS
 * returns error if name is not a user, or internal problem
 */
T_com_status auth_user_check_for_authent (char * name, char * password, bool * granted);


/**
 * \brief check for  authorization on a user (used by authenticated session), no matter here of authentication
 * 
 * Input: adresse of a string "name of user" that is NOT freed iside.
 * Input: adresse of a string "name of authorization" that is NOT freed iside.
 * Output: authorization is granted, as a boolean
 * 
 * This is useful to avoid session step, if authentication is not saved as a cookie
 * returns: T_COM_STATUS if no internal error (does not mean authoriz is granted), Do nothing if not SUCCESS
 * returns error if name is not found, or internal problem
 */
T_com_status auth_user_check_for_authoriz (char * name, char * authorization, bool * granted);


/**
 * \brief check for authentication AND authorization on a user (direct access w/o session)
 * 
 * Input: adresse of a string "name of user" that is NOT freed iside.
 * Input: adresse of a string " clear password" that is NOT freed iside.
 * Input: adresse of a string "name of authorization" that is NOT freed iside.
 * Output: couple (authentication/authorization) is a success as a boolean
 * 
 * This is useful to avoid session step, if authentication is not saved as a cookie
 * returns: T_COM_STATUS if no internal error (does not mean authoriz is granted), Do nothing if not SUCCESS
 * returns error if name is not found, or internal problem
 */
T_com_status auth_user_check_for_authent_authoriz (char * name, char * password, char * authorization, bool * granted);


#ifdef __cplusplus
extern "C"
{
#endif




#ifdef __cplusplus
}
#endif


#endif /* C_auth_rbac_users_h_include */
