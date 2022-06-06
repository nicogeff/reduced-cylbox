#ifndef CRYPTOAL_RAND_H
#define CRYPTOAL_RAND_H

#ifdef __cplusplus
extern "C" {
#endif

#include "cryptoal_common.h"

#define CRYPTOAL_UUID_SIZE  38
    
    int cryptoal_uuidgen(char **buffer, size_t *bufferSize);

#ifdef __cplusplus
}
#endif

#endif /* CRYPTOAL_RAND_H */

