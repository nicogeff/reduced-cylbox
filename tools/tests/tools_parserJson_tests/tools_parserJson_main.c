#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Basic.h>

#include "tools_parserJson_tests.h"
#include "tools_parserJsonTools_tests.h"

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
    CU_pSuite pSuiteParserJson = NULL;
    CU_pSuite pSuiteParserJsonTools = NULL;

    /* Initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* Add a suite to the registry */
    pSuiteParserJson = CU_add_suite("tools_parserJson", init_suite, clean_suite);
    if (NULL == pSuiteParserJson)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    pSuiteParserJsonTools = CU_add_suite("tools_parserJsonTools", init_suite, clean_suite);
    if (NULL == pSuiteParserJsonTools)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Add the tests to the suite */
    if ((NULL == CU_add_test(pSuiteParserJson, "tools_parserJson_parse_test", tools_parserJson_parse_test)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    /* Add the tests to the suite */
    if ((NULL == CU_add_test(pSuiteParserJsonTools, "tools_json_grab_data_test", tools_json_grab_data_test)))
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

