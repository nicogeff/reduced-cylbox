#ifndef CRYPTOAL_HASH_H
#define CRYPTOAL_HASH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "cryptoal_common.h"

    bool cryptoal_SHA256(void *input, unsigned long length, unsigned char *md);

#ifdef __cplusplus
}
#endif

#endif /* CRYPTOAL_HASH_H */

