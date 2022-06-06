#include <stdio.h>
#include <CUnit/Basic.h>

#include "tools_type.h"
#include "tools_chainedList.h"

#include "tools_chainedList_tests.h"
#include "tools_type.h"

void tools_test_chainedListInitAndDestroy(void)
{
    T_tools_status result = TOOLS_SUCCESS;
    
    T_tools_chainedListContext *ctx = NULL;

    result = tools_chainedListCreate(&ctx);
    CU_ASSERT_EQUAL(result, TOOLS_SUCCESS);
    CU_ASSERT_PTR_NOT_NULL(ctx);
    
    result = tools_chainedListDestroy(&ctx);
    CU_ASSERT_EQUAL(result, TOOLS_SUCCESS);
    CU_ASSERT_PTR_NULL(ctx);
}

void tools_test_chainedListPushAndPull(void)
{
    T_tools_status result = TOOLS_SUCCESS;
    
    T_tools_chainedListContext *ctx = NULL;
    
    char *payload00 = "payload#00";
    char *payload01 = "payload#01";
    char *payload02 = "payload#02";
    
    void *ptr00 = NULL;
    void *ptr01 = NULL;
    void *ptr02 = NULL;
    
    size_t count = 0u;
    
    result = tools_chainedListCreate(&ctx);
    CU_ASSERT_EQUAL(result, TOOLS_SUCCESS);
    CU_ASSERT_PTR_NOT_NULL(ctx);
    
    /* Add object */
    result = tools_chainedListPushBack(ctx, (void *)payload01);
    CU_ASSERT_EQUAL(result, TOOLS_SUCCESS);
    
    result = tools_chainedListCount(ctx, &count);
    CU_ASSERT_EQUAL(result, TOOLS_SUCCESS);
    CU_ASSERT_EQUAL(count, 1);
    
    result = tools_chainedListPushBack(ctx, (void *)payload02);
    CU_ASSERT_EQUAL(result, TOOLS_SUCCESS);
    
    result = tools_chainedListCount(ctx, &count);
    CU_ASSERT_EQUAL(result, TOOLS_SUCCESS);
    CU_ASSERT_EQUAL(count, 2);
    
    result = tools_chainedListPushFront(ctx, (void*)payload00);
    CU_ASSERT_EQUAL(result, TOOLS_SUCCESS);
    
    result = tools_chainedListCount(ctx, &count);
    CU_ASSERT_EQUAL(result, TOOLS_SUCCESS);
    CU_ASSERT_EQUAL(count, 3);
    
    result = tools_chainedListPullBack(ctx, (void**)&ptr02);
    CU_ASSERT_EQUAL(result, TOOLS_SUCCESS);
    CU_ASSERT_STRING_EQUAL((char*)ptr02, payload02);
    
    result = tools_chainedListCount(ctx, &count);
    CU_ASSERT_EQUAL(result, TOOLS_SUCCESS);
    CU_ASSERT_EQUAL(count, 2);
    
    result = tools_chainedListPullFront(ctx, (void**)&ptr00);
    CU_ASSERT_EQUAL(result, TOOLS_SUCCESS);
    CU_ASSERT_STRING_EQUAL((char*)ptr00, payload00);
    
    result = tools_chainedListCount(ctx, &count);
    CU_ASSERT_EQUAL(result, TOOLS_SUCCESS);
    CU_ASSERT_EQUAL(count, 1);
    
    result = tools_chainedListPullBack(ctx, (void**) &ptr01);
    CU_ASSERT_EQUAL(result, TOOLS_SUCCESS);
    CU_ASSERT_STRING_EQUAL((char*)ptr01, payload01);
    
    result = tools_chainedListCount(ctx, &count);
    CU_ASSERT_EQUAL(result, TOOLS_SUCCESS);
    CU_ASSERT_EQUAL(count, 0);
    
    result = tools_chainedListDestroy(&ctx);
    CU_ASSERT_EQUAL(result, TOOLS_SUCCESS);
    CU_ASSERT_PTR_NULL(ctx);
}

