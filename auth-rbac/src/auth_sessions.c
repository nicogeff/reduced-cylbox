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



/*------------------------- Static functions -------------------------------*/

/* strlen of returned ID is MAX-1 */
static T_com_status auth_session_generate_random_Id (T_auth_rbac_sessionId_string  session_id)
{
    T_com_status  returnCode = COM_SUCCESS;
    uint32_t ii = 0u;
    char myChar = 0;

    START_FUNCTION();

    VERIF_ARGS ("session_id pointer shall not be Null", session_id != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("*session_id pointer shall be Null",   *session_id == (char)0, returnCode, COM_ERR_PARAM);    /*first char of string is Null, thus string is empty*/
    if (returnCode == COM_ERR_PARAM)
    {
        goto exit;
    }

    for (ii=0; ii<(AUTH_STRING_SIZE-1); ii++)
    {
        myChar = (((size_t) rand()) % 62);   /*0..61;  (10+26+26 possible characters)*/
        if (myChar< 10)                         /*affectation to a number*/
            myChar += '0';
        else if ((10 <= myChar) && (myChar <= 35))   /*affectation to lower letter*/
            myChar = 'a' + (myChar - 10);
        else if ((36 <= myChar) && (myChar <= 61))   /*affectation to upper letter*/
            myChar = 'A' + (myChar - 36);
        else
        {
            /*not possible*/
            TRACE_GEN (WARN, "    AUTH-RBAC sessions: unable to get a random number", __FUNCTION__, __LINE__);
            returnCode = COM_ERR_INTERNAL;
            goto exit;
        }
        session_id[ii] = myChar;
    }
    session_id[AUTH_STRING_SIZE-1] = 0;

exit:
    END_FUNCTION();
    return returnCode;
}

/*------------------------------ Functions -----------------------------------*/

/**
 * \brief Clean and initialize static sessions
 * 
 * Input:
 * 
 * Nothing is malloced
 * Do NOT take mutex here because it's done by upper "function auth_initialize_rbac()"
 * 
 * returns: T_COM_STATUS, Do nothing if not SUCCESS
 */
T_com_status auth_init_all_sessions ()
{
    T_com_status  returnCode = COM_SUCCESS;
    /*uint32_t ii = 0u; */
    /*bool found = false; */
    /*char string[128] = ""; */

    START_FUNCTION();

    memset ( &auth_sessions, 0, sizeof(auth_sessions) );

/*exit: */
    END_FUNCTION();
    return returnCode;
}



/**
 * \brief Create one session, with ID as a string copied at given address (of a char array)..
 * ID has fixed length. Only zeros after in the structure.
 * A place is choosen in static structure of sessions (fixed array).
 * If no more place, an error is retuned.
 * Session Id randomly generated inside. Length is max minus 1, with 0 at end
 * 
 * Input: address of a string where I is copied here.
 * 
 * Nothing is malloced or freed here.
 * returns: T_COM_STATUS, Do nothing if not SUCCESS
 */
T_com_status auth_create_session (char * pId)
{
    T_com_status  returnCode = COM_SUCCESS;
    uint32_t ii = 0u;
    size_t index = 0u;
    bool found = false;

    START_FUNCTION();

    VERIF_ARGS ("pId pointer shall not be Null, in init sessions", pId != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("*pId pointer shall be Null, in init sessions", *pId == ((char) 0), returnCode, COM_ERR_PARAM);
    if (returnCode == COM_ERR_PARAM)
    {
        goto exit;
    }

    (void) nal_mutexLock ( & auth_rbac_mutex_id);

    /*find empty place*/
    for (ii=0; (ii<AUTH_MAX_SESSIONS) && (false==found); ii++)
    {
        if (0 == auth_sessions[ii].session_id[0] )   /*empty string*/
        {
            index = ii;
            found = true;
        }
    }
    if (false == found)    /*no more place, error*/
    {
        TRACE_GEN (WARN, "    AUTH-RBAC users array is full in create_session", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_MAX_ELEMENTS;
        goto exit;
    }

    /* Raz the session stuct */
    memset ( &auth_sessions[index], 0, sizeof(auth_sessions[index]) );

    /* Get an Id and put it insise struct */
    returnCode = auth_session_generate_random_Id (auth_sessions[index].session_id);
    if (returnCode == COM_ERR_PARAM)
    {
        goto exit;
    }
    
    (void) strncpy( pId, auth_sessions[index].session_id, AUTH_STRING_SIZE);

exit:

    (void) nal_mutexUnlock ( & auth_rbac_mutex_id);

    END_FUNCTION();
    return returnCode;
}



/**
 * \brief Delete session whom Id is given.
 * RAZ the place in static structure.
 * If Id does not exist, an error is returned
 * 
 * Input: address of a string Id that is NOT freed inside.
 * Output: N/A
 * 
 * No matter of the state of session, authenticated or not..
 * returns: T_COM_STATUS, Do nothing if not SUCCESS
 */
T_com_status auth_delete_session (char * id)
{
    T_com_status  returnCode = COM_SUCCESS;
    uint32_t ii = 0u;
    size_t index = 0u;
    bool found = false;
    /*char string[128] = ""; */

    START_FUNCTION();

    (void) nal_mutexLock ( & auth_rbac_mutex_id);

    /*find  place*/
    for (ii=0; (ii<AUTH_MAX_USERS) && (false==found); ii++)
    {
        if (0 == strncmp(auth_sessions[ii].session_id, id, AUTH_STRING_SIZE))
        {
            index = ii;
            found = true;
        }
    }
    if (false == found)    /*targeted session has not been found, error*/
    {
        TRACE_GEN (WARN, "    AUTH-RBAC session Id not found in session array, in delete_session", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_UNKNOWN_ELEMENT;
        goto exit;
    }

    /* RAZ  the structure of targeted session */
    memset ( &auth_sessions[index], 0, sizeof(T_auth_rbac_session_struct) );

exit:

    (void) nal_mutexUnlock ( & auth_rbac_mutex_id);

    END_FUNCTION();
    return returnCode;
}



/**
 * \brief Authenticate a session whose session Id is given, with user/password.
 * 
 * Input: address of a string Id that is NOT freed inside.
 * Input: address of a string user that is NOT freed inside.
 * Input: address of a string password that is NOT freed inside.
 * Output: pSuccess of authentications, pointer on result of authentication in a boolean. In this case session is marked as "authenticated"
 * 
 * Care is taken, not to let password in memory, in heap or stack or global memory.
 * 
 * returns: COM_SUCCESS if function runs w/o error (no matter of authentication is success or not), Do nothing if this is not the case.
 * returns error if Id or user not found, or internal problem
 */
T_com_status auth_authenticate_session (char * id, char * user, char * password, bool * pSuccess)
{
    T_com_status  returnCode = COM_SUCCESS;
    uint32_t ii = 0u;
    size_t index = 0u;
    bool found = false;
    bool authenticated = false;
    /*char string[128] = ""; */
    T_auth_rbac_user_struct * pUser = NULL;

    START_FUNCTION();

    VERIF_ARGS ("id pointer shall not be Null", id != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("id shall have fixed length", (strlen(id)) == (AUTH_STRING_SIZE-1), returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("user pointer shall not be Null", user != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("user shall have length not null", strlen(user) > 0, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("user shall have length < MAX", strlen(user) < AUTH_STRING_SIZE, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("password pointer shall not be Null", password != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("password shall have length not null", strlen(password) > 0, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("password shall have length of SHA256", strlen(password) == AUTH_SHA256_SIZE, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("pSuccess shall be not Null", pSuccess != NULL, returnCode, COM_ERR_PARAM);
    if (returnCode == COM_ERR_PARAM)
    {
        goto exit;
    }

    (void) nal_mutexLock ( & auth_rbac_mutex_id);

    /*find session place*/
    for (ii=0; (ii<AUTH_MAX_SESSIONS) && (false==found); ii++)
    {
        if (0 == strncmp(auth_sessions[ii].session_id, id, AUTH_STRING_SIZE))
        {
            index = ii;
            found = true;
        }
    }
    if (false == found)    /*session id not found*/
    {
        TRACE_GEN (WARN, "    AUTH-RBAC session Id not found in array of sessions", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_UNKNOWN_ELEMENT;
        goto exit;
    }

    returnCode = auth_user_check_for_authent (user, password, & authenticated);
    if (COM_SUCCESS != returnCode)            
    {
        TRACE_GEN (WARN, " auth_authenticate_session, cannot check authentication of a user", __FUNCTION__, __LINE__);
        goto exit;
    }

    * pSuccess = false;
    if (true == authenticated)
    {
        returnCode = auth_user_getAddress (user, & pUser);
        if (COM_SUCCESS != returnCode)
        {
            TRACE_GEN (WARN, "auth_authenticate_session, cannot get address of user", __FUNCTION__, __LINE__);
            goto exit;
        }

        * pSuccess = true;
        auth_sessions[index].authenticated = true;
        auth_sessions[index].time_validity = get_auth_rbac_session_duration();

        auth_sessions[index].user = pUser;
    }

    
exit:
    if (true != * pSuccess)
    {
        auth_sessions[index].authenticated = false;
        auth_sessions[index].time_validity = 0u;
        auth_sessions[index].user = NULL;
    }

    (void) nal_mutexUnlock ( & auth_rbac_mutex_id);

    END_FUNCTION();
    return returnCode;
}



/**
 * \brief check for an Authorization in an authenticated session
 * 
 * Input: address of a string session_id that is NOT freed inside.
 * Input: address of a string Authorization that is NOT freed inside.
 * Output: boolean response meaning "authorization granted" if true, or not. Not allocated inside
 * 
 * returns: T_COM_STATUS if success, Do nothing if not SUCCESS
 * returns error if Id is not found, or session not authenticated, or internal problem
 */
T_com_status auth_check_authorization_in_session (char * id, char * authorization, bool  * pGranted)
{
    T_com_status  returnCode = COM_SUCCESS;
    uint32_t ii = 0u;
    size_t index = 0u;
    bool found = false;
    /*char string[128] = ""; */

    START_FUNCTION();

    VERIF_ARGS ("id pointer shall not be Null", id != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("id shall have fixed size", strlen(id) == (AUTH_STRING_SIZE-1), returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("authorization pointer shall not be Null", authorization != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("authorization shall have length not null", strlen(authorization) > 0, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("authorization shall have length < MAX", strlen(authorization) < AUTH_STRING_SIZE, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("pGranted shall be not Null", pGranted != NULL, returnCode, COM_ERR_PARAM);
     if (returnCode == COM_ERR_PARAM)
    {
        goto exit;
    }
    
    (void) nal_mutexLock ( & auth_rbac_mutex_id);

    /*find session place*/
    for (ii=0; (ii<AUTH_MAX_SESSIONS) && (false==found); ii++)
    {
        if (0 == strncmp (auth_sessions[ii].session_id, id, AUTH_STRING_SIZE) )
        {
            index = ii;
            found = true;
        }
    }
    if (false == found)    /*session not found, error*/
    {
        TRACE_GEN (WARN, "    AUTH-RBAC session id not found in check_authoriz", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_UNKNOWN_ELEMENT;
        goto exit;
    }

    if (false == auth_sessions[index].authenticated)
    {
        TRACE_GEN (WARN, "    AUTH-RBAC session not authenticated in check_authoriz", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_UNKNOWN_ELEMENT;
        goto exit;
    }

    
    returnCode = auth_user_check_for_authoriz (auth_sessions[index].user->user, authorization, pGranted);
    if (COM_SUCCESS != returnCode)
    {
        TRACE_GEN (WARN, "    AUTH-RBAC in session_check, user_check_for_authoriz has failed", __FUNCTION__, __LINE__);
        * pGranted = false;
        goto exit;
    }
    
exit:

    (void) nal_mutexUnlock ( & auth_rbac_mutex_id);

    END_FUNCTION();
    return returnCode;
}


/**
 * \brief give address of a session element
 * 
 * Input: address of a string "Id" that is NOT freed inside.
 * Output: address of user element
 * 
 * This is useful to build Rbac tables with real pointer, giving maximum efficience when parsing
 * returns: T_COM_STATUS if success, Do nothing if not SUCCESS
 * returns error if name is not found, or internal problem
 */
T_com_status auth_session_getAddress (char * session_id, T_auth_rbac_session_struct ** ppSession)
{
    T_com_status  returnCode = COM_SUCCESS;
    uint32_t ii = 0u;
    bool found = false;
    /*char string[128] = ""; */

    START_FUNCTION();

    VERIF_ARGS ("session_id pointer shall not be Null", session_id != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("session_id shall have length not null", strlen(session_id) > 0, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("session_id shall have length < MAX", strlen(session_id) < AUTH_STRING_SIZE, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("ppSession shall be not Null", ppSession != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("*ppSession shall be Null", *ppSession == NULL, returnCode, COM_ERR_PARAM);
    if (returnCode == COM_ERR_PARAM)
    {
        goto exit;
    }

    for (ii=0; (ii<AUTH_MAX_SESSIONS) && (false == found); ii++)   /*search session*/
    {
            if (0 == strncmp(auth_sessions[ii].session_id, session_id, AUTH_STRING_SIZE))
                found = true;
    }

    if (false == found)
    {
        TRACE_GEN (WARN, "    AUTH-RBAC session Id has not been found in session_getAddress", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_UNKNOWN_ELEMENT;
        goto exit;
    }
        
    * ppSession = & auth_sessions[ii];   /* output address */
    
exit:
    END_FUNCTION();
    return returnCode;
}


