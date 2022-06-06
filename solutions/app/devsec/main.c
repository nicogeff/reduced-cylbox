
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <signal.h>
#include <unistd.h>

#include "com_error.h"
#include "com_entity.h"

#include "tools_trace.h"
#include "tools_strManip.h"

#include "entmgt_launcher.h"

/**
 * \brief Socket that order "application stop"
 * \note Following path shall correctly settled
 */
#define STOP_SOCKET_PATH "stop_socket"



/**
 * \brief Intercept SIGPIPE hanging signals
 * \remark: dummy static function
 */
static void handler_sigpipe(int name) {
}

/**
 * \brief Stops properly the application from outside.
 * \details uses UNIX Domain socket
 * \remark private
 */
static T_com_status waitForAppStopping(void) {

    T_com_status returnCode = COM_SUCCESS;
    
    int sock, s2, length;
    struct sockaddr_un localSocket, remoteSocket;
    socklen_t t;
    
    char * fullSocketPath = NULL;
    const char * rootPath = entmgt_launcher_getAppRootPath();
    
    /* Build the full path for the socket path */
    if (tools_concatStrings(&fullSocketPath, 2u, rootPath, STOP_SOCKET_PATH) != TOOLS_SUCCESS)
    {
        TRACE_GEN(WARN, "String concatenation failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_NO_MEMORY;
        goto exit;
    }
    
    if ((sock = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        returnCode = COM_ERR_INTERNAL;
        goto exit;
    }

    strcpy(localSocket.sun_path, fullSocketPath); /* @todo Better to use nal */
    localSocket.sun_family = AF_UNIX;
    unlink(localSocket.sun_path);
    length = strlen(localSocket.sun_path) + sizeof (localSocket.sun_family);

    if (bind(sock, (struct sockaddr *) &localSocket, length) == -1) {
        returnCode = COM_ERR_INTERNAL;
        goto exit;
    }

    if (listen(sock, 2) == -1) {
        returnCode = COM_ERR_INTERNAL;
        goto exit;
    }

    for (;;) {
        t = sizeof (remoteSocket);
        if ((s2 = accept(sock, (struct sockaddr *) &remoteSocket, &t)) == -1) {
            returnCode = COM_ERR_INTERNAL;
            goto exit;
        }
        /* Socket has received anything */

        /* Stop application */
        entmgt_launcher_stop();
        close(s2);
        break;
    }
    close(sock);

    /* Remove socket */
    unlink(localSocket.sun_path);

exit:
    if (fullSocketPath != NULL) { free(fullSocketPath); fullSocketPath = NULL; }
    return returnCode;
}

#if 0

/* Print directory of running application */
static void printCurrentExeDir(void) {
    char cwd[4096] = {0};

    getcwd(cwd, sizeof (cwd));
    printf("Current Exec file: %s", cwd);
}
#endif /* 0 */

/**
 * \brief Main of the application
 */
int main(int argc, char** argv) {
    T_com_status returnCode = COM_SUCCESS;

    /* Set SIGPIPE handler, avoid application to crash */
    signal(SIGPIPE, handler_sigpipe);

    /* Initialize application */
    returnCode = entmgt_launcher_initialize();
    if (returnCode != COM_SUCCESS) {
        goto exit;
    }

    /* Configure application  */
    returnCode = entmgt_launcher_configure();
    if (returnCode != COM_SUCCESS) {
        goto exit;
    }

    /* Start application */
    returnCode = entmgt_launcher_start();
    if (returnCode != COM_SUCCESS) {
        goto exit;
    }

    /* Stops the application, if waited synchro occur on external dedicated socket */
    returnCode = waitForAppStopping();
    if (returnCode != COM_SUCCESS) {
        goto exit;
    }

exit:
    if (returnCode != COM_SUCCESS) {
        /* Any  error, stops by force application */
        entmgt_launcher_stop();
    }

    return 0;
}

