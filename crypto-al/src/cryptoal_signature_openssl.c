#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "com_error.h"
#include "tools_trace.h"
#include "tools_fileManip.h"
#include "nal_mem.h"
#include "com_entity.h"

#include "com_cryptolib_header.h"

#include "cryptoal_hash.h"
#include "cryptoal_signature.h"


/* temporary solution to select crypto library: */
#if defined  CRYPTO_OPENSSL


/************************  Static functions  *****************************/



/************************  exported functions  *****************************/

/* Generate signature of the hash of given buffer. Algo are RSA2048 and SHA256.
 * Local certificate and key is used (belonging to us), and we can find it.
 * Signature string is malloced inside, and shall be freed by caller.
 * 
 * return COM_SUCCESS if execution is Ok, otherwise nothing is done.
*/
T_com_status cryptoal_hash_signature(void *input_buffer, unsigned long input_buffer_length, unsigned char ** ppSignature, size_t * pLength)
{
    T_com_status returnCode = COM_SUCCESS;;

    START_FUNCTION();

    VERIF_ARGS ("input_buffer pointer shall not be Null", input_buffer != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("length of input_buffer  shall not be Null", input_buffer_length != 0u, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("ppSignature pointer shall not be Null", ppSignature != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS (" * ppSignature pointer shall be Null",  (* ppSignature) == NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("pLength output pointer  shall not be Null", pLength != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS (" * pLength output value  shall be Null", (* pLength) == 0u, returnCode, COM_ERR_PARAM);
    if (returnCode == COM_ERR_PARAM)
    {
        goto exit;
    }

    TRACE_GEN(ERROR, "  Function not implemented for OPENSSL   !!!  ", __FUNCTION__, __LINE__);

    //TODO
exit:
    END_FUNCTION();
    return returnCode;
}

/* Check if given signature is related to the hash of given buffer. Algo are RSA2048 and SHA256.
 * External certificate is used (belonging to the nomad tool managing us, or to the CA authority), and we can find it.
 * So 2 cases of signature that we verify here: an Rbac given by nomad tool, or a patch given by the factory, that's why 3rd
 * param is a boolen: true for a patch to verify with CA-ROOT certificate, false for a conf or rbac from nomad toolthat is an intermediate CA.
 * Signature string come from the outside, nothing is done on it (not freed nor erased).
 * 
 * return COM_SUCCESS if execution is Ok and in this case (*pValid) is written (true if signature is valid), otherwise nothing is done.
*/
T_com_status cryptoal_verify_hash_signature( void *input_buffer, unsigned long input_buffer_length, bool ca_root,
                                     unsigned char * pSignature, size_t sign_len, bool * pValid)
{

    T_com_status returnCode = COM_SUCCESS;
    START_FUNCTION();

    VERIF_ARGS ("input_buffer pointer shall not be Null", input_buffer != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("length of input_buffer  shall not be Null", input_buffer_length != 0u, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("pSignature pointer shall not be Null", pSignature != NULL, returnCode, COM_ERR_PARAM);
    VERIF_ARGS ("sign_len  shall not be Null", sign_len != 0u, returnCode, COM_ERR_PARAM);
    VERIF_ARGS (" pValid pointer  shall be Null", pValid != NULL, returnCode, COM_ERR_PARAM);
    if (returnCode == COM_ERR_PARAM)
    {
        goto exit;
    }

    TRACE_GEN(ERROR, "  Function not implemented for OPENSSL   !!!  ", __FUNCTION__, __LINE__);

    //TODO
    
exit:
    END_FUNCTION();
    return returnCode;
}



/* Check if first certificate has well signed the second one. Algo is RSA2048 with SHA256 for both certificates, as
 * currently used in the cyber solution. Complete absolute path ended with name file shall be given for both.
 * Third parameter is a boolean returned, true if signature is good.
 * 
 * return COM_SUCCESS if execution is Ok,  otherwise nothing is done.
*/
T_com_status cryptoal_check_signature_2cert (char * signing_cert,  char * signed_cert, bool * valid)
{
    T_com_status returnCode = COM_SUCCESS;
    START_FUNCTION();

    TRACE_GEN(ERROR, "  Function not implemented for OPENSSL   !!!  ", __FUNCTION__, __LINE__);

    //TODO
exit:
    return returnCode;
}

#endif  // CRYPTO_OPENSSL
