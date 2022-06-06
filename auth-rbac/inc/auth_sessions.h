/**
 * \file
 * \brief inside AUTH-RBAC in Box project
 *
 * \date 12/01/2019 NGE creation
 */

#ifndef C_auth_rbac_sessions_h_included
#define C_auth_rbac_sessions_h_included

/*------------------------------ headers included -------------------------------*/
#include "com_error.h"


/*------------------------------ Definitions --------------------------------------*/


/*------------------------------ Structuress --------------------------------------*/

/* ... */
// type of one session Id, empty if first char is Null otherwise last char is Null (fixed size).
// It is randomly generated when sessionis created. Each char is in a..b/A..B/0..9 so in 62 possible values. So it is not predictable.
typedef char T_auth_rbac_sessionId_string[AUTH_STRING_SIZE];    

typedef struct T_auth_rbac_session_struct
{
    T_auth_rbac_sessionId_string  session_id;    // complex id as a 32 bytes long string. If equal to "", session is empty. In this case nothing is valid after.
    bool  authenticated;         // nothing valid after, if false
    T_auth_rbac_user_struct * user;                   //pointer to a user element
    uint32_t  time_validity;    // TTL in seconds for this session. So far NOT USED
}
T_auth_rbac_session_struct,  *T_auth_rbac_session_struct_p;

/* static structure */
EXTERN_AUTH  T_auth_rbac_session_struct  auth_sessions[AUTH_MAX_SESSIONS];



/*------------------------------ Functions -----------------------------------*/

/**
 * \brief Clean and initialize static sessions
 * 
 * Input:
 * 
 * Nothing is malloced
 * returns: T_COM_STATUS, Do nothing if not SUCCESS
 */
T_com_status auth_init_all_sessions ();


/**
 * \brief Create one session, with ID copied at given adress..
 * ID has fixed length. Only zeros after in the structure.
 * A place is choosen in static structure of sessions (fixed array).
 * If no more place, an error is retuned.
 * Session Id randomly generated inside. Length is max minus 1, with 0 at end
 * 
 * Input: adress of a string xhere I is copied here.
 * 
 * Nothing is malloced or freed here.
 * returns: T_COM_STATUS, Do nothing if not SUCCESS
 */
/* 
 * Exported in common header: T_com_status auth_create_session (char * pId);
 */


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
/*  Exported in common header:  T_com_status auth_delete_session (char * id); */


/**
 * \brief Authenticate a session whose session Id is given, with user/password.
 * 
 * Input: adresse of a string Id that is NOT freed iside.
 * Input: adresse of a string user that is NOT freed iside.
 * Input: adresse of a string password that is NOT freed iside.
 * Output: pSuccess, pointer on result of authentication in a boolean
 * 
 * Care is taken, not to let password in memory, in heap or stack or globals memory.
 * 
 * returns: COM_SUCCESS if success, Do nothing if not SUCCESS
 * returns error if Id or user not found, or internal problem
 */
/*  Exported in common header:  T_com_status auth_authenticate_session (char * id, char * user, char * password, bool * pSuccess); */


/**
 * \brief check for an Authorization n a session
 * 
 * Input: adresse of a string Id that is NOT freed iside.
 * Input: adresse of a string Authorization that is NOT freed iside.
 * Input: adresse of a string password that is NOT freed iside.
 * Output: boolean response meaning "authorization granted" if true, or not. Not allocated inside
 * 
 * returns: T_COM_STATUS if success, Do nothing if not SUCCESS
 * returns error if Id is not found, or session not authenticated, or internal problem
 */
/*  Exported in common header:   T_com_status auth_check_authorization_in_session (char * id, char * authorization, bool  * pGranted); */


/**
 * \brief give address of a session element
 * 
 * Input: adresse of a string "Id" that is NOT freed iside.
 * Output: adress of user element
 * 
 * This is useful to build Rbac tables with real pointer, giving maximum efficience when parsing
 * returns: T_COM_STATUS if success, Do nothing if not SUCCESS
 * returns error if name is not found, or internal problem
 */
T_com_status auth_session_getAddress (char * session_id, T_auth_rbac_session_struct ** ppSession);


#ifdef __cplusplus
extern "C"
{
#endif




#ifdef __cplusplus
}
#endif


#endif /* C_auth_rbac_sessions_h_include */
