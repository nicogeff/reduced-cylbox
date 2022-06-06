#ifndef TOOLS_CONSOLE_H
#define TOOLS_CONSOLE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * This function allow to print unlimited parameter
 * @param fmt
 * @param ...
 * @return \n
 * TOOLS_SUCCESS on success \n
 * TOOLS_ERROR on error \n
 */
T_tools_status tools_console_out
(
    char * fmt, ... 
);

#ifdef __cplusplus
}
#endif

#endif /* TOOLS_CONSOLE_H */

