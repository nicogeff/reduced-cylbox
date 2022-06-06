
#include <stdio.h>

#include "nal_mem.h"

#include "tools_trace.h"
#include "tools_type.h"

#include "tools_chainedList.h"

T_tools_status tools_chainedListCreate(T_tools_chainedListContext **ctx)
{
    T_tools_status returnCode = TOOLS_SUCCESS;

    VERIF_ARGS("ctx", ctx != NULL, returnCode, TOOLS_INVALID_PARAM);
    VERIF_ARGS("*ctx", *ctx == NULL, returnCode, TOOLS_INVALID_PARAM);
    if (returnCode != TOOLS_SUCCESS)
    {
        TRACE_GEN(ERROR, "Invalid parameter", __FUNCTION__, __LINE__);
        goto exit;
    }

    *ctx = NAL_MALLOC(sizeof (T_tools_chainedListContext));
    if (NULL == *ctx)
    {
        returnCode = TOOLS_MEM_ERROR;
        TRACE_GEN(ERROR, "Memory allocation failed", __FUNCTION__, __LINE__);
        goto exit;
    }
    memset(*ctx, 0, sizeof (T_tools_chainedListContext));

exit:
    return returnCode;
}

T_tools_status tools_chainedListDestroy(T_tools_chainedListContext **ctx)
{
    T_tools_status returnCode = TOOLS_SUCCESS;

    VERIF_ARGS("ctx", ctx != NULL, returnCode, TOOLS_INVALID_PARAM);
    VERIF_ARGS("*ctx", *ctx != NULL, returnCode, TOOLS_INVALID_PARAM);
    if (returnCode != TOOLS_SUCCESS)
    {
        TRACE_GEN(ERROR, "Invalid parameter", __FUNCTION__, __LINE__);
        goto exit;
    }

    if (0 != (*ctx)->count)
    {
        TRACE_GEN(WARN, "Chained list payload is not empty, pull before destroy it!", __FUNCTION__, __LINE__);
        returnCode = TOOLS_ERROR;
        goto exit;
    }

    NAL_FREE(*ctx);
    *ctx = NULL;

exit:
    return returnCode;
}

T_tools_status tools_chainedListPushBack(T_tools_chainedListContext *ctx, void *payload)
{
    T_tools_status returnCode = TOOLS_SUCCESS;

    T_tools_chainedListObject *curr = NULL;

    VERIF_ARGS("ctx", ctx != NULL, returnCode, TOOLS_INVALID_PARAM);
    VERIF_ARGS("payload", payload != NULL, returnCode, TOOLS_INVALID_PARAM);
    if (returnCode != TOOLS_SUCCESS)
    {
        TRACE_GEN(ERROR, "Invalid parameter", __FUNCTION__, __LINE__);
        goto exit;
    }

    /* Create object */
    curr = NAL_MALLOC(sizeof (T_tools_chainedListObject));
    if (NULL == curr)
    {
        returnCode = TOOLS_MEM_ERROR;
        TRACE_GEN(ERROR, "Memory allocation failed", __FUNCTION__, __LINE__);
        goto exit;
    }
    memset(curr, 0, sizeof (T_tools_chainedListObject));

    /* Add payload */
    curr->payload = payload;

    /* Empty chained list? */
    if (NULL == ctx->header && NULL == ctx->footer)
    {
        ctx->header = curr;
        ctx->footer = curr;
    }
    else
    {
        ctx->footer->next = curr;
        curr->prev = ctx->footer;
        ctx->footer = curr;
    }
    
    ctx->count++;

exit:
    return returnCode;
}

T_tools_status tools_chainedListPushFront(T_tools_chainedListContext *ctx, void *payload)
{
    T_tools_status returnCode = TOOLS_SUCCESS;

    T_tools_chainedListObject *curr = NULL;

    VERIF_ARGS("ctx", ctx != NULL, returnCode, TOOLS_INVALID_PARAM);
    VERIF_ARGS("payload", payload != NULL, returnCode, TOOLS_INVALID_PARAM);
    if (returnCode != TOOLS_SUCCESS)
    {
        TRACE_GEN(ERROR, "Invalid parameter", __FUNCTION__, __LINE__);
        goto exit;
    }

    /* Create object */
    curr = NAL_MALLOC(sizeof (T_tools_chainedListObject));
    if (NULL == curr)
    {
        returnCode = TOOLS_MEM_ERROR;
        TRACE_GEN(ERROR, "Memory allocation failed", __FUNCTION__, __LINE__);
        goto exit;
    }
    memset(curr, 0, sizeof (T_tools_chainedListObject));

    /* Add payload */
    curr->payload = payload;

    /* Empty chained list? */
    if (NULL == ctx->header && NULL == ctx->footer)
    {
        ctx->header = curr;
        ctx->footer = curr;
    }
    else
    {
        ctx->header->prev = curr;
        curr->next = ctx->header;
        ctx->header = curr;
    }
    
    ctx->count++;

exit:
    return returnCode;
}

T_tools_status tools_chainedListPullBack(T_tools_chainedListContext *ctx, void **payload)
{
    T_tools_status returnCode = TOOLS_SUCCESS;

    T_tools_chainedListObject *curr = NULL;

    VERIF_ARGS("ctx", ctx != NULL, returnCode, TOOLS_INVALID_PARAM);
    VERIF_ARGS("payload", payload != NULL, returnCode, TOOLS_INVALID_PARAM);
    VERIF_ARGS("*payload", *payload == NULL, returnCode, TOOLS_INVALID_PARAM);
    if (returnCode != TOOLS_SUCCESS)
    {
        TRACE_GEN(ERROR, "Invalid parameter", __FUNCTION__, __LINE__);
        goto exit;
    }

    curr = ctx->footer;

    *payload = curr->payload;

    ctx->footer = curr->prev;

    NAL_FREE(curr);
    curr = NULL;

    ctx->count--;

exit:
    return returnCode;
}

T_tools_status tools_chainedListPullFront(T_tools_chainedListContext *ctx, void **payload)
{
    T_tools_status returnCode = TOOLS_SUCCESS;

    T_tools_chainedListObject *curr = NULL;

    VERIF_ARGS("ctx", ctx != NULL, returnCode, TOOLS_INVALID_PARAM);
    VERIF_ARGS("payload", payload != NULL, returnCode, TOOLS_INVALID_PARAM);
    VERIF_ARGS("*payload", *payload == NULL, returnCode, TOOLS_INVALID_PARAM);
    if (returnCode != TOOLS_SUCCESS)
    {
        TRACE_GEN(ERROR, "Invalid parameter", __FUNCTION__, __LINE__);
        goto exit;
    }

    curr = ctx->header;

    *payload = curr->payload;

    ctx->header = curr->next;

    NAL_FREE(curr);
    curr = NULL;

    ctx->count--;

exit:
    return returnCode;
}

T_tools_status tools_chainedListCount(T_tools_chainedListContext *ctx, size_t *count)
{
    T_tools_status returnCode = TOOLS_SUCCESS;

    VERIF_ARGS("ctx", ctx != NULL, returnCode, TOOLS_INVALID_PARAM);
    VERIF_ARGS("count", count != NULL, returnCode, TOOLS_INVALID_PARAM);
    if (returnCode != TOOLS_SUCCESS)
    {
        TRACE_GEN(ERROR, "Invalid parameter", __FUNCTION__, __LINE__);
        goto exit;
    }
    
    *count = ctx->count;
    
exit:
    return returnCode;
}