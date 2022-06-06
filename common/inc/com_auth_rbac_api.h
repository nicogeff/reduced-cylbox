/*
 * \file com_auth_rbac_api.h
 * \brief auth_rbac external API's with useful Constants
 * \specifics
 *
 */

#ifndef COM_AUTH_RBAC_API_H
#define COM_AUTH_RBAC_API_H

#ifdef __cplusplus
extern "C" {
#endif


/**
 * \brief all exported constantes & types for auth-rbac
 */

#define  AUTH_STRING_SIZE  32
    
/* sha256 in printable ascii formt, so double size */
#define  AUTH_SHA256_SIZE  (2*32)
#define  SESSION_ID_STRING_SIZE  AUTH_STRING_SIZE


/*--------------------------  Exported functions   --------------------------*/


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
T_com_status auth_create_session (char * pId);


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
T_com_status auth_delete_session (char * id);


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
T_com_status auth_authenticate_session (char * id, char * user, char * password, bool * pSuccess);


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
T_com_status auth_check_authorization_in_session (char * id, char * authorization, bool  * pGranted);



/**
 * \brief re-Buid complete rbac, given a json buffer (coming from other entity, or web-service).
 *   Write directly in Ram structures figuring current Rbac. Radical action w/o mutex.

 * Input: pointer on the json buffer, containing authorizations, roles and users. Length is not useful
 * Output: N/A
 * 
 * If buffer gives an unconsistent rbac, nothing is done and and an error is returned
 * Nothing is malloced or freed inside
 * returns: T_COM_STATUS, Do nothing if not SUCCESS
 */
T_com_status auth_loadJsonBuffer_in_rbac_ram (void * p_json_buffer);


/*----------------------  End of Exported functions   -----------------------*/

#ifdef __cplusplus
}
#endif

#endif /* COM_AUTH_RBAC_API_H */

