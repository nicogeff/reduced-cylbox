#include <stdio.h>
#include <stdbool.h>

#include "cryptoal_base64.h"

#include "com_error.h"
#include "com_entity.h"
#include "com_auth_rbac_api.h"

#include "tools_trace.h"
#include "tools_parserJson.h"

#include "entmgt_utils.h"

#include "webserver_error.h"
#include "webserver_operations.h"

/**
 * @brief This function request authentication engine to
 * generate a new session
 * @param[out] id This parameter will be field with the unique session ID
 * @return A T_com_status error code
 */
T_com_status webserver_op_createSession(char *id)
{
    T_com_status returnCode = COM_SUCCESS;

    /* Sanity check */
    VERIF_ARGS("id", id != NULL, returnCode, COM_ERR_PARAM);
    if (returnCode == COM_ERR_PARAM)
    {
        TRACE_GEN(TRACE, "Invalid parameter", __FUNCTION__, __LINE__);
        goto exit;
    }

    /* Ask to create session */
    returnCode = auth_create_session(id);

exit:
    return returnCode;
}

/**
 * @brief This function request authentication engine to 
 * check that credential is valid. This function need a valid session ID.
 * That means is mandatory to call create session before.
 * @param[in] id A unique session ID
 * @param[in] login User login
 * @param[in] password User password
 * @param[out] signinStatus Will contains the result of authentication engine checks
 * @return A T_com_status error code
 */
T_com_status webserver_op_signIn(const char *id, const char *login, const char *password, NAL_BOOL *signinStatus)
{
    T_com_status returnCode = COM_SUCCESS;

    /* Sanity check */
    VERIF_ARGS("id", id != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("login", login != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("password", password != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("signinStatus", signinStatus != NULL, returnCode, COM_ERR_PARAM);
    if (returnCode == COM_ERR_PARAM)
    {
        TRACE_GEN(TRACE, "Invalid parameter", __FUNCTION__, __LINE__);
        goto exit;
    }

    /* Ask to create session */
    returnCode = auth_authenticate_session(id, login, password, (bool*)signinStatus);

exit:
    return returnCode;
}

/**
 * @brief This function request authentication engine to close et destroy the session ID
 * @param[in] id A unique session ID
 * @return A T_com_status error code
 */
T_com_status webserver_op_signOut(const char *id)
{
    T_com_status returnCode = COM_SUCCESS;

    /* Sanity check */
    VERIF_ARGS("id", id != NULL, returnCode, COM_ERR_PARAM);
    if (returnCode == COM_ERR_PARAM)
    {
        TRACE_GEN(TRACE, "Invalid parameter", __FUNCTION__, __LINE__);
        goto exit;
    }

    /* Delete session */
    auth_delete_session(id);

exit:
    return returnCode;
}

/**
 * @brief This function request authentication engine to check ID authorization
 * (the ID have been associate to user account during sign-in phase)
 * @param[in] id A unique session ID
 * @param[in] authorization String of the operation
 * @parma[out] isGranted Will contains the result of authentication engine decisions
 * @return A T_com_status error code
 */
T_com_status webserver_op_checkAuthorization(const char *id, const char *authorization, NAL_BOOL *isGranted)
{
    T_com_status returnCode = COM_SUCCESS;

    /* Sanity check */
    VERIF_ARGS("id", id != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("authorization", authorization != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS("isGranted", isGranted != NULL, returnCode, COM_ERR_PARAM);
    if (returnCode == COM_ERR_PARAM)
    {
        TRACE_GEN(TRACE, "Invalid parameter", __FUNCTION__, __LINE__);
        goto exit;
    }

    /* Check authorization */
    returnCode = auth_check_authorization_in_session(id, authorization, (bool*)isGranted);

exit:
    return returnCode;

}

/**
 * @brief This function request authentication engine to change the security model
 * @param[in] data Contains new security model data (base64 encoded) 
 * @return A T_com_status error code
 */
T_com_status webserver_op_changeSecurityModel(const char *data)
{
    T_com_status returnCode = COM_SUCCESS;

    char *decodedData = NULL;
    size_t decodedDataSize = 0u;

    char *runningSecurityModelFilePath = NULL;
    
    T_tools_json_value *rawJsonData = NULL;

    /* Sanity check */
    VERIF_ARGS("data", data != NULL, returnCode, COM_ERR_PARAM);
    if (returnCode == COM_ERR_PARAM)
    {
        TRACE_GEN(TRACE, "Invalid parameter", __FUNCTION__, __LINE__);
        goto exit;
    }

    /* Decode base64 */
    if (CRYPTOAL_SUCCESS != cryptoal_base64Decode(data,
                                                  &decodedData,
                                                  &decodedDataSize))
    {
        TRACE_GEN(ERROR, "Decoding base64 data failed!", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_INTERNAL;
        goto exit;
    }

    /* Get security model file path */
    returnCode = entmgt_getRunningSecurityModelFilePath(&runningSecurityModelFilePath);
    if (COM_SUCCESS != returnCode)
    {
        TRACE_GEN(ERROR, "Getting security model failed!", __FUNCTION__, __LINE__);
        goto exit;
    }

    /* Write the new security model on filesystem */
    returnCode = entmgt_writeInternalFile(runningSecurityModelFilePath, 
                                  decodedData, 
                                  decodedDataSize);
    if (COM_SUCCESS != returnCode)
    {
        TRACE_GEN(ERROR, "Writing security model file failed!", __FUNCTION__, __LINE__);
        goto exit;
    }

    /* Parse the security model */
    returnCode = entmgt_parseInternalJsonBuffer(decodedData, decodedDataSize, &rawJsonData);
    if (returnCode != COM_SUCCESS)
    {
        TRACE_GEN(ERROR, "Parsing security model failed!", __FUNCTION__, __LINE__);
        goto exit;
    }
    
    /* Send new JSON data to RBAC engine */
    returnCode = auth_loadJsonBuffer_in_rbac_ram(rawJsonData);
    if (returnCode != COM_SUCCESS)
    {
        TRACE_GEN(WARN, "Loading new security model failed!, back on file Rbac", __FUNCTION__, __LINE__);

        /* TODO after fail, call auth function that put Rbac file in Rbac Ram */

    }
    else
    {
        /* TODO put json buffer in Rbac running file to keep new rbac */
    }

exit:

    if (NULL != runningSecurityModelFilePath)
    {
        free(runningSecurityModelFilePath);
        runningSecurityModelFilePath = NULL;
    }
    if (NULL != decodedData)
    {
        free(decodedData);
        decodedData = NULL;
    }
    /** 
     * @todo The following code have been comment because auth_loadJsonBuffer_in_rbac_ram()
     * releases the memory of rawJsonData. This not a convenient solution if I 
     * need to manipulate my rawJsonData, it will be interesting to change that.
     * In logical situation the caller is responsible to his allocated memory and child shall not touch that!
     */
    if(NULL != rawJsonData)
    {
//        tools_json_value_free(rawJsonData);
//        rawJsonData = NULL;
    }

    return returnCode;
}