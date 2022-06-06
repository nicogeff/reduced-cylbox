#ifndef WEBSERVER_UTILS_H
#define WEBSERVER_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

    int webserver_utils_parse_http_variable(
            const char *buffer,
            const size_t bufferSize,
            const char *key,
            char *value,
            size_t valueSize);

    NAL_BOOL webserver_utils_isAnSsdpDiscoveryRequest(const char *buffer,
            const size_t bufferSize);

#ifdef __cplusplus
}
#endif

#endif /* WEBSERVER_UTILS_H */

