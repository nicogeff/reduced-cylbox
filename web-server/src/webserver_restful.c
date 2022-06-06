#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>

#include "civetweb.h"

#include "nal_common.h"
#include "nal_libc.h"

#include "com_error.h"

#include "webserver_www.h"
#include "webserver_error.h"
#include "webserver_restful.h"
#include "webserver_operations.h"
#include "webserver_utils.h"

/**
 * @brief Web service: Provide some information on the hardware / software 
 */
int webserver_rest_call_metadata(struct mg_connection *nc, void *hm)
{
    /* @todo not implemented yet */
}

/**
 * @brief This web service provide a way to create a session
 */
int webserver_rest_call_create_session(struct mg_connection *nc, void *hm)
{
    int restReturnCode = WEBSERVER_REST_SUCCESS;
    T_com_status returnCode = COM_SUCCESS;

    char id[PATH_MAX] = {0};
    char buffer[PATH_MAX] = {0};
    size_t bufferSize = 0u;

    /* Web service action: Create a session */
    returnCode = webserver_op_createSession(id);
    if (COM_SUCCESS != returnCode)
        restReturnCode = WEBSERVER_REST_EINTERNAL;

    /* Prepare response */
    nal_snprintf_s(buffer,
                   PATH_MAX,
                   PATH_MAX,
                   &bufferSize,
                   "{ \"id\": \"%s\", \"result\": %d }",
                   id,
                   restReturnCode);

    /* Header */
    mg_send_http_ok(nc, "application/json; charset=utf8", bufferSize);

    /* Body */
    mg_write(nc, buffer, bufferSize);
}

/**
 * @brief This web service provide a way to sign-in
 */
int webserver_rest_call_signin(struct mg_connection *nc, void *hm)
{
    int restReturnCode = WEBSERVER_REST_SUCCESS;
    int returnCode = 0;

    int readDataSize = 0;
    char buffer[PATH_MAX] = {0};
    size_t bufferSize = 0u;

    char id[PATH_MAX] = {0};
    char login[PATH_MAX] = {0};
    char password[PATH_MAX] = {0}; /* SHA256 password expected */
    NAL_BOOL signinStatus = NAL_FALSE;

    /* Read restful body */
    readDataSize = mg_read(nc, buffer, sizeof (buffer) - 1);
    if ((readDataSize < 1) || (readDataSize >= sizeof (buffer)))
    {
        mg_send_http_error(nc, 400, "%s", "Invalid request body data");
        returnCode = -1;
        goto exit;
    }

    /* Get restful parameter */
    if (0 > webserver_utils_parse_http_variable(buffer, readDataSize, "id", id, sizeof (id)))
    {
        mg_send_http_error(nc, 400, "%s", "Invalid request body data");
        returnCode = -1;
        goto exit;
    }
    if (0 > webserver_utils_parse_http_variable(buffer, readDataSize, "login", login, sizeof (login)))
    {
        mg_send_http_error(nc, 400, "%s", "Invalid request body data");
        returnCode = -1;
        goto exit;
    }
    if (0 > webserver_utils_parse_http_variable(buffer, readDataSize, "password", password, sizeof (password)))
    {
        mg_send_http_error(nc, 400, "%s", "Invalid request body data");
        returnCode = -1;
        goto exit;
    }

    /* Reset buffer */
    memset(buffer, 0, PATH_MAX);

    /* Authenticate */
    returnCode = webserver_op_signIn(id, login, password, &signinStatus);
    if (COM_SUCCESS == returnCode)
    {
        if (!signinStatus)
            restReturnCode = WEBSERVER_REST_ECREDENTIAL;
    }
    else
        restReturnCode = WEBSERVER_REST_EINTERNAL;

    /* Prepare response */
    nal_snprintf_s(buffer,
                   PATH_MAX,
                   PATH_MAX,
                   &bufferSize,
                   "{ \"result\": %d }",
                   restReturnCode);

    /* Header */
    mg_send_http_ok(nc, "application/json; charset=utf8", bufferSize);

    /* Body */
    mg_write(nc, buffer, bufferSize);

exit:
    return returnCode;
}

/**
 * @brief This web service provide a way to change a security model
 */
int webserver_rest_call_change_rbac(struct mg_connection *nc, void *hm)
{
    int restReturnCode = WEBSERVER_REST_SUCCESS;
    int returnCode = 0;

    int readDataSize = 0;
    char buffer[PATH_MAX] = {0};
    size_t bufferSize = 0u;

    char id[PATH_MAX] = {0};
    char data[PATH_MAX] = {0};
    NAL_BOOL isGranted = NAL_FALSE;

    /* Read restful body */
    readDataSize = mg_read(nc, buffer, sizeof (buffer) - 1);
    if ((readDataSize < 1) || (readDataSize >= sizeof (buffer)))
    {
        mg_send_http_error(nc, 400, "%s", "Invalid request body data");
        returnCode = -1;
        goto exit;
    }

    /* Get restful parameter */
    if (0 > webserver_utils_parse_http_variable(buffer, readDataSize, "id", id, sizeof (id)))
    {
        mg_send_http_error(nc, 400, "%s", "Invalid request body data");
        returnCode = -1;
        goto exit;
    }
    if (0 > webserver_utils_parse_http_variable(buffer, readDataSize, "data", data, sizeof (data)))
    {
        mg_send_http_error(nc, 400, "%s", "Invalid request body data");
        returnCode = -1;
        goto exit;
    }

    /* Reset buffer */
    memset(buffer, 0, PATH_MAX);

    /* Ask authorization */
    returnCode = webserver_op_checkAuthorization(id, "change-rbac", &isGranted);
    if (COM_SUCCESS == returnCode)
    {
        if (isGranted)
        {
            returnCode = webserver_op_changeSecurityModel(data);
            if (COM_SUCCESS != returnCode)
                restReturnCode = WEBSERVER_REST_EINTERNAL;
        }
        else
            restReturnCode = WEBSERVER_REST_EUNAUTHORIZED;
    }
    else
        restReturnCode = WEBSERVER_REST_EINTERNAL;

    /* Prepare response */
    nal_snprintf_s(buffer,
                   PATH_MAX,
                   PATH_MAX,
                   &bufferSize,
                   "{ \"result\": %d }",
                   restReturnCode);

    /* Header */
    mg_send_http_ok(nc, "application/json; charset=utf8", bufferSize);

    /* Body */
    mg_write(nc, buffer, bufferSize);

exit:
    return returnCode;
}
