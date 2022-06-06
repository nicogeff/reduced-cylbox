#ifndef TOOLS_TYPE_H
#define TOOLS_TYPE_H

#ifdef __cplusplus
extern "C"
{
#endif

/** 
 * \brief The following lines define the limits array size 
 */
#define TOOLS_BIG_ARRAY_SIZE      1024
#define TOOLS_MEDIUM_ARRAY_SIZE   128
#define TOOLS_SMALL_ARRAY_SIZE    64

/**
 * \brief This enumeration describe error codes usable in "tools" module
 */
typedef enum tools_status_enum
{
    TOOLS_SUCCESS = 0, /* Operation success */
    TOOLS_ERROR = 1, /* Operation error */
    TOOLS_INVALID_PARAM = 2, /* Invalid parameter */
    TOOLS_MEM_ERROR = 3, /* Invalid parameter */
    TOOLS_FILE_ERROR = 4 /* Error during file manipulation */
} T_tools_status;

typedef enum tools_boolean_enum
{
    TOOLS_FALSE = 0, /* FALSE */
    TOOLS_TRUE = 1 /* TRUE */
} T_tools_boolean;

#ifdef __cplusplus
}
#endif

#endif /* TOOLS_TYPE_H */

