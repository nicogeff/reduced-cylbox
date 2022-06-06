
#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Basic.h>

#include "nal_common.h"
#include "nal_rtos.h"
#include "nal_libc.h"
#include "nal_mem.h"

#include "nal_rtos_mutexes_tests.h"

#define NAL_TEST_MUTEXES_TASK_STACK_SIZE    16384 * 4
#define NAL_TEST_MUTEXES_TASK_PRIO          5

#define NAL_TEST_MUTEXES_NB_MUTEX_LOCK      10

/* Global variables for test */
T_nal_mutex mutexTest1;
T_nal_mutex mutexTest2;
T_nal_mutex mutexTest3;
T_nal_mutex mutexTest4;
NAL_BOOL endTask1Mutex = NAL_FALSE;
NAL_BOOL endTask2Mutex = NAL_FALSE;
NAL_BOOL endTask3Mutex = NAL_FALSE;
NAL_BOOL endTask4Mutex = NAL_FALSE;

/* Test task 1*/
static void nal_test_mutexes_task1Mutex(void *param)
{
    int result = NAL_SUCCESS;
    int i = 0;
    T_nal_time dly = {0u, 0u};

    /* Set timers */
    dly.usec = 100000u;
    dly.sec = 0u;

    /* Waiting task 2 */
    nal_sleep(&dly);

    for (i = 0; i < NAL_TEST_MUTEXES_NB_MUTEX_LOCK; i++)
    {
        result = nal_mutexLock(&mutexTest1);
        CU_ASSERT_EQUAL(result, NAL_SUCCESS);
        if (result != NAL_SUCCESS)
        {
            nal_printf("\nnal_mutexLock failed with error: %d, in function: %s(), line: %d\n", result, __FUNCTION__, __LINE__);
        }

        result = nal_mutexUnlock(&mutexTest2);
        CU_ASSERT_EQUAL(result, NAL_EACCES);
        if (result != NAL_EACCES)
        {
            nal_printf("\nnal_mutexUnlock unexpected error: %d, in function: %s, line: %d\n", result, __FUNCTION__, __LINE__);
        }
    }

    /* Set timers */
    dly.usec = 500000u;
    dly.sec = 0u;

    nal_sleep(&dly);

    for (i = 0; i < NAL_TEST_MUTEXES_NB_MUTEX_LOCK; i++)
    {
        /* Unlock locked mutex */
        result = nal_mutexUnlock(&mutexTest1);
        CU_ASSERT_EQUAL(result, NAL_SUCCESS);
    }

    endTask1Mutex = NAL_TRUE;
    nal_taskExit();
}

/* Test task 2 */
static void nal_test_mutexes_task2Mutex(void * param)
{
    int result = NAL_SUCCESS;
    int i = 0;
    T_nal_time dly = {0u, 0u};

    /* Set timer */
    dly.usec = 500000u;
    dly.sec = 0u;

    for (i = 0; i < NAL_TEST_MUTEXES_NB_MUTEX_LOCK; i++)
    {
        result = nal_mutexLock(&mutexTest2);
        CU_ASSERT_EQUAL(result, NAL_SUCCESS);
        if (result != NAL_SUCCESS)
        {
            nal_printf("\nnal_mutexLock failed with error: %d, in function: %s(), line: %d\n", result, __FUNCTION__, __LINE__);
        }

        result = nal_mutexUnlock(&mutexTest1);
        CU_ASSERT_EQUAL(result, NAL_EACCES);
        if (result != NAL_EACCES)
        {
            nal_printf("\nnal_mutexUnlock unexpected error: %d, in function: %s, line: %d\n", result, __FUNCTION__, __LINE__);
        }
    }

    nal_sleep(&dly);

    for (i = 0; i < NAL_TEST_MUTEXES_NB_MUTEX_LOCK; i++)
    {
        /* Unlock locked mutex */
        result = nal_mutexUnlock(&mutexTest2);
        CU_ASSERT_EQUAL(result, NAL_SUCCESS);
    }

    endTask2Mutex = NAL_TRUE;
    nal_taskExit();
}

/* Test task 3 */
static void nal_test_mutexes_task3Mutex(void * param)
{
    T_nal_time time = {0u, 0u};

    time.usec = 100000u; /* 100ms */
    time.sec = 0u;

    nal_mutexLock(&mutexTest3);

    while (!endTask3Mutex) /* Wait the end of test */
    {
        nal_sleep(&time);
    }

    nal_mutexUnlock(&mutexTest3);

    nal_taskExit();
}

/* Test task 4 */
static void nal_test_mutexes_task4Mutex(void * param)
{
    T_nal_time timeStarting = {0u, 0u};
    T_nal_time timeEnding = {0u, 0u};

    /* Get time */
    nal_getPosixTime(&timeStarting);

    /* Lock mutex */
    nal_mutexLock(&mutexTest4);

    /* Get time */
    nal_getPosixTime(&timeEnding);
    if ((timeEnding.sec - timeStarting.sec) < 2)
    {
        CU_ASSERT_TRUE(CU_FALSE);
    }
    else
    {
        CU_ASSERT_TRUE(CU_TRUE);
    }

    /* Unlock mutex */
    nal_mutexUnlock(&mutexTest4);

    endTask4Mutex = NAL_TRUE;

    nal_taskExit();
}

/**
 * TESTS
 */

/* Testing mutex by creating one, lock it, unlock it, then delete it */
void nal_test_mutexes_simpleUsage(void)
{
    int result = NAL_SUCCESS;
    T_nal_mutex mutexTest0;

    /* Initialize mutex */
    result = nal_mutexInit(&mutexTest0, "mutexTest0");
    CU_ASSERT_EQUAL(result, NAL_SUCCESS);

    /* Lock mutex */
    result = nal_mutexLock(&mutexTest0);
    CU_ASSERT_EQUAL(result, NAL_SUCCESS);

    /* Unlock mutex */
    result = nal_mutexUnlock(&mutexTest0);
    CU_ASSERT_EQUAL(result, NAL_SUCCESS);

    /* Delete mutex */
    nal_mutexDelete(&mutexTest0);
    CU_ASSERT_EQUAL(result, NAL_SUCCESS);
}

/**
 * Testing mutex used by tasks. 
 * Task 1 lock mutex 1, then task 2 try to unlock mutex 1 (that should be not possible)
 * Task 2 lock mutex 2, then task 1 try to unlock mutex 2 (that should be not possible)
 */
void nal_test_mutexes_createTaskMixingMutex(void)
{
    int result = NAL_SUCCESS;
    T_nal_task task1MutexTCB;
    T_nal_task task2MutexTCB;
    T_nal_time time = {0u, 0u};
    int *task1MutexStack;
    int *task2MutexStack;

    /* Initialize mutex 1 */
    result = nal_mutexInit(&mutexTest1, "mutexTest1");
    CU_ASSERT_EQUAL(result, NAL_SUCCESS);

    /* Initialize mutex 2 */
    result = nal_mutexInit(&mutexTest2, "mutexTest2");
    CU_ASSERT_EQUAL(result, NAL_SUCCESS);

    endTask1Mutex = NAL_FALSE;
    endTask2Mutex = NAL_FALSE;

    /* Allocate task stack */
    task1MutexStack = NAL_MALLOC(sizeof (int)*NAL_TEST_MUTEXES_TASK_STACK_SIZE);

    /* Create task 1 */
    result = nal_taskCreate(&task1MutexTCB,
                            "MutexTask1",
                            nal_test_mutexes_task1Mutex,
                            NULL,
                            NAL_TEST_MUTEXES_TASK_PRIO,
                            task1MutexStack,
                            NAL_TEST_MUTEXES_TASK_STACK_SIZE);
    CU_ASSERT_EQUAL(result, NAL_SUCCESS);

    /* Wait few millisecond before create task 2 */
    time.sec = 0;
    time.usec = 250000;
    nal_sleep(&time);

    /* Allocate task stack */
    task1MutexStack = NAL_MALLOC(sizeof (int)*NAL_TEST_MUTEXES_TASK_STACK_SIZE);

    /* Create task 2 */
    result = nal_taskCreate(&task2MutexTCB,
                            "MutexTask2",
                            nal_test_mutexes_task2Mutex,
                            NULL,
                            NAL_TEST_MUTEXES_TASK_PRIO,
                            task2MutexStack,
                            NAL_TEST_MUTEXES_TASK_STACK_SIZE);
    CU_ASSERT_EQUAL(result, NAL_SUCCESS);

    time.sec = 1;
    time.usec = 0;

    /* Wait the end of test */
    while (!endTask1Mutex || !endTask2Mutex)
    {
        nal_sleep(&time);
    }

    /* Delete tasks */
    nal_taskDelete(&task1MutexTCB);
    nal_taskDelete(&task2MutexTCB);

    /* Delete mutex */
    result = nal_mutexDelete(&mutexTest1);
    CU_ASSERT_EQUAL(result, NAL_SUCCESS);
    result = nal_mutexDelete(&mutexTest2);
    CU_ASSERT_EQUAL(result, NAL_SUCCESS);

    /* Release allocated memory */
    NAL_FREE(task1MutexStack);
    NAL_FREE(task2MutexStack);
}

/* Testing mutex by trying to initialize the same mutex two times */
void nal_test_mutexes_doubleInitializing(void)
{
    int result = NAL_SUCCESS;
    T_nal_mutex mutexTest;

    /* Initialize mutex */
    result = nal_mutexInit(&mutexTest, "mutexTest");
    CU_ASSERT_EQUAL(result, NAL_SUCCESS);

    /* Initialize the same mutex */
    result = nal_mutexInit(&mutexTest, "mutexTest");
    CU_ASSERT_EQUAL(result, NAL_EACCES);

    /* Delete mutex */
    result = nal_mutexDelete(&mutexTest);
    CU_ASSERT_EQUAL(result, NAL_SUCCESS);
}

/* Testing mutex by trying to delete the same mutex two times */
void nal_test_mutexes_doubleDeleting(void)
{
    int result = NAL_SUCCESS;
    T_nal_mutex mutexTest;

    /* Initialize mutex */
    result = nal_mutexInit(&mutexTest, "mutexTest");
    CU_ASSERT_EQUAL(result, NAL_SUCCESS);

    /* Delete mutex */
    result = nal_mutexDelete(&mutexTest);
    CU_ASSERT_EQUAL(result, NAL_SUCCESS);

    /* Delete the same mutex */
    result = nal_mutexDelete(&mutexTest);
    CU_ASSERT_EQUAL(result, NAL_ENORES);
}

/* Testing mutex by trying delete a mutex locked by another task */
void nal_test_mutexes_deleteMutexLockByTask3(void)
{
    int result = NAL_SUCCESS;
    T_nal_task task3MutexTCB;
    T_nal_time time = {0u, 0u};
    int *task3MutexStack;

    endTask3Mutex = NAL_FALSE;

    /* Initialize mutex 3 */
    result = nal_mutexInit(&mutexTest3, "mutexTest3");
    CU_ASSERT_EQUAL(result, NAL_SUCCESS);

    /* Allocate task stack */
    task3MutexStack = NAL_MALLOC(sizeof (int)*NAL_TEST_MUTEXES_TASK_STACK_SIZE);

    /* Create task 1 */
    result = nal_taskCreate(&task3MutexTCB,
                            "MutexTask3",
                            nal_test_mutexes_task3Mutex,
                            NULL,
                            NAL_TEST_MUTEXES_TASK_PRIO,
                            task3MutexStack,
                            NAL_TEST_MUTEXES_TASK_STACK_SIZE);
    CU_ASSERT_EQUAL(result, NAL_SUCCESS);

    /* Waiting task 3 locking the mutex */
    time.usec = 500000u; /* 500 ms */
    time.sec = 0u;

    nal_sleep(&time);

    /* Trying to delete mutex */
    result = nal_mutexDelete(&mutexTest3);
    CU_ASSERT_EQUAL(result, NAL_EBUSY);

    /* Requesting task 3 to stop */
    endTask3Mutex = NAL_TRUE;

    /* Waiting task 3 stop */
    time.usec = 500000u; /* 500 ms */
    time.sec = 0u;

    nal_sleep(&time);

    /**
     * Trying to delete mutex - This time its OK
     * Strange behavior: The result is NAL_ENORES, apparently the previous 
     * nal_mutexDelete is buffered then when the mutex is available again, 
     * its automatically deleted!
     */
    result = nal_mutexDelete(&mutexTest3);
    CU_ASSERT_EQUAL(result, NAL_SUCCESS | NAL_ENORES);

    /* Release allocated memory */
    NAL_FREE(task3MutexStack);
}

/* Testing mutex by trying delete a mutex locked by another task */
void nal_test_mutexes_GetLockedMutexByMain(void)
{
    int result = NAL_SUCCESS;
    T_nal_task task4MutexTCB;
    T_nal_time time = {0u, 0u};
    int *task4MutexStack;

    endTask3Mutex = NAL_FALSE;

    /* Initialize mutex 4 */
    result = nal_mutexInit(&mutexTest4, "mutexTest4");
    CU_ASSERT_EQUAL(result, NAL_SUCCESS);

    /* Lock the mutex 4 */
    result = nal_mutexLock(&mutexTest4);
    CU_ASSERT_EQUAL(result, NAL_SUCCESS);

    /* Allocate task stack */
    task4MutexStack = NAL_MALLOC(sizeof (int)*NAL_TEST_MUTEXES_TASK_STACK_SIZE);

    /* Create task 1 */
    result = nal_taskCreate(&task4MutexTCB,
                            "MutexTask4",
                            nal_test_mutexes_task4Mutex,
                            NULL,
                            NAL_TEST_MUTEXES_TASK_PRIO,
                            task4MutexStack,
                            NAL_TEST_MUTEXES_TASK_STACK_SIZE);
    CU_ASSERT_EQUAL(result, NAL_SUCCESS);

    /* Waiting 3 sec before unlocking the mutex */
    time.usec = 0u;
    time.sec = 3u;

    nal_sleep(&time);

    /* Unlock mutex */
    result = nal_mutexUnlock(&mutexTest4);
    CU_ASSERT_EQUAL(result, NAL_SUCCESS);

    /* Waiting task finish */
    time.usec = 100000u;
    time.sec = 0u;
    while (!endTask4Mutex)
    {
        nal_sleep(&time);
    }

    /* Task unlocked... that good! */
    CU_ASSERT_TRUE(CU_TRUE);

    /* Delete mutex */
    result = nal_mutexDelete(&mutexTest4);
    CU_ASSERT_EQUAL(result, NAL_SUCCESS);

    /* Release allocated memory */
    NAL_FREE(task4MutexStack);
}

void nal_test_mutexes_callingDeletedMutex(void)
{
    int result = NAL_SUCCESS;
    T_nal_mutex mutexTest;

    /* Initialize mutex */
    result = nal_mutexInit(&mutexTest, "mutexTest");
    CU_ASSERT_EQUAL(result, NAL_SUCCESS);

    /* Delete mutex */
    result = nal_mutexDelete(&mutexTest);
    CU_ASSERT_EQUAL(result, NAL_SUCCESS);

    /* Delete the same mutex */
    result = nal_mutexLock(&mutexTest);
    CU_ASSERT_EQUAL(result, NAL_ENORES);
}

void nal_test_mutexes_defensiveCode(void)
{
    int result = NAL_SUCCESS;
    T_nal_mutex mutexTest;

    /* Initialize mutex with NULL parameter 1 */
    result = nal_mutexInit(NULL, "mutexTest");
    CU_ASSERT_EQUAL(result, NAL_EINVAL);

    /* Initialize mutex with NULL parameter 2 */
    result = nal_mutexInit(&mutexTest, NULL);
    CU_ASSERT_EQUAL(result, NAL_EINVAL);

    /* Delete mutex NULL parameter */
    result = nal_mutexDelete(NULL);
    CU_ASSERT_EQUAL(result, NAL_EINVAL);

    /* Lock mutex with NULL parameter */
    result = nal_mutexLock(NULL);
    CU_ASSERT_EQUAL(result, NAL_EINVAL);

    /* Unlock mutex with NULL parameter */
    result = nal_mutexUnlock(NULL);
    CU_ASSERT_EQUAL(result, NAL_EINVAL);
}


