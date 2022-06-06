
#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Basic.h>

#include "nal_rtos_mutexes_tests.h"
#include "nal_rtos_semaphores_tests.h"

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
    CU_pSuite pSuiteMutex = NULL;
    CU_pSuite pSuiteSemaphore = NULL;

    /* Initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* Add a suite to the registry */
    pSuiteMutex = CU_add_suite("nal_rtos_mutexes_tests", init_suite, clean_suite);
    if (NULL == pSuiteMutex)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Add a suite to the registry */
    pSuiteSemaphore = CU_add_suite("nal_rtos_semaphores_tests", init_suite, clean_suite);
    if (NULL == pSuiteSemaphore)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Add the tests to the suite */
    if (
            /* Mutexes */
            (NULL == CU_add_test(pSuiteMutex, "nal_test_mutexes_simpleUsage", nal_test_mutexes_simpleUsage)) ||
            (NULL == CU_add_test(pSuiteMutex, "nal_test_mutexes_createTaskMixingMutex", nal_test_mutexes_createTaskMixingMutex)) ||
            (NULL == CU_add_test(pSuiteMutex, "nal_test_mutexes_doubleInitializing", nal_test_mutexes_doubleInitializing)) ||
            (NULL == CU_add_test(pSuiteMutex, "nal_test_mutexes_doubleDeleting", nal_test_mutexes_doubleDeleting)) ||
            (NULL == CU_add_test(pSuiteMutex, "nal_test_mutexes_deleteMutexLockByTask3", nal_test_mutexes_deleteMutexLockByTask3)) ||
            (NULL == CU_add_test(pSuiteMutex, "nal_test_mutexes_callingDeletedMutex", nal_test_mutexes_callingDeletedMutex)) ||
            (NULL == CU_add_test(pSuiteMutex, "nal_test_mutexes_defensiveCode", nal_test_mutexes_defensiveCode)) ||
            (NULL == CU_add_test(pSuiteMutex, "nal_test_mutexes_GetLockedMutexByMain", nal_test_mutexes_GetLockedMutexByMain)) ||

            /* Semaphores */
            (NULL == CU_add_test(pSuiteSemaphore, "nal_test_semaphores_simpleUsage", nal_test_semaphores_simpleUsage)) ||
            (NULL == CU_add_test(pSuiteSemaphore, "nal_test_semaphores_threadUsage", nal_test_semaphores_threadUsage))
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
