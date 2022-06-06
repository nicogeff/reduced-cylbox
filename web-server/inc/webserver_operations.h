#ifndef WEBSERVER_OPERATION_H
#define WEBSERVER_OPERATION_H

#ifdef __cplusplus
extern "C" {
#endif

    T_com_status webserver_op_createSession(char *id);
    T_com_status webserver_op_signIn(const char *id, const char *login, const char *password, NAL_BOOL *signinStatus);
    T_com_status webserver_op_signOut(const char *id);
    T_com_status webserver_op_checkAuthorization(const char *id, const char *authorization, NAL_BOOL *isGranted);
    T_com_status webserver_op_changeSecurityModel(const char *data);

#ifdef __cplusplus
}
#endif

#endif /* WEBSERVER_OPERATION_H */

