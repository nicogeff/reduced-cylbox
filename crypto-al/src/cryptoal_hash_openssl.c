#include <stddef.h>

#include "com_error.h"
#include "com_entity.h"

#include "cryptoal_hash.h"

#ifdef  CRYPTO_OPENSSL

#include "openssl/sha.h"

bool cryptoal_SHA256(void *data, unsigned long len, unsigned char *md)
{
    SHA256_CTX context;
    
    if(!SHA256_Init(&context))
        return false;
    if(!SHA256_Update(&context, (unsigned char*)data, len))
        return false;
    if(!SHA256_Final(md, &context))
        return false;
    
    return true;
}

#endif  /* CRYPTO_OPENSSL */

