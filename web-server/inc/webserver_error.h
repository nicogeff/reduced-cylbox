#ifndef WEBSERVER_ERROR_H
#define WEBSERVER_ERROR_H

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * \brief Restful error code
     */
#define WEBSERVER_REST_SUCCESS             0

#define WEBSERVER_REST_EPARAM              (-1)

#define WEBSERVER_REST_EINTERNAL           (-10)
#define WEBSERVER_REST_ECREDENTIAL         (-11)
#define WEBSERVER_REST_EUNAUTHORIZED       (-12)
#define WEBSERVER_REST_ESESSIONEXPIRED     (-13)

#ifdef __cplusplus
}
#endif

#endif /* WEBSERVER_ERROR_H */

