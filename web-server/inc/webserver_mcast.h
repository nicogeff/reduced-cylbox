#ifndef WEBSERVER_MCAST_H
#define WEBSERVER_MCAST_H

#ifdef __cplusplus
extern "C" {
#endif

void webserver_mcast_thread(void *data);

void webserver_mcast_stop(void);

#ifdef __cplusplus
}
#endif

#endif /* WEBSERVER_MCAST_H */

