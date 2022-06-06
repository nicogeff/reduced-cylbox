
#ifndef NAL_RTOS_H_
#define NAL_RTOS_H_

#include "nal_common.h"

#if defined NAL_RTOS_POSIX
#include "nal_rtos_posix.h"
#else
#error "No OS defined ..."
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * \brief Datetime structure.
 */
typedef struct
{
    unsigned short year;    /**< The year in the gregorian calendar. */
    unsigned char month;    /**< The month index: 1=January, 2=February, ..., 12=December. */
    unsigned char day;      /**< The day of the month from 1 to 31. */
    unsigned char wday;     /**< The day of week 0 to 6. */
    unsigned short yday;    /**< The day of year 0 to 365. */
    unsigned char hour;     /**< The hour: from 0 to 23. */
    unsigned char min;      /**< The minute: from 0 to 59. */
    unsigned char sec;      /**< The second: from 0 to 59. */
    unsigned short msec;    /**< The millisecond: from 0 to 999. */
    int isdst;              /**< DST [-1/0/1]. */
} T_nal_datetime;

#ifdef __cplusplus
}
#endif

/*----------------------------------------------------------------------------*\
 *                                Task API                                    *
\*----------------------------------------------------------------------------*/

typedef void (*nal_task_fctptr) (void*);

extern int nal_taskCreate
(
 T_nal_task *th,
 const char *task_name,
 void (*task)(void *),
 void *data,
 int task_priority,
 void *stack_pointer,
 int stack_size
 );

extern int nal_taskDelete
(
 T_nal_task *th
 );

extern void nal_taskExit
(
 void
 );

extern uint32_t nal_taskGetPid
(
 void
 );

/**
 * Use this to retrieve parameter passed to task.
 */
#ifndef nal_taskParam
#define nal_taskParam(p) do {} while(0)
#endif

/*----------------------------------------------------------------------------*\
 *                                Time API                                    *
\*----------------------------------------------------------------------------*/

extern int nal_getClock
(
 T_nal_time *time
 );

extern int nal_sleep
(
 T_nal_time *dly
 );

/** 
 * Absolute time is not always available on embedded systems.
 */
#ifdef NAL_HAVE_DATETIME

extern int nal_getUtcTime
(
 T_nal_datetime *datetime
 );

extern int nal_convertTimeToString
(
    const T_nal_datetime *datetime, 
    char **strdatetime, 
    size_t *strdatetimeSize
);

extern int nal_setUtcTime
(
 const T_nal_datetime *datetime
 );

#endif /* NAL_HAVE_DATETIME */

extern int nal_getPosixTime
(
 T_nal_time *time
 );

extern time_t nal_getBasicTime
(
 time_t *time
 );

/*----------------------------------------------------------------------------*\
 *                             Mutexes API                             *
\*----------------------------------------------------------------------------*/

extern int nal_mutexInit
(
 T_nal_mutex *mutex,
 const char *name
 );

extern int nal_mutexDelete
(
 T_nal_mutex *mutex
 );

extern int nal_mutexLock
(
 T_nal_mutex *mutex
 );

extern int nal_mutexUnlock
(
 T_nal_mutex *mutex
 );

/*----------------------------------------------------------------------------*\
 *                            Semaphores API                                  *
\*----------------------------------------------------------------------------*/

extern int nal_semInit
(
 T_nal_sem *sem,
 int max_count,
 int value,
 const char *name
 );

extern int nal_semDelete
(
 T_nal_sem *sem
 );

extern int nal_semPost
(
 T_nal_sem *sem
 );

extern int nal_semWait
(
 T_nal_sem *sem,
 const T_nal_time *dly
 );

extern int nal_semClear
(
 T_nal_sem *sem
 );

/*----------------------------------------------------------------------------*\
 *                            Queue API                                       *
\*----------------------------------------------------------------------------*/

extern int nal_queueCreate
(
 T_nal_queue *queue,
 char *name,
 unsigned long max_msg_length,
 unsigned long max_msgs,
 void *work_buffer,
 unsigned long work_buffer_size
 );

extern int nal_queueDelete
(
 T_nal_queue *queue
 );

extern int nal_queueReceive
(
 T_nal_queue *queue,
 void *buffer,
 const T_nal_time *dly
 );

extern int nal_queueSend
(
 T_nal_queue *queue,
 void *buffer,
 const T_nal_time *dly
 );

#ifdef __cplusplus
}
#endif

#endif /* NAL_RTOS_H_ */
