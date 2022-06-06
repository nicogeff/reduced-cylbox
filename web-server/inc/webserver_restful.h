#ifndef WEBSERVER_RESTFUL_H
#define WEBSERVER_RESTFUL_H

#ifdef __cplusplus
extern "C" {
#endif

    int webserver_rest_call_metadata(struct mg_connection *nc, void *hm);
    int webserver_rest_call_create_session(struct mg_connection *nc, void *hm);
    int webserver_rest_call_signin(struct mg_connection *nc, void *hm);
    int webserver_rest_call_change_rbac(struct mg_connection *nc, void *hm);
    
#ifdef __cplusplus
}
#endif

#endif /* WEBSERVER_RESTFUL_H */

