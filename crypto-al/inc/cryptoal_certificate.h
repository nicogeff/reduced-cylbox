#ifndef CRYPTOAL_CERTIFICATE_H
#define CRYPTOAL_CERTIFICATE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "cryptoal_common.h"


/*==================== Definitions ====================*/

#define GENERATED_CERT_SUBJECT  "C=FR, ST=aquitaine, L=merignac, O=CYLBOX, CN=cylbox_autosigned"

    
/*================= Exported functions ================*/


/* Suppress all certificate and keypair of the CA chain. We know its structure.
 * To bo called when CA is failing, and then generate an autosigned certificate in degraded mode.
 * 
 * returns nothing.
 */
/* Exported in common header file "com_cryptolib_header":
 * void cryptoal_ca_chain_clean_certificates (void);
 */


/* Check chain of confidence of all  certificates, assuming here they all exist.
 * See detail in documention on CA-chain.
 * It is assumed target directories also exist.
 *
 * If  one or more signature is not correct, false is returned in out parameter.
 * 
 * return COM_SUCCESS if execution is Ok,  otherwise nothing is done.
*/
/* Exported in common header file "com_cryptolib_header":
 * T_com_status cryptoal_check_ca_chain_certificates (bool * pIs_ca_correct);
 */


/* Generate local certificate file and public/private keys in 2 files. Algo is RSA2048 with SHA256, used for signing and TLS client or server.
 * We have all information inside it, and name an path of files.
 * This is done when no certificate exists at init, it will be replaced by a signed one when enrolled by the nomad tool.
 * It is assumed target directory exists.
 * 
 * return COM_SUCCESS if execution is Ok,  otherwise nothing is done.
*/
T_com_status cryptoal_generate_autosigned_local_certificate_and_keys ();


/* Check if first certificate has well signed the second one. Algo is RSA2048 with SHA256 for both certificates, as
 * currently used in the cyber solution. Complete absolute or relative path ended with name file shall be given for both.
 * Third parameter is a boolean returned, true if first certficate are really signed the second one..
 * 
 * return COM_SUCCESS if execution is Ok,  otherwise nothing is done nor returned.
*/
T_com_status cryptoal_check_signature_2cert (const char * ca_signing_cert,  const char * signed_cert, bool * pValid);


#if 1
/*These 2 functions maybe be suppressed in operational mode. Used for certificate TU */
/* Put certificate in a malloced buffer  and in a file cert.info in /tmp. Buffer shall be freed bycaller.
 * return COM_SUCCESS if execution is Ok,  otherwise nothing is done.
*/
T_com_status cryptoal_put_certificate_in_readable_buffer (const char * fileName, unsigned char ** ppBuffer, size_t *pLen);

/*
 *  Put keypair in a malloced buffer and in a file keypair.info (in /tmp). Buffer shall be freed bycaller.
 * return COM_SUCCESS if execution is Ok,  otherwise nothing is done.
*/
T_com_status cryptoal_put_keypair_in_readable_buffer (const char * fileName, unsigned char ** ppBuffer, size_t *pLen);
#endif


#ifdef __cplusplus
}
#endif

#endif /* CRYPTOAL_CERTIFICATE_H */

