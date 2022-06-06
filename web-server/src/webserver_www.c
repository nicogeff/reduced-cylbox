#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "civetweb.h"

#include "nal_common.h"
#include "nal_rtos.h"
#include "nal_mem.h"

#include "com_error.h"
#include "com_entity.h"

#include "tools_trace.h"
#include "tools_parserJson.h"
#include "tools_fileManip.h"
#include "tools_strManip.h"

#include "entmgt_utils.h"

#include "cryptoal_base64.h"

#include "webserver_mgt.h"
#include "webserver_restful.h"

//static struct mg_mgr g_webserver_mgr;
//static struct mg_connection *g_webserver_connection = NULL;
//static struct mg_bind_opts g_webserver_bindOpts;
//static struct mg_serve_http_opts g_webserver_httpOpts;

#if MG_ENABLE_SSL
static const char *g_webserver_mg_ssl_cert = NULL;
#endif

/**
 * \brief This function stop the thread : WWW
 */
void webserver_www_stop(void)
{

}

/*---------------------------------- Accessor --------------------------------*/

/**
 * @brief Get the web server connection data
 */
//struct mg_connection webserver_getConnection(void)
//{
//    return *g_webserver_connection;
//}

/**
 * @brief Get the web server binding option
 */
//struct mg_bind_opts webserver_getBindOpts(void)
//{
//    return g_webserver_bindOpts;
//}

/**
 * @brief Get the web server HTTP option
 */
//struct mg_serve_http_opts webserver_getHttpOpts(void)
//{
//    return g_webserver_httpOpts;
//}

/*---------------------------------- Thread ----------------------------------*/

#define NO_SSL

/**
 * @brief This thread provide a listener on a specific port for HTTP data exchange
 */
void webserver_www_thread(void *data)
{
    const char *options[] = {
        "document_root",
        get_WEBSERVER_CONF_WWW_ROOT_DIR(),
        "listening_ports",
        get_WEBSERVER_CONF_WWW_PORT(),
        "request_timeout_ms",
        "10000",
        "error_log_file",
        "",
#ifndef NO_SSL  /* @todo need to change folder and option */
        "ssl_certificate",
        "../../resources/cert/server.pem",
        "ssl_protocol_version",
        "3",
        "ssl_cipher_list",
        "DES-CBC3-SHA:AES128-SHA:AES128-GCM-SHA256",
#endif
        "enable_auth_domain_check",
        "no",
        0
    };

    struct mg_callbacks callbacks;
    struct mg_context *ctx = NULL;

    T_nal_time dly = {1, 0};

    /* Check if libcivetweb has been built with all required features. */
#ifndef NO_SSL
    if (!mg_check_feature(2))
    {
        TRACE_GEN(ERROR, "SSL support asked but libcivetweb build without", __FUNCTION__, __LINE__);
        return;
    }

    mg_init_library(MG_FEATURES_SSL);

#else

    mg_init_library(0);

#endif

    /* Callback will print error messages to console */
    memset(&callbacks, 0, sizeof (callbacks));

    /* Start civetweb web server */
    ctx = mg_start(&callbacks, 0, options);

    /* Check return value: */
    if (ctx == NULL)
    {
        TRACE_GEN(ERROR, "Cannot start CivetWeb - mg_start failed", __FUNCTION__, __LINE__);
        return;
    }

    mg_set_request_handler(ctx, "/api/v1/create-session", webserver_rest_call_create_session, NULL);
    mg_set_request_handler(ctx, "/api/v1/sign-in", webserver_rest_call_signin, NULL);
    mg_set_request_handler(ctx, "/api/v1/change-rbac", webserver_rest_call_change_rbac, NULL);

    /* Wait until the server should be closed */
    while (NAL_TRUE == g_webserver_www_thread_status)
    {
        nal_sleep(&dly);
    }

    mg_stop(ctx);
}
