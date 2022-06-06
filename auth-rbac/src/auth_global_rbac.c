/**
 * \file
 * \brief inside AUTH-RBAC in Box project
 *
 * \date 12/01/2019 NGE creation
 */

/*------------------------------ Definitions --------------------------------------*/
/* Only this source file create Rbac tables (is extern in other source files): */
#define EXTERN_AUTH

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
#include "nal_mem.h"

#include "com_error.h"
#include "com_entity.h"
#include "com_cryptolib_header.h"

#include "tools_trace.h"
#include "tools_parserJson.h"
#include "tools_parserJsonTools.h"

#include "com_auth_rbac_api.h"
#include "auth_global_rbac.h"

#include "cryptoal_signature.h"
#include "cryptoal_tls.h"


/*------------------------------ Structures --------------------------------------*/

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
T_com_status auth_initialize_rbac ()
{
    T_com_status  returnCode = COM_SUCCESS;

    START_FUNCTION();

    (void) nal_mutexLock ( & auth_rbac_mutex_id);

    /* These inits always succed*/
    auth_init_all_authorizations ();
    auth_init_all_roles ();
    auth_init_all_users ();
    auth_init_all_sessions ();

    (void) nal_mutexUnlock ( & auth_rbac_mutex_id);

/*exit: */
    END_FUNCTION();
    return returnCode;
}

//#define DEBUG_RBAC
//==============================================   DEBUG
#ifdef DEBUG_RBAC


static T_com_status auth_print_rbac_ram_debug ()
{
    T_com_status  returnCode = COM_SUCCESS;
    uint32_t ii = 0u;
    uint32_t jj = 0u;

    printf ("\n\t Authorizations:\n");
    for (ii=0; ii<AUTH_MAX_AUTHORIZATIONS; ii++)
    {
      if ( 0 != authorizations[ii][0] )
        printf ("%u  %s\n", ii, authorizations[ii]);
    }
    
    printf ("\n\t Roles:\n");
    for (ii=0; ii<AUTH_MAX_ROLES; ii++)
    {
        if (0 != auth_roles[ii].name[0])
        {
            printf ("%u %s  \n", ii, auth_roles[ii].name);
            for (jj=0; jj<AUTH_MAX_AUTHORIZATIONS_IN_ROLE; jj++)
            {
                if (0 != auth_roles[ii].pAuthorizations[jj])
                    printf ("   %u  %s\n", jj, (char*) auth_roles[ii].pAuthorizations[jj]);
            }
        }
    }

    printf ("\n\t Users:\n");
    for (ii=0; ii<AUTH_MAX_USERS; ii++)
    {
        if (0 != auth_users[ii].user[0])
        {
            printf ("%u  %s / %s, roles:  %s  %s\n", ii, auth_users[ii].user, auth_users[ii].hash_password, auth_users[ii].pRole1->name, auth_users[ii].pRole2->name);
        }
    }
    printf("\n\n");
    

exit:
    END_FUNCTION();
    return returnCode;
}
#endif  //DEBUG_RBAC
//==========================================   FIN DEBUG




/**
 * \brief re-Buid complete rbac, given a json buffer (coming from auth-mgt, or a web-service for example).
 * 
 * Input: pointer on the json buffer, containing authorizations, roles and users. Length is not useful
 * Output: N/A
 * 
 * If buffer gives an unconsistent rbac, nothing is done and and an error is returned
 * Nothing is malloced or freed inside except by json grab function. In this case json tree is freed with "tools_json_value_free".
 * 
 * returns: T_COM_STATUS, Do nothing useful  if not SUCCESS  but let the Rbac DESTROYED in Ram  !!!
 */
T_com_status auth_loadJsonBuffer_in_rbac_ram (void * p_json_buffer)
{
    T_com_status  returnCode = COM_SUCCESS;
    uint32_t ii = 0u;
    uint32_t jj = 0u;
    uint32_t kk = 0u;
    /*bool found = false; */
    /*char string[128] = ""; */
    /*int nalReturn = 0; */
    bool user_inactif = false;
    size_t authorization_index = 0u;
    size_t role_index = 0u;
    
    //T_nal_stream * json_rbac_stream = NULL;
    T_tools_json_value* jsonTree =  (T_tools_json_value *) p_json_buffer;

    T_tools_json_type version_dataType = 0;
    T_tools_union_data version_dataValue = {0};
    
    T_tools_json_type authoriz_dataType = 0;
    T_tools_union_data authoriz_dataValue = {0};
    T_tools_json_type roles_dataType = 0;
    T_tools_union_data roles_dataValue = {0};
    T_tools_json_type role_dataType = 0;
    T_tools_union_data role_dataValue = {0};
    T_tools_json_type authoridx_dataType = 0;
    T_tools_union_data authoridx_dataValue = {0};

    T_tools_json_type users_dataType = 0;
    T_tools_union_data users_dataValue = {0};
    
    T_tools_json_type user_dataType = 0;
    T_tools_union_data user_dataValue = {0};

    T_tools_json_type inactif_dataType = 0;
    T_tools_union_data inactif_dataValue = {0};
    
    T_tools_json_type hashedPassword_dataType = 0;
    T_tools_union_data hashedPassword_dataValue = {0};

    T_tools_json_type roleidx_dataType = 0;
    T_tools_union_data roleidx_dataValue = {0};

    /* Rbac data */
    int rbac_version = -1;

    START_FUNCTION();

    VERIF_ARGS ("json_rbac_buffer pointer shall not be Null", p_json_buffer != NULL, returnCode, COM_ERR_PARAM);
    if (returnCode == COM_ERR_PARAM)
    {
        goto exit;
    }

    /*For debug: tools_json_print_tree(jsonTree, 0);*/

    /* Get field "version" as a string: */
    tools_json_grab_data(jsonTree, "version", & version_dataType, & version_dataValue);                  /*version of Rbac format*/
    if ( json_null == version_dataType )
    {
        returnCode = COM_ERR_TUPLE_MISSING;
        goto exit;
    }
    else if (json_integer != version_dataType)
    {
        returnCode = COM_ERR_TUPLE_BAD_KEY;
        goto exit;
    }
    rbac_version = version_dataValue.integer;   /* for futur usage  */

    /* Empty and clean Rbac in memory, to build a new one (mutex is managed inside) */
    (void) auth_initialize_rbac ();

    (void) nal_mutexLock ( & auth_rbac_mutex_id);

    /* Get array of "authorizations" as a numbered array of strings: */
    tools_json_grab_data(jsonTree, "authorizations", & authoriz_dataType, & authoriz_dataValue);                  /*Load authorizations*/
    if ( json_null == authoriz_dataType )
    {
        returnCode = COM_ERR_TUPLE_MISSING;
        goto exit;
    }
    else if (json_array != authoriz_dataType)
    {
        returnCode = COM_ERR_TUPLE_BAD_KEY;
        goto exit;
    }
    for (ii=0; ii<authoriz_dataValue.array.length; ii++)
    {
        returnCode = auth_create_authorization ( (char *) authoriz_dataValue.array.values[ii]->u.string.ptr);
        if (COM_SUCCESS != returnCode)
        {
            TRACE_GEN (WARN, "Auth LoadJsonBuffer, cannot createAuthorization", __FUNCTION__, __LINE__);
            goto exit;
        }
    }


    /* Get array of "roles" as a numbered array of strings: */
    tools_json_grab_data(jsonTree, "roles", &roles_dataType, &roles_dataValue);                  /*Load roles*/
    if ( json_null == roles_dataType )
    {
        returnCode = COM_ERR_TUPLE_MISSING;
        goto exit;
    }
    else if (json_array != roles_dataType)
    {
        returnCode = COM_ERR_TUPLE_BAD_KEY;
        goto exit;
    }
    for (ii=0; ii<roles_dataValue.array.length; ii++)
    {
         tools_json_grab_data(roles_dataValue.array.values[ii], "role", &role_dataType, &role_dataValue);                  /*Load a role's name*/
        if ( json_null == role_dataType )
        {
            returnCode = COM_ERR_TUPLE_MISSING;
            goto exit;
        }
        else if (json_string != role_dataType)
        {
            returnCode = COM_ERR_TUPLE_BAD_KEY;
            goto exit;
        }
        strncpy (auth_roles[ii].name, role_dataValue.string.ptr, role_dataValue.string.length);

        tools_json_grab_data(roles_dataValue.array.values[ii], "author-idx", &authoridx_dataType, &authoridx_dataValue);      /*point a role's authorizations*/ //TODO check AUTH_MAX_AUTHORIZATIONS_IN_ROLE
        if ( json_null == authoridx_dataType )
        {
            returnCode = COM_ERR_TUPLE_MISSING;
            goto exit;
        }
        else if (json_array != authoridx_dataType)
        {
            returnCode = COM_ERR_TUPLE_BAD_KEY;
            goto exit;
        }

        for (jj=0; jj<authoridx_dataValue.array.length; jj++)
        {
            authorization_index = authoridx_dataValue.array.values[jj]->u.integer;     /* TODO not mandatory: check if an idx is multiple times */
            /* check if index and string really point an authorization */
            if ( (authorization_index < AUTH_MAX_AUTHORIZATIONS) && ( authorizations [authorization_index][0] != (char) 0 ) )
                auth_roles[ii].pAuthorizations[jj] = & authorizations [authorization_index];
        }
    }


    /* Get array of "users" as a numbered array of strings: */
    tools_json_grab_data(jsonTree, "users", &users_dataType, &users_dataValue);                  /*Load users*/
    if ( json_null == users_dataType )
    {
        returnCode = COM_ERR_TUPLE_MISSING;
        goto exit;
    }
    else if (json_array != users_dataType)
    {
        returnCode = COM_ERR_TUPLE_BAD_KEY;
        goto exit;
    }
    /* jj loops on user that are active */
    for (ii=0, jj=0; ii<users_dataValue.array.length; ii++)
    {

        tools_json_grab_data(users_dataValue.array.values[ii], "inactif", &inactif_dataType, &inactif_dataValue);         /*Load a user's "inactif" flag*/
        if ( json_null == inactif_dataType )
        {
            returnCode = COM_ERR_TUPLE_MISSING;
            goto exit;
        }
        else if (json_boolean != inactif_dataType)
        {
            returnCode = COM_ERR_TUPLE_BAD_KEY;
            goto exit;
        }
        user_inactif = inactif_dataValue.boolean;

        if (false == user_inactif)
        {
            tools_json_grab_data(users_dataValue.array.values[ii], "user", &user_dataType, &user_dataValue);         /*Load a user's name*/
            if ( json_null == user_dataType )
            {
                returnCode = COM_ERR_TUPLE_MISSING;
                goto exit;
            }
            else if (json_string != user_dataType)
            {
                returnCode = COM_ERR_TUPLE_BAD_KEY;
                goto exit;
            }
            strncpy (auth_users[jj].user, user_dataValue.string.ptr, user_dataValue.string.length);

            tools_json_grab_data(users_dataValue.array.values[ii], "hashedPassword", &hashedPassword_dataType, &hashedPassword_dataValue);         /*Load a user's hashedPassword*/
            if ( json_null == hashedPassword_dataType )
            {
                returnCode = COM_ERR_TUPLE_MISSING;
                goto exit;
            }
            else if (json_string != hashedPassword_dataType)
            {
                returnCode = COM_ERR_TUPLE_BAD_KEY;
                goto exit;
            }
            strncpy (auth_users[jj].hash_password, hashedPassword_dataValue.string.ptr, hashedPassword_dataValue.string.length);

            tools_json_grab_data(users_dataValue.array.values[ii], "role-idx", &roleidx_dataType, &roleidx_dataValue);      /*point a user's roles*/
            if ( json_null == roleidx_dataType )
            {
                returnCode = COM_ERR_TUPLE_MISSING;
                goto exit;
            }
            else if (json_array != roleidx_dataType)
            {
                returnCode = COM_ERR_TUPLE_BAD_KEY;
                goto exit;
            }

            for (kk=0; kk<roleidx_dataValue.array.length; kk++)  /* k loops on indexes */
            {
                role_index = roleidx_dataValue.array.values[kk]->u.integer;
                if (role_index <  roles_dataValue.array.length   /* index lower than global number of roles */ )
                {
                    if (NULL == auth_users[jj].pRole1)
                        auth_users[jj].pRole1 = & auth_roles [(int) role_index];
                    else if (NULL == auth_users[ii].pRole2)
                        auth_users[jj].pRole2 = & auth_roles [(int) role_index];
                }

            }
            jj++;
        }

// TODO sauver les session authentifiees, si le user est dans la nouvelle rbac

    }

#ifdef DEBUG_RBAC
    auth_print_rbac_ram_debug();    // DEBUG
#endif
    
exit:
    (void) nal_mutexUnlock ( & auth_rbac_mutex_id);

    /* liberer la ram alloué pour la structure json, error or not */
    tools_json_value_free(jsonTree);

    END_FUNCTION();
    return returnCode;
}



/**
 * \brief Core of thread in AUTH_RBAC: Handles all time to live in Rbac structure, like session duration since last authentication, and temporary locked account duration
 * \details This function enter an infinite loop which basically:
 * \li wake up each 10 second, this is the granularity, vlue are in seconds
 * \li take or wait for the mutex
 * \li decrement time to live for session and locking status of an account
 * \li Take decision if a time to live as reach zero
 * \li give back the mutex
 * \li Sleep again, to wake up lately
 * 
 * PARAM data: not used
 * \return void
 */
#define AUTH_DURATION_TICK 10
void auth_duration_thread ( void *data )
{
    int32_t alReturn = 0;
    uint32_t ii = 0u;
    T_nal_time tick_timetable = {AUTH_DURATION_TICK, 0};

    while (NAL_TRUE == auth_duration_thread_status)
    {
        tick_timetable.sec = AUTH_DURATION_TICK;
        alReturn = nal_sleep ( & tick_timetable );
        if (NAL_SUCCESS != alReturn)
        {
            TRACE_GEN (WARN, " AUTH_DURATION thread for time to live, failed to sleep few second", __FUNCTION__, __LINE__);
        }

        /* Protection d'acces ecriture echeancier par mutex */
        (void) nal_mutexLock ( & auth_rbac_mutex_id);

        /* Manages session duration */
        for (ii=0; ii<AUTH_MAX_SESSIONS; ii++)
        {
            if ((auth_sessions[ii].session_id[0] != 0) && (true == auth_sessions[ii].authenticated))   /*session to check*/
            {
                if (auth_sessions[ii].time_validity > AUTH_DURATION_TICK)
                {
                    auth_sessions[ii].time_validity = auth_sessions[ii].time_validity - AUTH_DURATION_TICK;   /*just decrement remaining time*/
                }
                else   /* break authent */
                {
                    auth_sessions[ii].user = NULL;
                    auth_sessions[ii].authenticated = false;
                    auth_sessions[ii].time_validity = 0u;
                }
            }
        }



        /* Manages locked account */
        
        /* TODO */



        /* Fin protection d'acces ecriture echeancier */
        (void) nal_mutexUnlock ( & auth_rbac_mutex_id);

    }  /*End of life of thread*/

exit:
    TRACE_GEN (WARN, "Orchestrator thread for timetable is finishing.." , __FUNCTION__, __LINE__ );

    nal_taskExit ();
}


