
#include <stdio.h>
#include <stdlib.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "cryptoal_tls_tests.h"

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
  {
    return CU_get_error();
  }

  /* Add a suite to the registry */
  pSuite = CU_add_suite("cryptoal_tls_tests", init_suite, clean_suite);
  if (NULL == pSuite)
  {
    CU_cleanup_registry();
    return CU_get_error();
  }

  /* Add the tests to the suite */
  if (NULL == CU_add_test(pSuite, "cryptoal_test_tls_cnx_exchange", cryptoal_test_tls_cnx_exchange))
  {
    CU_cleanup_registry();
    return CU_get_error();
  }
    /* Add the tests to the suite */
  if (NULL == CU_add_test(pSuite, "cryptoal_test_tls_cnx_big_traffic", cryptoal_test_tls_cnx_big_traffic))
  {
    CU_cleanup_registry();
    return CU_get_error();
  }
  /* Add the tests to the suite */
  if (NULL == CU_add_test(pSuite, "cryptoal_test_tls_cnx_big_packet", cryptoal_test_tls_cnx_big_packet))
  {
    CU_cleanup_registry();
    return CU_get_error();
  }
  /* Add the tests to the suite */
  if (NULL == CU_add_test(pSuite, "cryptoal_test_tls_cnx_bad_certificate", cryptoal_test_tls_cnx_bad_certificate))
  {
    CU_cleanup_registry();
    return CU_get_error();
  }
  
  /* Run all tests using the CUnit Basic interface */
  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return 0;   //   NGE TEST    CU_get_error();
}
