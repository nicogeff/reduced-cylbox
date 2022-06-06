
#ifndef NAL_MEM_H_
#define NAL_MEM_H_

#include "nal_common.h"

#if defined NAL_MEM_ALLOC
#if defined NAL_INTERNAL_ALLOC
#include "nal_mem_alloc.h"
#else
#include <stdlib.h>
#endif
#else
#error "No OS defined ..."
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef NAL_USE_INTERNAL_ALLOC

#include "nal_mem_alloc.h"
#include "nal_stdlib.h"

extern void* mymspace;

#ifndef DSA_POOL_SIZE
#error "Macro DSA_POOL_SIZE must be set (size of memory buffer in bytes) when using AL_USE_AL_MALLOC"
#endif

#ifndef NAL_MALLOC
#define NAL_MALLOC(x)       (void *)mspace_malloc (mymspace, (x)) 
#endif
#ifndef NAL_FREE
#define NAL_FREE(x)         mspace_free(mymspace, (x))
#endif
#ifndef NAL_REALLOC
#define NAL_REALLOC(x,y)    mspace_realloc(mymspace, (x),(y))
#endif
#ifndef NAL_CALLOC
#define NAL_CALLOC(x,y)     mspace_calloc(mymspace, (x),(y))
#endif

#else 

#ifndef NAL_MALLOC
#define NAL_MALLOC(n)  malloc((n))
#endif
#ifndef NAL_FREE
#define NAL_FREE(p) { if(p) free(p); (p)=NULL; }
#endif
#ifndef NAL_REALLOC
#define NAL_REALLOC(x,y) realloc((x),(y))
#endif
#ifndef NAL_CALLOC
#define NAL_CALLOC(x,y)  calloc((x),(y))
#endif

#endif /* NAL_USE_INTERNAL_ALLOC */

#ifdef __cplusplus
}
#endif

#endif /* NAL_MEM_H_ */
