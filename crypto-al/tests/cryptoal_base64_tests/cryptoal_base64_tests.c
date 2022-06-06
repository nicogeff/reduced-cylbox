
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <CUnit/Basic.h>

#include "com_error.h"
#include "nal_mem.h"
#include "cryptoal_base64.h"

#include "cryptoal_base64_tests.h"

/* */
void cryptoal_test_base64_encode(void)
{
    T_com_status returnCode = COM_SUCCESS;
    
    char inData[] = "Mais attendez, vous êtes interprète ou diplomate ? (Alexandre Astier - Kaamelott, L'interprète S01E24)";
    char outDataExpected[] = "TWFpcyBhdHRlbmRleiwgdm91cyDDqnRlcyBpbnRlcnByw6h0ZSBvdSBkaXBsb21hdGUgPyAoQWxleGFuZHJlIEFzdGllciAtIEthYW1lbG90dCwgTCdpbnRlcnByw6h0ZSBTMDFFMjQp";
    char *outData = {0};
    size_t outDataSize = 0u;
    
    returnCode = cryptoal_base64Encode(inData, strlen(inData), &outData, &outDataSize);
    CU_ASSERT_EQUAL(returnCode, COM_SUCCESS);
    if (NULL != outData)
        CU_ASSERT_STRING_EQUAL(outData, outDataExpected);
    
    NAL_FREE (outData);
}

/* */
void cryptoal_test_base64_decode(void)
{
    int returnCode = COM_SUCCESS;
    
    char inData[] = "SWwgZG9pdCB5IGF2b2lyIDgwIGxhcmJpbnMgYXUgY2jDonRlYXUgZXQgY+KAmWVzdCB2b3VzIHF1aSB2b3VzIHRhcGV6IGxhIHRhbWJvdWlsbGUgISAoTGlvbm5lbCBBc3RpZXIgLSBLYWFtZWxvdHQsIExhIFRhcnRlIGF1eCBNeXJ0aWxsZXMgUzAxRTAyKQ==";
    char outDataExpected[]= "Il doit y avoir 80 larbins au château et c’est vous qui vous tapez la tambouille ! (Lionnel Astier - Kaamelott, La Tarte aux Myrtilles S01E02)";
    char *outData = {0};
    size_t outDataSize = 0u;
    
    returnCode = cryptoal_base64Decode(inData, &outData, &outDataSize);
    CU_ASSERT_EQUAL(returnCode, COM_SUCCESS);
    if (outData != NULL)
        CU_ASSERT_STRING_EQUAL(outData, outDataExpected);
    
    NAL_FREE (outData);
}


/********************** Bouchons for tests ************************/
char theRootPathOfResources[] = "/tmp/";
char * entmgt_launcher_getAppRootPath ()
{
    return theRootPathOfResources;
}

