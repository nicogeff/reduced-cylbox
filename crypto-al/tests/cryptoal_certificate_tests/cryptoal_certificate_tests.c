
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <CUnit/Basic.h>

#include "nal_mem.h"

#include "com_error.h"
#include "com_entity.h"

#include "com_cryptolib_header.h"
#include "cryptoal_rand.h"
#include "cryptoal_certificate.h"
#include "cryptoal_certificate_tests.h"

/* Generate auto-signed certificate/keypair, and verify its content */
void cryptoal_test_certificate_pkey_generation(void)
{

    int returnCode = COM_SUCCESS;
    int ret = 0;
    char path[128];
    unsigned char * pBuffer1 = NULL;
    unsigned char * pBuffer2 = NULL;
    unsigned char * pBuffer3 = NULL;
    size_t len1 = 0u;
    size_t len2 = 0u;
    size_t len3 = 0u;
    int diff = 0;
    
    printf ("\nTest autosigned certificate and keypair generation, used in degraded mode\n");
    returnCode = cryptoal_generate_autosigned_local_certificate_and_keys ();
    CU_ASSERT_EQUAL(returnCode, COM_SUCCESS);

    returnCode = cryptoal_put_certificate_in_readable_buffer ("./tests/cryptoal_certificate_tests/certs/local/box.cert.pem", & pBuffer1, & len1);
    CU_ASSERT_EQUAL(returnCode, COM_SUCCESS);
    printf ("%s", pBuffer1);

    /* verify certificate content */
    ret = strstr (pBuffer1, "version     : 3");
    CU_ASSERT_EQUAL((ret==0), false )   /*ret shall be diffrent of zero*/
    ret = strstr (pBuffer1, "issuer name       : C=FR, ST=aquitaine, L=merignac, O=CYLBOX, CN=cylbox_autosigned");
    CU_ASSERT_EQUAL((ret==0), false )
    ret = strstr (pBuffer1, "subject name      : C=FR, ST=aquitaine, L=merignac, O=CYLBOX, CN=cylbox_autosigned");
    CU_ASSERT_EQUAL((ret==0), false )
    ret = strstr (pBuffer1, "expires on        : 2030-01-01 12:00:00");
    CU_ASSERT_EQUAL((ret==0), false )
    ret = strstr (pBuffer1, "signed using      : RSA with SHA-256");
    CU_ASSERT_EQUAL((ret==0), false )
    ret = strstr (pBuffer1, "RSA key size      : 2048 bits");
    CU_ASSERT_EQUAL((ret==0), false )
    ret = strstr (pBuffer1, "cert. type        : SSL Client, SSL Server, Object Signing");
    CU_ASSERT_EQUAL((ret==0), false )

    /* Generate a second certificate and verfy it is different (only public key is different) */
    returnCode = cryptoal_generate_autosigned_local_certificate_and_keys ();
    CU_ASSERT_EQUAL(returnCode, COM_SUCCESS);

    returnCode = cryptoal_put_certificate_in_readable_buffer ("./tests/cryptoal_certificate_tests/certs/local/box.cert.pem", & pBuffer2, & len2);
        
    CU_ASSERT_EQUAL(returnCode, COM_SUCCESS);
    CU_ASSERT_EQUAL(len1, len2);
    diff = memcpy (pBuffer1, pBuffer2, len1);
    CU_ASSERT_EQUAL((diff==0), false )

    NAL_FREE (pBuffer1);
    NAL_FREE (pBuffer2);

    returnCode = cryptoal_put_keypair_in_readable_buffer ("./tests/cryptoal_certificate_tests/certs/local/box.key.pem", & pBuffer3, & len3);
    CU_ASSERT_EQUAL(returnCode, COM_SUCCESS);
    printf ("%s", pBuffer3);

    NAL_FREE (pBuffer3);
    
}

void cryptoal_test_certificate_signature_verification(void)
{
    int returnCode = COM_SUCCESS;
    bool valid = false;

    printf ("\nTest signature validity between 2 simple certificates\n");

    /*  Here CA is really issuer*/
    returnCode = cryptoal_check_signature_2cert ( "./tests/cryptoal_certificate_tests/chain/root/ca.cert.pem",
                                                 "./tests/cryptoal_certificate_tests/chain/children/children.cert.pem",
                                                 & valid);
    CU_ASSERT_EQUAL(returnCode, COM_SUCCESS);
    CU_ASSERT_EQUAL(valid, true);

    /*  Here another CA has signed the certificate*/
    returnCode = cryptoal_check_signature_2cert ("./tests/cryptoal_certificate_tests/chain/root/ca.cert.pem",
                                                 "./tests/cryptoal_certificate_tests/chain/children/children.cert_bad_key.pem",
                                                 & valid);
    CU_ASSERT_EQUAL(returnCode, COM_SUCCESS);
    CU_ASSERT_EQUAL(valid, false);

   /*  Here another CA has signed the certificate*/
    returnCode = cryptoal_check_signature_2cert ("./tests/cryptoal_certificate_tests/chain/root/ca.cert_bad_key.pem",
                                                 "./tests/cryptoal_certificate_tests/chain/children/children.cert.pem",
                                                 & valid);
    CU_ASSERT_EQUAL(returnCode, COM_SUCCESS);
    CU_ASSERT_EQUAL(valid, false);

    /* ca certificate DO NOT exist */
    returnCode = cryptoal_check_signature_2cert ("./tests/cryptoal_certificate_tests/chain/ca.cert.pem",  
                                                 "./tests/cryptoal_certificate_tests/chain/children/children.cert_expired.pem",
                                                 & valid);
    CU_ASSERT_EQUAL(returnCode, COM_ERR_INTERNAL);

}

void cryptoal_test_ca_chain_verification(void)
{
    T_com_status returnCode = COM_SUCCESS;
    bool ca_valid = false;

    printf ("\nTest the validation of complete CA chain, as expected by application at start, on many tests vectors\n");

    entmgt_launcher_setAppRootPath("./tests/cryptoal_certificate_tests/ca_chain/good/");
    returnCode = cryptoal_check_ca_chain_certificates ( & ca_valid);
    CU_ASSERT_EQUAL(returnCode, COM_SUCCESS);
    CU_ASSERT_EQUAL(ca_valid, true);

    /* Now test on missing certificate or keypair .. */

    entmgt_launcher_setAppRootPath("./tests/cryptoal_certificate_tests/ca_chain/missing_local_key/");
    returnCode = cryptoal_check_ca_chain_certificates ( & ca_valid);
    CU_ASSERT_EQUAL(returnCode, COM_SUCCESS);
    CU_ASSERT_EQUAL(ca_valid, false);

    entmgt_launcher_setAppRootPath("./tests/cryptoal_certificate_tests/ca_chain/missing_local_cert/");
    returnCode = cryptoal_check_ca_chain_certificates ( & ca_valid);
    CU_ASSERT_EQUAL(returnCode, COM_SUCCESS);
    CU_ASSERT_EQUAL(ca_valid, false);

    entmgt_launcher_setAppRootPath("./tests/cryptoal_certificate_tests/ca_chain/missing_cloud_cert/");
    returnCode = cryptoal_check_ca_chain_certificates ( & ca_valid);
    CU_ASSERT_EQUAL(returnCode, COM_SUCCESS);
    CU_ASSERT_EQUAL(ca_valid, false);

    entmgt_launcher_setAppRootPath("./tests/cryptoal_certificate_tests/ca_chain/missing_tool_cert/");
    returnCode = cryptoal_check_ca_chain_certificates ( & ca_valid);
    CU_ASSERT_EQUAL(returnCode, COM_SUCCESS);
    CU_ASSERT_EQUAL(ca_valid, false);

    entmgt_launcher_setAppRootPath("./tests/cryptoal_certificate_tests/ca_chain/missing_root_cert/");
    returnCode = cryptoal_check_ca_chain_certificates ( & ca_valid);
    CU_ASSERT_EQUAL(returnCode, COM_SUCCESS);
    CU_ASSERT_EQUAL(ca_valid, false);

    entmgt_launcher_setAppRootPath("./tests/cryptoal_certificate_tests/ca_chain/good/");
    returnCode = cryptoal_check_ca_chain_certificates ( & ca_valid);
    CU_ASSERT_EQUAL(returnCode, COM_SUCCESS);
    CU_ASSERT_EQUAL(ca_valid, true);

    /* Now test of bad signatures .. */

    entmgt_launcher_setAppRootPath("./tests/cryptoal_certificate_tests/ca_chain/bad_signature_local/");
    returnCode = cryptoal_check_ca_chain_certificates ( & ca_valid);
    CU_ASSERT_EQUAL(returnCode, COM_SUCCESS);
    CU_ASSERT_EQUAL(ca_valid, false);

    entmgt_launcher_setAppRootPath("./tests/cryptoal_certificate_tests/ca_chain/expired_cert_local/");
    returnCode = cryptoal_check_ca_chain_certificates ( & ca_valid);
    CU_ASSERT_EQUAL(returnCode, COM_SUCCESS);
    CU_ASSERT_EQUAL(ca_valid, false);


}

/*************** Bouchons for tests ****************/
char theRootPathOfResources[256] = "./tests/cryptoal_certificate_tests/";

const char * entmgt_launcher_getAppRootPath()
{
    return theRootPathOfResources;
}
void   entmgt_launcher_setAppRootPath(char * new_path_string)
{
    strcpy (theRootPathOfResources, new_path_string);
}

