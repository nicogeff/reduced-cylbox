
#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Basic.h>

#include "cryptoal_certificate_tests.h"

/*
 * CUnit Test Suite
 */

int init_suite(void)
{
  return 0;
}

int clean_suite(void)
{
  return 0;
}

int main()
{
  CU_pSuite pSuite = NULL;

  /* Initialize the CUnit test registry */
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  /* Add a suite to the registry */
  pSuite = CU_add_suite("cryptoal_certificate_tests", init_suite, clean_suite);
  if (NULL == pSuite)
  {
    CU_cleanup_registry();
    return CU_get_error();
  }

  /* Add the tests to the suite */
  if (  (NULL == CU_add_test(pSuite, "cryptoal_test_certificate_pkey_generation", cryptoal_test_certificate_pkey_generation)) ||
          (NULL == CU_add_test(pSuite, "cryptoal_test_certificate_signature_verification", cryptoal_test_certificate_signature_verification)) ||
          (NULL == CU_add_test(pSuite, "cryptoal_test_ca_chain_verification", cryptoal_test_ca_chain_verification))
      )
  {
    CU_cleanup_registry();
    return CU_get_error();
  }

  /* Run all tests using the CUnit Basic interface */
  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}
