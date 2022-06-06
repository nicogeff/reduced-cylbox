#ifndef TOOLS_CHAINEDLIST_H
#define TOOLS_CHAINEDLIST_H

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct tools_chainedListObject
    {
        void *payload;
        struct tools_chainedListObject *prev;
        struct tools_chainedListObject *next;
    } T_tools_chainedListObject;
    
    typedef struct tools_chainedListContext
    {
        T_tools_chainedListObject *header;
        T_tools_chainedListObject *footer;
        size_t count;
    } T_tools_chainedListContext;

    T_tools_status tools_chainedListCreate(T_tools_chainedListContext **ctx);
    T_tools_status tools_chainedListDestroy(T_tools_chainedListContext **ctx);
    
    T_tools_status tools_chainedListPushBack(T_tools_chainedListContext *ctx, void *payload);
    T_tools_status tools_chainedListPushFront(T_tools_chainedListContext *ctx, void *payload);
    T_tools_status tools_chainedListPushAt(T_tools_chainedListContext *ctx, void *payload);
    
    T_tools_status tools_chainedListPullBack(T_tools_chainedListContext *ctx, void **payload);
    T_tools_status tools_chainedListPullFront(T_tools_chainedListContext *ctx, void **payload);
    T_tools_status tools_chainedListPullAt(T_tools_chainedListContext *ctx, void **payload);
    
    T_tools_status tools_chainedListCount(T_tools_chainedListContext *ctx, size_t *count);
    T_tools_status tools_chainedListGetAt(T_tools_chainedListContext *ctx, size_t at, void **payload);
    

#ifdef __cplusplus
}
#endif

#endif /* TOOLS_CHAINEDLIST_H */

