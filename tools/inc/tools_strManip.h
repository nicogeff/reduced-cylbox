#ifndef TOOLS_STRMANIP_H
#define TOOLS_STRMANIP_H

#include "tools_type.h"

#ifdef __cplusplus
extern "C"
{
#endif

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
T_tools_status tools_concatStrings
(
 char** out, size_t count, const char* str, ...
 );

/**
 * This function can be use to search if the end of a string character is present
 * @param[in] in String to scan
 * @param[in] character Character to search into the string
 * @return True if the character is found, otherwise false is returned
 */
T_tools_boolean tools_isCharacterPresentAtEnd(const char* in, const char character);

#ifdef __cplusplus
}
#endif    

#endif /* TOOLS_STRMANIP_H */