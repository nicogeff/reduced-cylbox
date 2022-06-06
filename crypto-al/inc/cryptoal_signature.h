#ifndef CRYPTOAL_SIGNATURE_H
#define CRYPTOAL_SIGNATURE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "cryptoal_common.h"


/*==================== Definitions ====================*/


    
/*================= Exported functions ================*/
    
    
/* Generate signature of the hash of given buffer. Algo are RSA2048 and SHA256.
 * Local certificate and key is used (belonging to us), and we can find it.
 * Signature string is malloced inside, and shall be freed by caller.
 * 
 * return COM_SUCCESS if execution is Ok, otherwise nothing is done.
*/
#define MBEDTLS_PK_SIGNATURE_MAX_SIZE 256
T_com_status cryptoal_hash_signature(void *input_buffer, unsigned long input_buffer_length, unsigned char ** ppSignature, size_t * pLength);

/* Check if given signature is related to the hash of given buffer. Algo are RSA2048 and SHA256.
 * External certificate is used (belonging to the nomad tool managing us, or to the CA authority), and we can find it.
 * So 2 cases of signature that we verify here: an Rbac given by nomad tool, or a patch given by the factory, that's why 3rd
 * param is a boolen: true for a patch to verify with CA-ROOT certificate, false for a conf or rbac from nomad toolthat is an intermediate CA.
 * Signature string come from the outside, nothing is done on it (not freed nor erased).
 * 
 * return COM_SUCCESS if execution is Ok and in this case (*pValid) is written
 *     (true if signature is valid), otherwise nothing is done.
*/
T_com_status cryptoal_verify_hash_signature( void *input_buffer, unsigned long input_buffer_length,
                                      bool ca_root, unsigned char * pSignature, size_t sign_len, bool * pValid);



#ifdef __cplusplus
}
#endif

#endif /* CRYPTOAL_SIGNATURE_H */

