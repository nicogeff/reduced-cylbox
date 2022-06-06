#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Basic.h>

#include "tools_type.h"
#include "tools_strManip.h"

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

void tools_concatStrings_test01(void)
{
    T_tools_status result = TOOLS_SUCCESS;

    result = tools_concatStrings(NULL, 2, "parameter1", "parameter2");
    CU_ASSERT_EQUAL(result, TOOLS_INVALID_PARAM);
}

void tools_concatStrings_test02(void)
{
    T_tools_status result = TOOLS_SUCCESS;

    char * out1 = NULL;

    result = tools_concatStrings(&out1, 2, NULL, NULL);
    CU_ASSERT_EQUAL(result, TOOLS_INVALID_PARAM);
}

void tools_concatStrings_test03(void)
{
    T_tools_status result = TOOLS_SUCCESS;

    char * out3 = (char *) malloc(4 * sizeof (char));

    result = tools_concatStrings(&out3, 2, "parameter1", "parameter2");
    CU_ASSERT_EQUAL(result, TOOLS_INVALID_PARAM);

    free(out3);
}

void tools_concatStrings_test04(void)
{
    T_tools_status result = TOOLS_SUCCESS;

    char * out2 = "NotNull";

    /* How to manage a local allocated string -> Segmentation fault */
    result = tools_concatStrings(&out2, 2, "parameter1", "parameter2");
    CU_ASSERT_EQUAL(result, TOOLS_INVALID_PARAM);
}

void tools_concatStrings_test05(void)
{
    T_tools_status result = TOOLS_SUCCESS;

    char * out1 = NULL;

    result = tools_concatStrings(&out1, 0, NULL);
    CU_ASSERT_EQUAL(result, TOOLS_INVALID_PARAM);
}

void tools_concatStrings_test06(void)
{
    T_tools_status result = TOOLS_SUCCESS;

    char * out1 = NULL;

    result = tools_concatStrings(&out1, 2, NULL);
    CU_ASSERT_EQUAL(result, TOOLS_INVALID_PARAM);
}

void tools_concatStrings_test07(void)
{
    /* T_tools_status result = TOOLS_SUCCESS; */
    /* How to manage this case -> Segmentation fault */
    /* res = tools_ConcatStrings(&out1, 2, "parameter1"); */
}

void tools_concatStrings_test08(void)
{
    /* T_tools_status result = TOOLS_SUCCESS; */
    /* How to manage this case -> Segmentation fault */
    /* res = tools_ConcatStrings(&out1, 2, "parameter1", "parameter2", "parameter3"); */
}

int main()
{
    CU_pSuite pSuite = NULL;

    /* Initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* Add a suite to the registry */
    pSuite = CU_add_suite("test_tools_strManip", init_suite, clean_suite);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Add the tests to the suite */
    if (
            (NULL == CU_add_test(pSuite, "tools_concatStrings_test01", tools_concatStrings_test01)) ||
            (NULL == CU_add_test(pSuite, "tools_concatStrings_test02", tools_concatStrings_test02)) ||
            (NULL == CU_add_test(pSuite, "tools_concatStrings_test03", tools_concatStrings_test03)) ||
            (NULL == CU_add_test(pSuite, "tools_concatStrings_test04", tools_concatStrings_test04)) ||
            (NULL == CU_add_test(pSuite, "tools_concatStrings_test05", tools_concatStrings_test05)) ||
            (NULL == CU_add_test(pSuite, "tools_concatStrings_test06", tools_concatStrings_test06)) ||
            (NULL == CU_add_test(pSuite, "tools_concatStrings_test07", tools_concatStrings_test07)) ||
            (NULL == CU_add_test(pSuite, "tools_concatStrings_test08", tools_concatStrings_test08))
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
