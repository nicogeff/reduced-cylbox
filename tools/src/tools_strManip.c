
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "tools_trace.h"
#include "tools_strManip.h"

/**
 * This function can be use to concat several of string
 * @param[in/out] out A pointer to get the result of concat
 * @param[in] count A number of string to concat
 * @param[in] str Strings to concat
 * @remark free() should be used to deallocate memory
 * @return \n
 * TOOLS_SUCCESS on success
 * TOOLS_ERROR on error
 */
T_tools_status tools_concatStrings(char** out, size_t count, const char* str, ...)
{
    T_tools_status res = TOOLS_SUCCESS;
    va_list pa;
    char ** argList = NULL;
    char* in = NULL;
    size_t len = 0u;
    size_t lenMax = 0u;
    size_t idx = 0u;

    /* Sanity check */
    VERIF_ARGS("out", out != NULL, res, TOOLS_INVALID_PARAM);
    VERIF_ARGS("*out", *out == NULL, res, TOOLS_INVALID_PARAM);
    VERIF_ARGS("str", str != NULL, res, TOOLS_INVALID_PARAM);
    VERIF_ARGS("count", count > 0, res, TOOLS_INVALID_PARAM);
    if (res != TOOLS_SUCCESS)
    {
        TRACE_GEN(TRACE, "Invalid parameter", __FUNCTION__, __LINE__);
        goto exit;
    }

    /* Allocate nb element */
    argList = malloc(count * sizeof (char*));
    if (argList == NULL)
    {
        res = TOOLS_ERROR;
        goto exit;
    }

    /* Start the list */
    va_start(pa, str);

    /* Size of the first element */
    len = strlen(str);

    /* Memorize this first element */
    argList[0] = malloc(len * sizeof (char) + 1u);
    if (argList[0] == NULL)
    {
        res = TOOLS_ERROR;
        goto exit;
    }
    memset(argList[0], 0, (len * sizeof (char)) + 1u);
    memcpy(argList[0], str, len * sizeof (char));

    lenMax += len;

    /* Manage the others elements */
    for (idx = 1u; idx < count; idx++)
    {
        in = va_arg(pa, char *);

        if (in == NULL)
        {
            res = TOOLS_ERROR;
            goto exit;
        }

        len = strlen(in);

        argList[idx] = malloc(len * sizeof (char) + 1u);
        if (argList[idx] == NULL)
        {
            res = TOOLS_ERROR;
            goto exit;
        }
        memset(argList[idx], 0, (len * sizeof (char)) + 1u);
        memcpy(argList[idx], in, len * sizeof (char));

        lenMax += len;
    }

    /* Close the list */
    va_end(pa);

    /* Allocate memory for the output */
    (*out) = malloc((lenMax * sizeof (char)) + 1u);
    if (*out == NULL)
    {
        res = TOOLS_ERROR;
        goto exit;
    }
    memset(*out, 0, (lenMax * sizeof (char)) + 1u);

    /* Copy all elements */
    for (idx = 0u; idx < count; idx++)
    {
        strncat(*out, argList[idx], lenMax + 1);
    }

exit:

    /* Free memory */
    if (argList != NULL)
    {
        for (idx = 0u; idx < count; idx++)
        {
            if (argList[idx] != NULL)
            {
                free(argList[idx]);
            }
        }

        free(argList);
    }

    if ((res != TOOLS_SUCCESS) && (res != TOOLS_INVALID_PARAM) &&
            (out != NULL) && (*out != NULL))
    {
        free(*out);
    }

    return res;
}

/**
 * This function can be use to search if the end of a string character is present
 * @param[in] in String to scan
 * @param[in] character Character to search into the string
 * @return True if the character is found, otherwise false is returned
 */
T_tools_boolean tools_isCharacterPresentAtEnd(const char* in, const char character)
{
    T_tools_boolean res = TOOLS_FALSE;
    size_t inLen = 0u;

    START_FUNCTION();

    /* Sanity check */
    if (in == NULL)
    {
        res = TOOLS_FALSE;
        goto exit;
    }

    inLen = strlen(in);
    if (0u == inLen)
    {
        res = TOOLS_FALSE;
    }
    else if (in[inLen - 1u] == character)
    {
        res = TOOLS_TRUE;
    }
    else
    {
        res = TOOLS_FALSE;
    }

exit:
    END_FUNCTION();
    return res;
}