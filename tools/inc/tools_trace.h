#ifndef TOOLS_TRACE_H
#define TOOLS_TRACE_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------ Include files -------------------------------*/

#include <stdio.h>    
#include <stdint.h>
#include <string.h>
    
#include "nal_libc.h"

#include "tools_type.h"

/*------------------------------ Constants -----------------------------------*/
    
#define TOOLS_LOG_MSG_ERROR      "Logging invalid message! \n"
    
/*------------------------------- Types --------------------------------------*/
    
/**
 * \brief This enumeration define the level of the logging message
 */
typedef enum tools_trace_level_enum
{
    TRACE = 0x00,
    WARN  = 0x01,
    ERROR = 0x02
} T_tools_trace_level;

/**
 * \brief This enumeration define the output of the logging message
 */
typedef enum tools_trace_appender_enum
{
    NO_TRACE = 0x00, /* Logging message is disable */
    PRINT_ALL_TRACE = 0x01, /* Logging all traces (displaying on console) */   
    PRINT_ONLY_ERR_TRACE = 0x02, /* Logging only error traces (displaying on console) */
    PRINT_ONLY_WARNING_TRACE = 0x03, /* Logging only warning traces (displaying on console) */
    PRINT_TRACE_BUT_NO_ENTERING_AND_LEAVING = 0x04, /* Logging all traces except ENTERING and LEAVING traces (displaying on console) */
    FILE_ALL_TRACE = 0x05      /* Logging message is added on file */
} T_tools_trace_appender;

/*------------------------------ Functions -----------------------------------*/

/**
 * This function allow to change the trace appender
 * @param appender The new appender value
 * @return \n
 * T_tools_trace_appender The trace appender
 */
T_tools_trace_appender tools_get_trace_appender 
(
    void
);

/**
 * This function allow to change the trace appender
 * @param appender The new appender value
 */
void tools_set_trace_appender 
(
    T_tools_trace_appender appender
);

/**
 * This function allow to change the trace level
 * @param level The new level value
 * @return \n
 * T_tools_trace_level The trace level
 */
T_tools_trace_level tools_get_trace_level 
(
    void
);

/**
 * This function allow to change the trace level
 * @param level The new level value
 */
void tools_set_trace_level 
(
    T_tools_trace_level level
);
    
/**
 * This function log simple debug information
 * @detail By default the level of the log message is "DEBUG" and the message is 
 * configured to be printing on the console "PRINT_LOG" 
 * @param level Level of the logging message
 * @param message Message to log
 * @param ...Variable argument
 */
void tools_trace 
(
    T_tools_trace_level level, char * message, ...
);

/*------------------------------ Macros --------------------------------------*/
    
/* Used to trace when entering a function */
#define START_FUNCTION()                                                                                \
do {/* So that the MACRO can't be expanded anywhere */                                                  \
    if ((tools_get_trace_appender() != NO_TRACE) && (tools_get_trace_appender() == PRINT_ALL_TRACE))  \
        tools_trace(TRACE,"Entering function: %s in file:%s ",                                         \
            __FUNCTION__, __FILE__);                                                                    \
}                                                                                                       \
while (0);

/* Used to trace when leaving a function */
#define END_FUNCTION()                                                                                 \
do {/* So that the MACRO can't be expanded anywhere */                                                  \
    if ((tools_get_trace_appender() != NO_TRACE) && (tools_get_trace_appender() == PRINT_ALL_TRACE))  \
        tools_trace(TRACE,"Leaving function: %s in file:%s ",                                          \
            __FUNCTION__, __FILE__);                                                                    \
}                                                                                                       \
while (0);

/* Used to add a trace */
#define TRACE_GEN(TRACE_LEVEL, TRACE_STR, FUNCTION, LINE)                       \
do {                                                                            \
    if (tools_get_trace_appender() != NO_TRACE)                                 \
    {                                                                           \
        char * str = NULL;                                                      \
        size_t total_str_len = 0u;                                              \
        const char * s = ": in function: %s at line: %d";                       \
        if (TRACE_STR != NULL)                                                  \
        {                                                                       \
            total_str_len = strlen(TRACE_STR) + strlen(s);                      \
            str = malloc (total_str_len * sizeof(char) +1);                     \
            memset (str, 0, total_str_len * sizeof(char) +1);                   \
            nal_strncpy_s(str, total_str_len, TRACE_STR, strlen(TRACE_STR));    \
            strncat (str, s, strlen(s));                                        \
                                                                                \
            tools_trace(TRACE_LEVEL, str, FUNCTION, LINE);                      \
        }                                                                       \
        else                                                                    \
            tools_trace (TRACE_LEVEL, TRACE_STR, FUNCTION, LINE);               \
                                                                                \
        if (str != NULL) { free(str); str = NULL; }                             \
    }                                                                           \
}                                                                               \
while (0);


    /**
     * @brief This macro must be use to check input/output parameters using in 
     * functions. If failing, trace, and 3rd parameter is filed with 4th value
     */
#define VERIF_ARGS(argument, test, returnCode, errorCode)              \
if ((returnCode) == (errorCode))                                                           \
{}                                                                                                          \
else {                                                                                                     \
    if (!(test))                                                                                           \
    {                                                                                                        \
        tools_trace (ERROR,"VERIF_ARGS, aborting due to VERIF_ARGS         \
            failed in Function: %s File:%s at Line:%d for                                      \
            parameter: %s\n",__FUNCTION__,__FILE__,__LINE__,(argument));    \
        (returnCode) = (errorCode);                                                                   \
    }                                                                                                                 \
}

    /************************  Functions  *******************************/
    void log_add(const uint32_t type, const char *string, const uint32_t option, const uint32_t facility, const uint32_t priority);


#ifdef __cplusplus
}
#endif

#endif /* TOOLS_TRACE_H */
