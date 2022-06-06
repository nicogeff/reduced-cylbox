/**
 * \file
 * \brief inside AUTH-RBAC in Box project
 *
 * \date 12/01/2019 NGE creation
 */


/*------------------------------ Definitions --------------------------------------*/
/* This source file DO NOT create Rbac tables: */
#define EXTERN_AUTH extern


/*------------------------------ headers included -------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "com_error.h"

#include "nal_common.h"
#include "nal_rtos_posix.h"
#include "nal_rtos.h"

#include "com_error.h"
#include "com_entity.h"
#include "tools_trace.h"
#include "tools_parserJson.h"

#include "com_auth_rbac_api.h"

#include "auth_global_rbac.h"



/*------------------------------ Structures --------------------------------------*/



/*------------------------------ Functions -----------------------------------*/

/**
 * \brief Clean and initialize users in array
 * 
 * Input:
 * 
 * Nothing is malloced
 * Do NOT take mutex here because it's done by upper "fonction auth_initialize_rbac()"
 * 
 *  returns: T_COM_STATUS, Do nothing if not SUCCESS
 */
T_com_status auth_init_all_users ()
{
    T_com_status  returnCode = COM_SUCCESS;
    /*uint32_t ii = 0u; */
    /*bool found = false; */
    /*char string[128] = ""; */

    START_FUNCTION();

/*    VERIF_ARGS ("name pointer shall not be Null", name != NULL, returnCode, COM_ERR_PARAM);
 *    VERIF_ARGS ("name shall have length not null", strlen(name) > 0, returnCode, COM_ERR_PARAM);
 *    VERIF_ARGS ("name shall have length < MAX", strlen(name) < AUTH_STRING_SIZE, returnCode, COM_ERR_PARAM);
 *    if (returnCode == COM_ERR_PARAM)
 *    {
 *        goto exit;
 *    }
 */
    memset ( &auth_users, 0, sizeof(auth_users) );

    /*exit: */
    END_FUNCTION();
    return returnCode;
}


/**
 * \brief Create one user, with password and one role at least.
 * A place is choosen in fixed array, If no more place, an error is retuned.
 * 
 * Input: string with name
 * Input: string with hashed password (immediatly cleaned from memory when used). Shall not be NULL
 * Input: adress of first role
 * Input: adress of optional second role
 * Output: N/A
 * 
 * Nothing is malloced inside
 * returns: T_COM_STATUS, Do nothing if not SUCCESS
 */
T_com_status auth_create_user (char * name, char * password, T_auth_rbac_role_struct * role1, T_auth_rbac_role_struct * role2)
{
    T_com_status  returnCode = COM_SUCCESS;
    uint32_t ii = 0u;
    size_t index = 0u;
    bool found = false;
    /*char string[128] = ""; */

    START_FUNCTION();

    VERIF_ARGS ("name pointer shall not be Null", name != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("name shall have length not null", strlen(name) > 0, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("name shall have length < MAX", strlen(name) < AUTH_STRING_SIZE, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("name password shall not be Null", password != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("password shall have length not null", strlen(password) > 0, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("password shall have length of SHA256", strlen(password) == AUTH_SHA256_SIZE, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("role1 shall be not Null", role1 != NULL, returnCode, COM_ERR_PARAM);   /*role2 is optional*/
    VERIF_ARGS ("role1 and role2 shall be different", role1 != role2, returnCode, COM_ERR_PARAM);
    if (returnCode == COM_ERR_PARAM)
    {
        goto exit;
    }

    /*find empty place*/
    for (ii=0; (ii<AUTH_MAX_USERS) && (false==found); ii++)
    {
        if (0 == auth_users[ii].user[0] )
        {
            index = ii;
            found = true;
        }
    }
    if (false == found)    /*no more place, error*/
    {
        TRACE_GEN (WARN, "    AUTH-RBAC users array is full", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_MAX_ELEMENTS;
        goto exit;
    }

    memcpy (auth_users[index].user, name, 1+strlen(name));    /*put role name in it, with trailing 0*/
    memcpy (auth_users[index].hash_password, password, 1+strlen(password));    /*put role name in it, with trailing 0*/
    memcpy (auth_users[index].user, name, 1+strlen(name));    /*put role name in it, with trailing 0*/
    auth_users[index].pRole1 = role1;    /*put role1 adress in it*/
    auth_users[index].pRole2 = role2;    /*may be null*/
    
    exit:
    END_FUNCTION();
    return returnCode;
}


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
T_com_status auth_delete_user (char * name)
{
    T_com_status  returnCode = COM_SUCCESS;
    /*uint32_t ii = 0u; */
    /*bool found = false; */
    /*char string[128] = ""; */

    START_FUNCTION();

    VERIF_ARGS ("name pointer shall not be Null", name != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("name shall have length not null", strlen(name) > 0, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("name shall have length < MAX", strlen(name) < AUTH_STRING_SIZE, returnCode, COM_ERR_PARAM);
    if (returnCode == COM_ERR_PARAM)
    {
        goto exit;
    }

    /**  TODO  **/
    
    exit:
    END_FUNCTION();
    return returnCode;
}



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
T_com_status auth_user_getAddress (char * name, T_auth_rbac_user_struct ** ppUser)
{
    T_com_status  returnCode = COM_SUCCESS;
    uint32_t ii = 0u;
    size_t index = 0u;
    bool found = false;
    /*char string[128] = ""; */

    START_FUNCTION();

    VERIF_ARGS ("name pointer shall not be Null", name != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("name shall have length not null", strlen(name) > 0, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("name shall have length < MAX", strlen(name) < AUTH_STRING_SIZE, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("ppUser pointer shall not be Null", ppUser != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("*ppUser pointer shall be Null", *ppUser == NULL, returnCode, COM_ERR_PARAM);
    if (returnCode == COM_ERR_PARAM)
    {
        goto exit;
    }

    for (ii=0; (ii<AUTH_MAX_USERS) && (false == found); ii++)   /*search user*/
    {
            if (0 == strncmp(auth_users[ii].user, name, AUTH_STRING_SIZE))
            {
                index = ii;
                found = true;
            }
    }

    if (false == found)
    {
        TRACE_GEN (WARN, "    AUTH-RBAC user name has not been found in user_getAddress", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_UNKNOWN_ELEMENT;
        goto exit;
    }
        
    * ppUser = & auth_users[index];   /* output address */
    
exit:
    END_FUNCTION();
    return returnCode;
}


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
T_com_status auth_user_check_for_authent (char * name, char * password, bool * granted)
{
    T_com_status  returnCode = COM_SUCCESS;
    uint32_t ii = 0u;
    size_t index = 0u;
    bool found = false;
    /*char string[128] = ""; */

    START_FUNCTION();

    VERIF_ARGS ("name pointer shall not be Null", name != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("name shall have length not null", strlen(name) > 0, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("name shall have length < MAX", strlen(name) < AUTH_STRING_SIZE, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("password pointer shall not be Null", password != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("password shall have length not null", strlen(password) > 0, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("password shall have length of SHA256", strlen(password) == AUTH_SHA256_SIZE, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("granted pointer shall not be Null", granted != NULL, returnCode, COM_ERR_PARAM);
    if (returnCode == COM_ERR_PARAM)
    {
        goto exit;
    }

    for (ii=0; (ii<AUTH_MAX_USERS) && (false == found); ii++)   /*search user*/
    {
            if (0 == strncmp(auth_users[ii].user, name, AUTH_STRING_SIZE))
            {
                index = ii;
                found = true;
            }
    }
    if (false == found)
    {
        TRACE_GEN (WARN, "    AUTH-RBAC user name has not been found in user_check_for_authent", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_UNKNOWN_ELEMENT;
        goto exit;
    }

    * granted = false;
    if  (0 == strncmp(auth_users[index].hash_password, password, AUTH_SHA256_SIZE) )  /*authentication is a success*/
        * granted = true;

exit:
    END_FUNCTION();
    return returnCode;
}



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
T_com_status auth_user_check_for_authoriz (char * name, char * authorization, bool * granted)
{
    T_com_status  returnCode = COM_SUCCESS;
    uint32_t ii = 0u;
    size_t index = 0u;
    uint32_t jj = 0u;
    bool found = false;
    /*char string[128] = ""; */

    START_FUNCTION();

    VERIF_ARGS ("name pointer shall not be Null", name != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("name shall have length not null", strlen(name) > 0, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("name shall have length < MAX", strlen(name) < AUTH_STRING_SIZE, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("authorization pointer shall not be Null", authorization != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("authorization shall have length not null", strlen(authorization) > 0, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("authorization shall have length < MAX", strlen(authorization) < AUTH_STRING_SIZE, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("granted pointer shall not be Null", granted != NULL, returnCode, COM_ERR_PARAM);
    if (returnCode == COM_ERR_PARAM)
    {
        goto exit;
    }

    for (ii=0; (ii<AUTH_MAX_USERS) && (false == found); ii++)   /*search user*/
    {
            if (0 == strncmp(auth_users[ii].user, name, AUTH_STRING_SIZE))
            {
                index = ii;
                found = true;
            }
    }
    if (false == found)
    {
        TRACE_GEN (WARN, "    AUTH-RBAC user name has not been found in user_check_for_authoriz", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_UNKNOWN_ELEMENT;
        goto exit;
    }

    found = false;
    for (jj=0; (jj<AUTH_MAX_AUTHORIZATIONS_IN_ROLE) && (false == found) ; jj++)
    {
      if (NULL != auth_users[index].pRole1)
            if (NULL != auth_users[index].pRole1->pAuthorizations[jj])
                if (0 == strncmp( * (auth_users[index].pRole1->pAuthorizations[jj]), authorization, AUTH_STRING_SIZE))
                    found = true;
        if (NULL != auth_users[index].pRole2)
            if (NULL != auth_users[index].pRole2->pAuthorizations[jj])
                if (0 == strncmp( * (auth_users[index].pRole2->pAuthorizations[jj]), authorization, AUTH_STRING_SIZE))
                    found = true;
    }

    * granted = false;
    if (true == found)
        * granted = true;   /* authorization has been found, Ok */
      
        
exit:
    END_FUNCTION();
    return returnCode;
}



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
T_com_status auth_user_check_for_authent_authoriz (char * name, char * password, char * authorization, bool * granted)
{
    T_com_status  returnCode = COM_SUCCESS;
    uint32_t ii = 0u;
    size_t index = 0u;
    uint32_t jj = 0u;
    bool found = false;
    bool authenticated = false;
    bool authorized = false;
    /*char string[128] = ""; */

    START_FUNCTION();

    VERIF_ARGS ("name pointer shall not be Null", name != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("name shall have length not null", strlen(name) > 0, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("name shall have length < MAX", strlen(name) < AUTH_STRING_SIZE, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("password pointer shall not be Null", password != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("password shall have length not null", strlen(password) > 0, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("password shall have length of SHA256", strlen(password) == AUTH_SHA256_SIZE, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("authorization pointer shall not be Null", authorization != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("authorization shall have length not null", strlen(authorization) > 0, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("authorization shall have length < MAX", strlen(authorization) < AUTH_STRING_SIZE, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("granted pointer shall not be Null", granted != NULL, returnCode, COM_ERR_PARAM);
    if (returnCode == COM_ERR_PARAM)
    {
        goto exit;
    }

    for (ii=0; (ii<AUTH_MAX_USERS) && (false == found); ii++)   /*search user*/
    {
            if (0 == strncmp(auth_users[ii].user, name, AUTH_STRING_SIZE))
            {
                index = ii;
                found = true;
            }
    }
    if (false == found)
    {
        TRACE_GEN (WARN, "    AUTH-RBAC user name has not been found in user_check_for_authent", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_UNKNOWN_ELEMENT;
        goto exit;
    }

    authenticated = false;
    if  (0 == strncmp (auth_users[index].hash_password, password, AUTH_SHA256_SIZE))  /*authentication is a success*/
        authenticated = true;
    
    found = false;
    for (jj=0; (jj<AUTH_MAX_AUTHORIZATIONS_IN_ROLE) && (false == found) && (true == authenticated); jj++)
    {
        if (NULL != auth_users[index].pRole1)
            if (NULL != auth_users[index].pRole1->pAuthorizations[jj])
                if (0 == strncmp( * (auth_users[index].pRole1->pAuthorizations[jj]), authorization, AUTH_STRING_SIZE))
                    found = true;
        if (NULL != auth_users[index].pRole2)
            if (NULL != auth_users[index].pRole2->pAuthorizations[jj])
                if (0 == strncmp( * (auth_users[index].pRole2->pAuthorizations[jj]), authorization, AUTH_STRING_SIZE))
                    found = true;
    }
    authorized = false;
    if  (true == found)  /*authorization for this user is found*/
        authorized = true;

    * granted = false;
    if ((authenticated) && (authorized))
        * granted = true;
        
    exit:
    END_FUNCTION();
    return returnCode;
}

