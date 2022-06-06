#ifndef CRYPTOAL_BASE64_H
#define CRYPTOAL_BASE64_H

#ifdef __cplusplus
extern "C" {
#endif

#include "cryptoal_common.h"

int cryptoal_base64Encode(const char *inData, const size_t inDataSize, char **outData, size_t *outDataSize);
int cryptoal_base64Decode(const char *inData, char **outData, size_t *outDataSize);

size_t crypoal_base64DecodedSize(const char *inData);

#ifdef __cplusplus
}
#endif

#endif /* CRYPTOAL_BASE64_H */

