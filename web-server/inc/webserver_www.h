#ifndef WEBSERVER_WWW_H
#define WEBSERVER_WWW_H

#ifdef __cplusplus
extern "C" {
#endif

    struct mg_connection webserver_getConnection(void);
    struct mg_bind_opts webserver_get_bind_opts(void);
    struct mg_serve_http_opts webserver_getHttpOpts(void);

    void webserver_www_thread(void *data);
    void webserver_www_stop(void);

#ifdef __cplusplus
}
#endif

#endif /* WEBSERVER_WWW_H */

