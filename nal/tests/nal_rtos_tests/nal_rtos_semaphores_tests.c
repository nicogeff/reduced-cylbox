
#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Basic.h>

#include "nal_common.h"
#include "nal_rtos.h"
#include "nal_libc.h"
#include "nal_mem.h"

#include "nal_rtos_semaphores_tests.h"

#define NAL_TEST_SEMAPHORES_TASK_STACK_SIZE     16384 * 4
#define NAL_TEST_SEMAPHORES_TASK_PRIO           5

#define NAL_TEST_SEMAPHORES_NB_SEM_POST         10

/* Global variables for test */
T_nal_sem semTest1;
NAL_BOOL endTask1Sem = NAL_FALSE;
NAL_BOOL endTask2Sem = NAL_FALSE;
int counterSemaPost = 0;
int counterSemaWait = 0;

/* Test task 1 */
static void nal_test_semaphore_task1Sem(void *param)
{
    int result = NAL_SUCCESS;
    int i = 0;
    T_nal_time time;

    time.usec = 100000; /* 100ms */
    time.sec = 0;

    for (i = 0; i < NAL_TEST_SEMAPHORES_NB_SEM_POST; i++)
    {
        result = nal_semPost(&semTest1);
        CU_ASSERT_EQUAL(result, NAL_SUCCESS);
        
        counterSemaPost++;
        
        nal_sleep(&time); /* Cycle time */
    }

    endTask1Sem = NAL_TRUE;
    nal_taskExit();
}

/* Test task 2 */
static void nal_test_semaphore_task2Sem(void *param)
{
    int result = NAL_SUCCESS;
   
    while (!endTask1Sem)
    {
        result = nal_semWait(&semTest1, &NAL_WAIT_FOREVER);
        CU_ASSERT_EQUAL(result, NAL_SUCCESS);
        if (result == NAL_SUCCESS)
            counterSemaWait++;
    }
    
    endTask2Sem = NAL_TRUE;
    nal_taskExit();
}

/* Testing semaphores by create one, use it and delete it */
void nal_test_semaphores_simpleUsage(void)
{
    int result = NAL_SUCCESS;
    T_nal_sem semTest0;

    result = nal_semInit(&semTest0, 255, 0, "semTest0");
    CU_ASSERT_EQUAL(result, NAL_SUCCESS);

    result = nal_semPost(&semTest0);
    CU_ASSERT_EQUAL(result, NAL_SUCCESS);

    result = nal_semWait(&semTest0, &NAL_WAIT_FOREVER);
    CU_ASSERT_EQUAL(result, NAL_SUCCESS);

    result = nal_semDelete(&semTest0);
    CU_ASSERT_EQUAL(result, NAL_SUCCESS);
}

/* Testing semaphores by ... */
void nal_test_semaphores_threadUsage(void)
{
    int result = NAL_SUCCESS;
    T_nal_task task1Sem;
    T_nal_task task2Sem;
    T_nal_time time = {0u, 0u};
    int *task1SemStack = NULL;
    int *task2SemStack = NULL;

    /* Allocate task stack */
    task1SemStack = NAL_MALLOC(sizeof (int)*NAL_TEST_SEMAPHORES_TASK_STACK_SIZE);
    
    /* Create task 1 */
    result = nal_taskCreate(&task1Sem,
                            "SemTask1",
                            nal_test_semaphore_task1Sem,
                            NULL,
                            NAL_TEST_SEMAPHORES_TASK_PRIO,
                            task1SemStack,
                            NAL_TEST_SEMAPHORES_TASK_STACK_SIZE);
    CU_ASSERT_EQUAL(result, NAL_SUCCESS);

    /* Allocate task stack */
    task2SemStack = NAL_MALLOC(sizeof (int)*NAL_TEST_SEMAPHORES_TASK_STACK_SIZE);
    
    /* Create task 2 */
    result = nal_taskCreate(&task2Sem,
                            "SemTask2",
                            nal_test_semaphore_task2Sem,
                            NULL,
                            NAL_TEST_SEMAPHORES_TASK_PRIO,
                            task2SemStack,
                            NAL_TEST_SEMAPHORES_TASK_STACK_SIZE);
    CU_ASSERT_EQUAL(result, NAL_SUCCESS);
    
    time.usec = 100000u;
    time.sec = 0u;
    
    while(!endTask1Sem)
    {
        nal_sleep(&time);
    }
    
    /* Check counter */
    CU_ASSERT_EQUAL(counterSemaWait, counterSemaPost);
    
    /* Deleting task */
    nal_taskDelete(&task1SemStack);
    nal_taskDelete(&task2SemStack);
    
    /* Free allocate memory */
    NAL_FREE(task1SemStack);
    NAL_FREE(task2SemStack);
}