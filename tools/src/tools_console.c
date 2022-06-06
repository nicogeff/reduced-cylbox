
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

#include "tools_type.h"
#include "tools_console.h"

/**
 * This function allow to print unlimited parameter
 * @param fmt[in]
 * @param ...[in]
 * @return \n
 * TOOLS_SUCCESS on success \n
 * TOOLS_ERROR on error \n
 */
T_tools_status tools_console_out(char * fmt, ...)
{
    T_tools_status res = TOOLS_SUCCESS;
    va_list args;

    if (fmt == NULL)
    {
        res = TOOLS_ERROR;
    }
    else
    {
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);

        /* To force output even without a carriage return at 
         * the end of the message 
         */
        fflush(stdout);

        res = TOOLS_SUCCESS;
    }
    return res;
}

/**
 * 
 * @param fmt[in] Format character
 * @param param[out] Pointer on the variable to fill in
 * @param sz[in] Size of the available memory in the variable param, in bytes.
 * @return \n
 * TOOLS_SUCCESS on success \n
 * TOOLS_ERROR on error \n
 */
T_tools_status tools_console_get(const char* fmt, void* param, unsigned int sz)
{
    T_tools_status res = TOOLS_SUCCESS;

    if ((fmt == NULL) || (param == NULL))
    {
        res = TOOLS_ERROR;
    }
    else
    {
        if (scanf(fmt, param) == EOF)
        {
            if (errno == ENOMEM)
            {
                res = TOOLS_ERROR;
                return res;
            }
        }
    }
    return res;
}