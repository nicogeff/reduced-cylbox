
#ifndef NAL_RTOS_POSIX_H_
#define NAL_RTOS_POSIX_H_

#include "nal_common.h"

/* For queues & PTHREAD_MUTEX_RECURSIVE using in Linux */
#if !defined(_XOPEN_SOURCE)
#define _XOPEN_SOURCE 600
#endif

#include <pthread.h>
#include <semaphore.h>
#include <mqueue.h>

/* Definitions for AL_NO_WAIT and AL_WAIT_FOREVER */
extern const T_nal_time nalNoWait;
extern const T_nal_time nalWaitForever;

#define NAL_NO_WAIT      nalNoWait
#define NAL_WAIT_FOREVER nalWaitForever

#define NAL_HAVE_DATETIME

#define NAL_INVALID_HANDLE NULL

#if __WORDSIZE == 64
#define NAL_INVALID_MUTEX   ((T_nal_mutex){{ NULL, 0, 0, 0, 0, 0, { 0, 0 } }, 0})
#else
#define NAL_INVALID_MUTEX   ((T_nal_mutex){{ NULL, 0, 0, 0, 0, { 0 } }, 0})
#endif
#define NAL_IS_INVALID_MUTEX(mutex) (mutex.mutex.__m_reserved == NAL_INVALID_HANDLE)

#define NAL_INVALID_SEM ((T_nal_sem){NULL, NULL, NULL})
#define NAL_IS_INVALID_SEM(sem) (sem->__sem_value == NAL_INVALID_HANDLE)

#define NAL_INVALID_TASK ((T_nal_task)NAL_INVALID_HANDLE)
#define NAL_IS_INVALID_TASK(task) (task == NAL_INVALID_TASK)

#define NAL_INVALID_QUEUE ((T_nal_queue){NULL, NULL, 0})
#define NAL_IS_INVALID_QUEUE(queue) (queue->queue_handler == NAL_INVALID_HANDLE)

/*----------------------------------------------------------------------------*\
 *                              Thread API                                    *
\*----------------------------------------------------------------------------*/
typedef pthread_t T_nal_task;

/*----------------------------------------------------------------------------*\
 *                             Mutexes API                                    *
\*----------------------------------------------------------------------------*/
#define NAL_MUTEX_INITIALIZED_FLAG 0xA18C

typedef struct nal_mutex
{
    pthread_mutex_t mux;
    int init_flag;
} T_nal_mutex;

/*----------------------------------------------------------------------------*\
 *                            Semaphores API                                  *
\*----------------------------------------------------------------------------*/
typedef sem_t T_nal_sem;

/*----------------------------------------------------------------------------*\
 *                            Queues API                                      *
\*----------------------------------------------------------------------------*/
typedef struct nal_queue
{
    mqd_t queue_handler;
    char *queue_name;
    unsigned long msg_size;
} T_nal_queue;

#endif /* NAL_RTOS_POSIX_H_ */
