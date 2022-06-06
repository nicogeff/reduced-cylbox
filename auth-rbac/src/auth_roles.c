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


/*------------------------------ Structuress --------------------------------------*/

/* ... */



/*------------------------------ Functions -----------------------------------*/

/**
 * \brief Clean and initialize static sessions
 * 
 * Input:
 * 
 * Nothing is malloced
 * Do NOT take mutex here because it's done by upper "fonction auth_initialize_rbac()"

 * returns: T_COM_STATUS, Do nothing if not SUCCESS
 */
T_com_status auth_init_all_roles ()
{
    T_com_status  returnCode = COM_SUCCESS;

    START_FUNCTION();

//    VERIF_ARGS ("type strictly between UNKNOWN and MAX", type > ORCHEST_EVENT_TYPE_UNKNOWN, returnCode, COM_ERR_PARAM);
//    if (returnCode == COM_ERR_PARAM)
//    {
//        goto exit;
//    }
    
    memset ( &auth_roles, 0, sizeof(auth_roles) );

/*exit: */
    END_FUNCTION();
    return returnCode;
}


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
T_com_status auth_create_role (char * name, T_auth_rbac_authorization_string  * authorizations_array, size_t nb_authorizations)
{
    T_com_status  returnCode = COM_SUCCESS;
    uint32_t ii = 0u;
    size_t index = 0u;
    uint32_t jj = 0u;
    bool found = false;
    /* char string[128] = ""; */

    START_FUNCTION();

    VERIF_ARGS ("name pointer shall not be Null", name != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("name shall have length > 0", strlen(name) > 0, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("name shall have length < MAX", strlen(name) < AUTH_STRING_SIZE, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("authorizations_array pointer shall not be Null", authorizations_array != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("nb_authorizations shall not be Null", nb_authorizations != 0u, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("nb_authorizations shall be <= Max", nb_authorizations <= AUTH_MAX_AUTHORIZATIONS_IN_ROLE, returnCode, COM_ERR_PARAM);
    for (ii=0; ii<nb_authorizations; ii++)
    {
        VERIF_ARGS ("authorization number ii shall not be Null", authorizations_array[ii] != NULL, returnCode, COM_ERR_PARAM);
        VERIF_ARGS ("authorization number ii shall have length bigger than 0", 0<strlen(authorizations_array[ii]), returnCode, COM_ERR_PARAM);
        VERIF_ARGS ("authorization number ii shall have length lower Max", strlen(authorizations_array[ii])<AUTH_STRING_SIZE, returnCode, COM_ERR_PARAM);
    }
    if (returnCode == COM_ERR_PARAM)
    {
        goto exit;
    }
    
    /*find empty place*/
    for (ii=0; (ii<AUTH_MAX_ROLES) && (false==found); ii++)
    {
        if (0 == auth_roles[ii].name[0] )
        {
            index = ii;
            found = true;
        }
    }
    if (false == found)    /*no more place, error*/
    {
        TRACE_GEN (WARN, "    AUTH-RBAC roles array is full", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_MAX_ELEMENTS;
        goto exit;
    }

    memcpy (auth_roles[index].name, name, 1+strlen(name));    /*put role name in it, with trailing 0*/
    
    /*copy array of authorizations in the role*/
    for (jj=0; jj<nb_authorizations; jj++)
    {
        /*  check if each authorization in the array is really  string*/
        if (authorizations_array[jj][0] != (char)0 )
        {
            auth_roles[index].pAuthorizations[jj] = & authorizations_array[jj];     /* TODO check if pointed authorization exists in Ram */
        }
    }
    
exit:
    END_FUNCTION();
    return returnCode;
}

/**
 * \brief Delete role whose name is given.
 * RAZ the place in static structure and delete role in users using it.
 * If role does not exist, an error is returned
 * 
 * Input: adresse of a string Id that is NOT freed iside.
 * Output: N/A
 * 
 * returns: T_COM_STATUS, Do nothing if not SUCCESS
 */
T_com_status auth_delete_role (char * name)
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
 * \brief give address of a role element
 * 
 * Input: adresse of a string "name of role" that is NOT freed iside.
 * Output: adress of user element
 * 
 * This is useful to build Rbac tables with real pointer, giving maximum efficience when parsing
 * returns: T_COM_STATUS if success, Do nothing if not SUCCESS
 * returns error if name is not found, or internal problem
 */
T_com_status auth_role_getAddress (char * name, T_auth_rbac_role_struct ** ppRole)
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
    VERIF_ARGS ("ppRole pointer shall not be Null", ppRole != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("*ppRole pointer shall be Null", *ppRole != NULL, returnCode, COM_ERR_PARAM);
    if (returnCode == COM_ERR_PARAM)
    {
        goto exit;
    }
    
    for (ii=0; (ii<AUTH_MAX_ROLES) && (false == found); ii++)   /*search role*/
    {
            if (0 == strncmp(auth_roles[ii].name, name, AUTH_STRING_SIZE))
            {
                index = ii;
                found = true;
            }
    }

    if (false == found)
    {
        TRACE_GEN (WARN, "    AUTH-RBAC role name has not been found in role_getAddress", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_UNKNOWN_ELEMENT;
        goto exit;
    }
        
    * ppRole = & auth_roles[index];   /* output address */
    
    exit:
    END_FUNCTION();
    return returnCode;
}

/**
 * \brief suppress an authorization pointer, in one given role. Just RAZ the pointer when found and shall happen once
 * 
 * Input: adresse of an authorization.
 * Output: N/A
 * 
 * returns: T_COM_STATUS since it always succed, except bad parameters.
 */
T_com_status auth_delete_authorization_from_role (char * nameRole, T_auth_rbac_authorization_string  authorization)
{
    T_com_status  returnCode = COM_SUCCESS;
    uint32_t ii = 0u;
    size_t index = 0u;
    uint32_t jj = 0u;
    bool found = false;
    /*char string[128] = ""; */

    START_FUNCTION();

    VERIF_ARGS ("nameRole pointer shall not be Null", nameRole != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("nameRole shall have length not null", strlen(nameRole) > 0, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("nameRole shall have length < MAX", strlen(nameRole) < AUTH_STRING_SIZE, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("authorization pointer shall not be Null", authorization != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("authorization shall have length not null", strlen(authorization) > 0, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("authorization shall have length < MAX", strlen(authorization) < AUTH_STRING_SIZE, returnCode, COM_ERR_PARAM);
    if (returnCode == COM_ERR_PARAM)
    {
        goto exit;
    }
    
    /* search the role in array */
    for (ii=0;(ii<AUTH_MAX_ROLES) && (false==found); ii++)
    {
        if (0 == strncmp(auth_roles[ii].name, nameRole, AUTH_STRING_SIZE))
        {
            index = ii;
            found = true;
        }
    }

    if (false == found)
    {
        TRACE_GEN (WARN, "    AUTH-RBAC authorization name has not been found in delete_authoriz", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_UNKNOWN_ELEMENT;
        goto exit;
    }

    for (jj=0; jj<AUTH_MAX_AUTHORIZATIONS_IN_ROLE; jj++)
    {
        if ( (* auth_roles[index].pAuthorizations[jj]) == authorization)
            auth_roles[index].pAuthorizations[jj] = NULL;   /*RAZ authorization pointer all time it feets*/
    }

exit:
    END_FUNCTION();
    return returnCode;
}


/**
 * \brief suppress an authorization pointer, in all roles where it appears (0 or N times). Just RAZ pointers when found
 * 
 * Input: adresse of an authorization.
 * Output: N/A
 * 
 * returns: T_COM_STATUS since it always succed, except bad parameters.
 */
T_com_status auth_delete_authorization_from_all_roles (T_auth_rbac_authorization_string  authorization)
{
    T_com_status  returnCode = COM_SUCCESS;
    uint32_t ii = 0u;
    uint32_t jj = 0u;
    /*bool found = false; */
    /*char string[128] = ""; */

    START_FUNCTION();

    VERIF_ARGS ("authorization pointer shall not be Null", authorization != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("authorization shall have length not null", strlen(authorization) > 0, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("authorization shall have length < MAX",  strlen(authorization) < AUTH_STRING_SIZE, returnCode, COM_ERR_PARAM);
    if (returnCode == COM_ERR_PARAM)
    {
        goto exit;
    }
    
    for (ii=0; ii<AUTH_MAX_ROLES; ii++)
    {
        for (jj=0; jj<AUTH_MAX_AUTHORIZATIONS_IN_ROLE; jj++)
        {
            if ( (* auth_roles[ii].pAuthorizations[jj]) == authorization)
                auth_roles[ii].pAuthorizations[jj] = NULL;   /*RAZ authorization pointer all time it feets*/
        }
    }

exit:
    END_FUNCTION();
    return returnCode;
}
