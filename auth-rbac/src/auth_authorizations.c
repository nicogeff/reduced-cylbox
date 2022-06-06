/**
 * \file    auth_authorization.c
 * \brief  inside AUTH-RBAC in Box project
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


EXTERN_AUTH T_auth_rbac_authorization_string  authorizations[AUTH_MAX_AUTHORIZATIONS];      /* Array of authorizations */


/*------------------------------ Functions -----------------------------------*/

/**
 * \brief Clean and initialize static array of authorizations
 * 
 * Input:
 * 
 * Nothing is malloced
 * Do NOT take mutex here because it's done by upper "fonction auth_initialize_rbac()"
 * returns: T_COM_STATUS, Do nothing if not SUCCESS
 */
T_com_status auth_init_all_authorizations ()
{
    T_com_status  returnCode = COM_SUCCESS;

    START_FUNCTION();

/*    VERIF_ARGS ("type strictly between UNKNOWN and MAX", type > ORCHEST_EVENT_TYPE_UNKNOWN, returnCode, COM_ERR_PARAM);
 *    if (returnCode == COM_ERR_PARAM)
 *    {
 *        goto exit;
 *    }
 */ 
    memset ( &authorizations, 0, sizeof(authorizations) );

/*exit: */
    END_FUNCTION();
    return returnCode;
}



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
T_com_status auth_create_authorization (char * name)
{
    T_com_status  returnCode = COM_SUCCESS;
    uint32_t ii = 0u;
    bool found = false;
    size_t found_index = 0u;
/*    char string[128] = ""; */

    START_FUNCTION();

    VERIF_ARGS ("name pointer shall not be Null", name != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("name shall have length > 0", strlen(name) > 0, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("name shall have length < MAX", strlen(name) < AUTH_STRING_SIZE, returnCode, COM_ERR_PARAM);
    if (returnCode == COM_ERR_PARAM)
    {
        goto exit;
    }
    
    /*find empty place*/
    for (ii=0; (ii<AUTH_MAX_AUTHORIZATIONS) && (false==found); ii++)
    {
        if ( ((char)0) == authorizations[ii][0] )
        {
            found = true;
            found_index = ii;
        }
    }
    if (false == found)    /*no more place, error*/
    {
        TRACE_GEN (WARN, "    AUTH-RBAC authorization array is full", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_MAX_ELEMENTS;
        goto exit;
    }
    else /*fill the place*/
    {
        memcpy (authorizations[found_index], name, 1+strlen(name));    /*put name in it, with trailing 0*/
    }
    
exit:
    END_FUNCTION();
    return returnCode;
}


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
T_com_status auth_delete_authorization (char * name)
{
    T_com_status  returnCode = COM_SUCCESS;
    uint32_t ii = 0u;
    size_t index = 0u;
    bool found = false;
    /*char string[128] = ""; */

    START_FUNCTION();

    VERIF_ARGS ("name pointer shall not be Null", name != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("name shall have length > 0", strlen(name) > 0, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("name shall have length < MAX", strlen(name) < AUTH_STRING_SIZE, returnCode, COM_ERR_PARAM);
    if (returnCode == COM_ERR_PARAM)
    {
        goto exit;
    }
    
    /*find the place with name*/
    for (ii=0; (ii<AUTH_MAX_AUTHORIZATIONS) && (false==found); ii++)
    {
        if (0 == authorizations[ii][0])  {}    /*empty place*/
        else if (0 == strncmp(name, authorizations[ii], AUTH_STRING_SIZE ))
        {
          index = ii;
            found = true;
        }
        else {}  /*another authorization*/
    }
    if (false == found)    /*not found, error*/
    {
        TRACE_GEN (WARN, "    AUTH-RBAC authorization name has not been found", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_UNKNOWN_ELEMENT;
        goto exit;
    }
    else /*found searched item, now raz it*/
    {
        memset (authorizations[index], 0, AUTH_STRING_SIZE);    /*put name in it, with trailing 0*/
    }
    
    /* Delete this authorizations from roles where it appears */
    returnCode = auth_delete_authorization_from_all_roles (authorizations[index]);
    if (COM_SUCCESS != returnCode)    /*internal error in previous function*/
    {
        TRACE_GEN (WARN, "    AUTH-RBAC authorization: delete one authorization from al roles failed", __FUNCTION__, __LINE__);
        goto exit;
    }

exit:
    END_FUNCTION();
    return returnCode;
}


/**
 * \brief give address of an authorization element            =====>          NOT USED
 * 
 * Input: adresse of a string "name of authorization" that is NOT freed iside.
 * Output: adress of authorization element, that shall not be modified by caller
 * 
 * This is useful to build Rbac tables with real pointer, giving maximum efficience when parsing
 * returns: T_COM_STATUS if success, Do nothing if not SUCCESS
 * returns error if name is not found, or internal problem
 */
/*
T_com_status auth_authorization_getAddress (char * name, T_auth_rbac_authorization_string * pAuthorization)      =====>    NOT USED
{
    T_com_status  returnCode = COM_SUCCESS;
    uint32_t ii = 0u;
    size_t index = 0u;
    bool found = false;
    /*char string[128] = ""; * /

    START_FUNCTION();

    VERIF_ARGS ("name pointer shall not be Null", name != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("name shall have length > 0", strlen(name) > 0, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("name shall have length < MAX", strlen(name) < AUTH_STRING_SIZE, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("pAuthorization pointer shall not be Null", pAuthorization!=NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("*pAuthorization object shall be Null", *pAuthorization==NULL, returnCode, COM_ERR_PARAM);
    if (returnCode == COM_ERR_PARAM)
    {
        goto exit;
    }
    
    /*find the place with name* /
    for (ii=0; (ii<AUTH_MAX_AUTHORIZATIONS) && (false==found); ii++)
    {
        if (0 == authorizations[ii][0])  {}    /*empty place* /
        else if (0 == strncmp(name, authorizations[ii], AUTH_STRING_SIZE ))
        {
            index = ii;
            found = true;
        }
        else {}
    }
    if (false == found)    /*not found, error* /
    {
        TRACE_GEN (WARN, "    AUTH-RBAC authorization name has not been found", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_UNKNOWN_ELEMENT;
        goto exit;
    }
    else /*found searched item, now raz it* /
    {
         pAuthorization = & authorizations[index];    /*Output adress of element* /
    }

exit:
    END_FUNCTION();
    return returnCode;
}
*/
