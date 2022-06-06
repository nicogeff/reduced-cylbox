#ifndef FACTORY_FILE_RBAC_H
#define FACTORY_FILE_RBAC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Rbac factory content: becomes running Rbac if none at starting. Here no need to hash and signature since file isn't transfered */
/* Do not forget that Rbac shall be decided by nomad tool, here is just a set of default values */
    char g_factory_file_rbac[] =
"{\"rbac\":{\"version\":1,\"users\":[{\"user\":\"installer\",\"hashedPassword\":\"6a51c8231013a106bed56fd2d6084f07851de165bccafec5672dbd24d623fa96\",\"role-idx\":[0],\"inactif\":false}],\"roles\":[{\"role\":\"installation\",\"author-idx\":[0]}],\"authorizations\":[\"change-rbac\"]}}";



#ifdef __cplusplus
}
#endif

#endif /* FACTORY_FILE_RBAC_H */

