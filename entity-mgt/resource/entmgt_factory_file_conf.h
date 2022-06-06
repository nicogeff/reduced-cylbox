#ifndef FACTORY_FILE_CONF_H
#define FACTORY_FILE_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/* Conf factory content: becomes running Conf if none at starting. Here no need to hash and signature since file isn't transfered */
/* Do not forget that conf shall be decided by main application, here is just a set of default values */
char g_factory_file_conf[] = "{\
  \"localConfiguration\": {\
    \"version\": 1,\
    \"http://www.devsec.com/devsec/entity-entity-mgt\": {\
      \"settings\": [\
        {\
          \"ENTMGT_DEVICE_HOSTNAME\": \"box-indus-001\"\
        },\
        {\
          \"ENTMGT_DEVICE_IP_ADDR\": \"192.168.0.1\"\
        },\
        {\
          \"ENTMGT_DEVICE_SYSTEM\": \"Cylbox\"\
        },\
        {\
          \"ENTMGT_DEVICE_LOCATION\": \"localB12minor\"\
        }\
      ]\
    },\
    \"http://www.devsec.com/devsec/auth_rbac\": {\
      \"settings\": [\
        {\
          \"AUTH_RBAC_SESSION_DURATION_SEC\": 20\
        },\
        {\
          \"AUTH_RBAC_LOCKACCOUNT_DURATION_SEC\": 60\
        }\
      ]\
    },\
  }\
}";

#ifdef __cplusplus
}
#endif

#endif /* FACTORY_FILE_CONF_H */

