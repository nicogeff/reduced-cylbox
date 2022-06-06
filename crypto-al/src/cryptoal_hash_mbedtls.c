#include <stddef.h>

#include "com_error.h"
#include "com_entity.h"

#include "cryptoal_hash.h"

#ifdef CRYPTO_MBEDTLS

#include "mbedtls/config.h"
#include "mbedtls/md.h"

bool cryptoal_SHA256(void *data, unsigned long len, unsigned char *md)
{
    mbedtls_md_context_t context;
    mbedtls_md_type_t md_type = MBEDTLS_MD_SHA256;

    mbedtls_md_init(&context);
    mbedtls_md_setup(&context, mbedtls_md_info_from_type(md_type), 0);
    mbedtls_md_starts(&context);
    mbedtls_md_update(&context, (const unsigned char *)data, len);
    mbedtls_md_finish(&context, md);
    mbedtls_md_free(&context);

    return true;
}

#endif  /* CRYPTO_MBEDTLS */
