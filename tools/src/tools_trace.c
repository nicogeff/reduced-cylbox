
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>

#include "tools_trace.h"
#include "tools_type.h"
#include "tools_console.h"

static T_tools_trace_level G_TRACE_LEVEL = TRACE;
static T_tools_trace_appender G_TRACE_APPENDER = PRINT_TRACE_BUT_NO_ENTERING_AND_LEAVING;

/*------------------------ Private functions ---------------------------------*/

/*------------------------- Public functions ---------------------------------*/

/**
 * This function allow to change the trace appender
 * @param appender The new appender value
 * @return \n
 * T_tools_trace_appender The trace appender
 */
T_tools_trace_appender tools_get_trace_appender
(
 void
 )
{
    return G_TRACE_APPENDER;
}

/**
 * This function allow to change the trace appender
 * @param appender The new appender value
 */
void tools_set_trace_appender(T_tools_trace_appender appender)
{
    G_TRACE_APPENDER = appender;
}

/**
 * This function allow to change the trace level
 * @param level The new level value
 * @return \n
 * T_tools_trace_level The trace level
 */
T_tools_trace_level tools_get_trace_level
(
 void
 )
{
    return G_TRACE_LEVEL;
}

/**
 * This function allow to change the trace level
 * @param level The new level value
 */
void tools_set_trace_level(T_tools_trace_level level)
{
    G_TRACE_LEVEL = level;
}

/**
 * This function log simple debug information
 * @detail By default the level of the log message is "DEBUG" and the message is 
 * configured to be printing on the console "PRINT_LOG" The log message will be 
 * truncated if it length is over 255 char.
 * @param level Level of the logging message
 * @param message Message to log
 * @param ...Optional parameter
 */
void tools_trace(T_tools_trace_level level, char * message, ...)
{
    va_list args;
    char logBuffer[TOOLS_BIG_ARRAY_SIZE] = "\0";
    int32_t rc = 0;

    if ((message == NULL) || (strlen(message) >=
            ((size_t) TOOLS_BIG_ARRAY_SIZE + 1)))
    {
        tools_console_out(TOOLS_LOG_MSG_ERROR);
        goto exit;
    }

    /* Test if message should be logged */
    if (level >= G_TRACE_LEVEL)
    {
        /* Create a string with the log formatting */
        va_start(args, message);
        rc = vsnprintf(logBuffer, sizeof (logBuffer), message, args);
        va_end(args);

        /* Handle truncating (CERT FIO33-C) */
        if ((rc == -1) || (rc >= ((int32_t)sizeof (logBuffer))))
        {
            logBuffer[(sizeof (logBuffer)) - 1u] = '\0';
        }

        switch (level)
        {
        case TRACE:
            tools_console_out(" [DEBUG] %s %s", logBuffer, "\n");
            break;
        case WARN:
            tools_console_out(" [WARN] %s %s", logBuffer, "\n");
            break;
        case ERROR:
            tools_console_out(" [ERROR] %s %s", logBuffer, "\n");
            break;
        default:
            break;
        }
    }

exit:
    return;
}

/** 
 * Stub to send Log, can be a trace temporary  
 * @todo "A terme une entité Log viendra..." 
 */
void log_add(const uint32_t type, const char *string, const uint32_t option, const uint32_t facility, const uint32_t priority)
{
    char lstring[128];
    size_t  writtenSize=0u;
    nal_snprintf_s (lstring, 128,NAL_TRUNCATE, & writtenSize,"logger Log %d: %s, %d %d %d", type,string,option,facility,priority); 

    system (lstring);
}
