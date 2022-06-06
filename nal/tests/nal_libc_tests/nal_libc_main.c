
#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Basic.h>

#include "nal_libc_tests.h"

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
    pSuite = CU_add_suite("nal_stdlib_tests", init_suite, clean_suite);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Add the tests to the suite */
    if (
            (NULL == CU_add_test(pSuite, "nal_test_print_simple_string", nal_test_print_simple_string)) ||
            (NULL == CU_add_test(pSuite, "nal_test_print_a_formated_string", nal_test_print_a_formated_string)) ||
            (NULL == CU_add_test(pSuite, "nal_test_print_utf8_string", nal_test_print_utf8_string)) ||
            (NULL == CU_add_test(pSuite, "nal_test_print_big_string", nal_test_print_big_string)) ||
            (NULL == CU_add_test(pSuite, "nal_test_write_simple_string", nal_test_write_simple_string))
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
