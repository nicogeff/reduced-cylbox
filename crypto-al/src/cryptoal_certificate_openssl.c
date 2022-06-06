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
#include "cryptoal_certificate.h"

#ifdef  CRYPTO_OPENSSL


//============================   GLobal functions   =============================//


/* Generate local certificate file and public/private keys in 2 files. Algo is RSA2048 with SHA256, used for signing and TLS client or server.
 * We have all information inside it, and name an path of files.
 * This is done when no certificate exists at init, it will be replaced by a signed one when enrolled by the nomad tool.
 * It is assumed target directory exists.
 * 
 * return COM_SUCCESS if execution is Ok,  otherwise nothing is done.
*/
T_com_status cryptoal_generate_autosigned_local_certificate_and_keys ()
{
    T_com_status returnCode = COM_SUCCESS;

    START_FUNCTION();

    if (tools_buildFilePath(rootPath, ENTMGT_DIR_CERTIFICATE_LOCAL, ENTMGT_NAME_CERTIFICATE_LOCAL, & cryptoCertificatePath) != TOOLS_SUCCESS)
    {
        TRACE_GEN(ERROR, "Building crypto folder path failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_NO_MEMORY;
        goto exit;
    }
    if (tools_buildFilePath(rootPath, ENTMGT_DIR_CERTIFICATE_LOCAL, ENTMGT_NAME_KEY_LOCAL, & cryptoKeyPath) != TOOLS_SUCCESS)
    {
        TRACE_GEN(ERROR, "Building crypto folder path failed", __FUNCTION__, __LINE__);
        returnCode = COM_ERR_NO_MEMORY;
        goto exit;
    }

    /* TODO */

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

    /* TODO : find a function or calcule everything */

exit:
    END_FUNCTION();
    return returnCode;
}


/* Check chain of confidence of all  certificates, assuming here they all exist.
 * See detail in documention on CA-chain.
 * It is assumed target directories also exist.
 *
 * If  one or more signature is not correct, false is returned in out parameter.
 * 
 * return COM_SUCCESS if execution is Ok,  otherwise nothing is done.
*/
T_com_status cryptoal_check_ca_chain_certificates (bool * pIs_ca_correct)
{
    T_com_status returnCode = COM_SUCCESS;

    START_FUNCTION();
    VERIF_ARGS (" pIs_ca_correct shall not be Null", pIs_ca_correct != NULL, returnCode, COM_ERR_PARAM);
    if (returnCode == COM_ERR_PARAM)
    {
        goto exit;
    }

    /* TODO: check certificates chain */
    
    * pIs_ca_correct = true;
exit:
    END_FUNCTION();
    return returnCode;
}


#endif  // CRYPTO_OPENSSL

