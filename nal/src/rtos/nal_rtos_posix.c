


#include "nal_rtos_posix.h"
#include "nal_rtos.h"
#include "nal_mem.h"

#include <time.h>
#include <sys/time.h>
#include <sys/times.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

const T_nal_time nalNoWait = {0xFFFFFFFE, 0};
const T_nal_time nalWaitForever = {0xFFFFFFFF, 0};

#define   MSG_DEFAULT_PRIO  0
#define   QUEUE_NAME_SIZE   128

int map_value(int x, int in_min, int in_max, int out_min, int out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/*----------------------------------------------------------------------------*\
 *                              Thread API                                    *
\*----------------------------------------------------------------------------*/

#ifdef NO_NAL_RTOS_POSIX_THREAD_API
#warning "Thread API disabled..."
#else

/**
 * \brief Create and execute immediately a new task (thread)
 * @param[out] th Handle to the task control block (TCB) 
 * @param[in] task_name Not implemented
 * @param[in] task A function pointer to the task routine
 * @param[in] data A pointer passed as a parameter to the task routine function
 * @param[in] task_priority Task priority
 * @param[in] stack_pointer Not implemented
 * @param[in] stack_size Task stack size
 * @return \n
 * NAL_SUCCESS: Task created and launched
 * NAL_EINVAL: invalid parameter (stack_size PTHREAD_STACK_MIN, priority not between 0-255 ...)
 * NAL_EACCES: TCB already initialized.
 * NAL_EINTERNAL: Other internal error.
 */
int nal_taskCreate
(
 T_nal_task *th,
 const char *task_name,
 void (*task)(void *),
 void *data,
 int task_priority,
 void *stack_pointer,
 int stack_size
 )
{
    int result = NAL_SUCCESS;
    pthread_attr_t thread_attr = {0};
    struct sched_param param = {0};
    int policy = 0;

    /* Sanity checks */
    if ((th == NULL) || (task == NULL) || (task_name == NULL) || (task_priority < 0) || (task_priority > 255))
        return NAL_EINVAL;

    /* Initializing other attributes */
    if (pthread_attr_init(&thread_attr))
        return NAL_EINTERNAL;

    /* Check stack size parameter */
    if (stack_size != 0)
    {
        if (stack_size < PTHREAD_STACK_MIN)
            return NAL_EINVAL;

        /* stack size setting */
        switch (pthread_attr_setstacksize(&thread_attr, stack_size))
        {
            /* Thread stack size created */
        case 0:
            result = NAL_SUCCESS;
            break;

            /* Invalid attribute */
        case EINVAL:
            return NAL_EINVAL;

            /* Other error */
        default:
            return NAL_EINTERNAL;
        }
    }

    /*Set the thread in detached state to avoid memory leaks after thread exit (FR31398)*/
    if (pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED))
        return NAL_EINTERNAL;

    /* priority setting */

    /*inheriting caller thread policy*/
    if (pthread_getschedparam(pthread_self(), &policy, &param))
        return NAL_EINTERNAL;

    if (pthread_attr_setinheritsched(&thread_attr, PTHREAD_EXPLICIT_SCHED))
        return NAL_EINTERNAL;

    if (pthread_attr_setschedpolicy(&thread_attr, policy))
        return NAL_EINTERNAL;

    /*remap priority if need*/
    param.sched_priority =
            map_value(task_priority, 0, 255, sched_get_priority_min(policy), sched_get_priority_max(policy));

    switch (pthread_attr_setschedparam(&thread_attr, &param))
    {
        /* Thread stack size created */
    case 0:
        result = NAL_SUCCESS;
        break;

        /* Invalid attribute */
    case EINVAL:
        return NAL_EINVAL;

        /* Other error */
    default:
        return NAL_EINTERNAL;
    }

    /* Create the thread */
    switch (pthread_create(th, &thread_attr, (void *(*)(void *))task, data))
    {
        /* Thread created */
    case 0:
        result = NAL_SUCCESS;
        break;

        /* Not enough memory to create the thread */
    case EAGAIN:
        result = NAL_ENOMEM;
        break;

        /* Invalid attribute */
    case EINVAL:
        result = NAL_EINVAL;
        break;

        /* Other error */
    default:
        result = NAL_EINTERNAL;
        break;
    }

    return (result);
}

/**
 * \brief Terminate a task, and free internal data
 * @param[in] th Task handle
 * @return \n
 * NAL_SUCCESS: call successful
 * NAL_EINVAL: Invalid (NULL) handle
 * NAL_ENORES: The handle does not correspond to any task in activity
 */
int nal_taskDelete(T_nal_task *th)
{
    int result = NAL_SUCCESS;
    int cancelRes = 0;

    if (th == NULL)
    {
        result = NAL_EINVAL;
    }
    else if (*th == 0)
    {
        result = NAL_ENORES;
    }
    else
    {
        /* Delete the task */
        cancelRes = pthread_cancel(*th);

        /* Return value conversion */
        switch (cancelRes)
        {
            /* Thread deleted */
        case 0:
            result = NAL_SUCCESS;
            *th = NAL_INVALID_TASK;
            break;

            /* Thread does not exist */
        case ESRCH:
            result = NAL_ENORES;
            break;

            /* Other error */
        default:
            result = NAL_EINTERNAL;
            break;
        }
    }
    return (result);
}

/**
 * \brief Terminate a task, and free internal data
 */
void nal_taskExit(void)
{
    pthread_exit(NULL);
}

/**
 * \brief Return a unique unsigned 32 bits integer associated with the current task
 * @return Process ID : A unique unsigned 32 bit int associated with each task
 */
uint32_t nal_taskGetPid(void)
{
    return getpid();
}

#endif /* NO_NAL_RTOS_POSIX_THREAD_API */

/*----------------------------------------------------------------------------*\
 *                            Semaphores management                           *
\*----------------------------------------------------------------------------*/

#ifdef NO_NAL_RTOS_POSIX_SEM_API
#warning "Semaphore API disabled..."
#else

/**
 * \brief Create and initialize a counting semaphore
 * @param[in] sem Handle to the semaphore control block to initialize
 * @param[in] max_count Not implemented
 * @param[in] value Default value at initialization
 * @param[in] name Not implemented 
 * @return \n
 * NAL_SUCCESS: Semaphore created and initialized.
 * NAL_EINVAL: Invalid parameter.
 * NAL_EACCES: Semaphore already initialized.
 */
int nal_semInit(T_nal_sem *sem, int max_count, int value, const char* name)
{
    int result = NAL_SUCCESS;
    int initRes = 0;

    /* Sanity checks */
    if ((sem == NULL) || (name == NULL) || (value > max_count))
    {
        result = NAL_EINVAL;
    }
    else
    {
        /* Initialize the semaphore */
        initRes = sem_init(sem, 0, value);

        /* Retrieve the error if necessary */
        if (initRes == -1)
            initRes = errno;

        /* Return value conversion */
        switch (initRes)
        {
            /* Semaphore initialized */
        case 0:
            result = NAL_SUCCESS;
            break;

            /* value exceed SEM_VALUE_MAX */
        case EINVAL:
            result = NAL_EIMPL;
            break;

            /* Semaphore already initialized */
        case EBUSY:
            result = NAL_EACCES;
            break;

            /* Other error */
        default:
            result = NAL_EINTERNAL;
            break;
        }
    }
    return result;
}

/**
 * \brief Deletes a semaphore previously created
 * @param[in] sem Handle on the semaphore to delete
 * @return \n
 * NAL_SUCCESS: Semaphore deleted.
 * NAL_EINVAL: Invalid (NULL) pointers on parameter sem.
 * NAL_ENORES: The semaphore handle does not correspond to any valid semaphore.
 */
int nal_semDelete(T_nal_sem *sem)
{
    int result = NAL_SUCCESS;
    int destroyRes = 0;

    if (sem == NULL)
    {
        result = NAL_EINVAL;
    }
    else
    {
        /* destroy the semaphore */
        destroyRes = sem_destroy(sem);

        /* Retrieve the error if necessary */
        if (destroyRes == -1)
            destroyRes = errno;

        /* Return value conversion */
        switch (destroyRes)
        {
            /* Semaphore deleted */
        case 0:
            result = NAL_SUCCESS;
            break;

            /* Not a valid semaphore */
        case EINVAL:
            result = NAL_ENORES;
            break;

            /* Other error */
        default:
            result = NAL_EINTERNAL;
            break;
        }
    }
    return result;
}

/**
 * \brief Increment the internal counter of the semaphore
 * @param[in] sem Handle on the semaphore to increment
 * @return \n
 * NAL_SUCCESS : Semaphore incremented.
 * NAL_EINVAL : Invalid (NULL) pointers on parameter sem.
 * NAL_ENORES : The semaphore handle does not correspond to any valid semaphore.
 */
int nal_semPost(T_nal_sem *sem)
{
    int result = NAL_SUCCESS;
    int postRes = 0;

    if (sem == NULL)
    {
        result = NAL_EINVAL;
    }
    else
    {
        /* Post to the semaphore */
        postRes = sem_post(sem);

        /* Retrieve the error if necessary */
        if (postRes == -1)
            postRes = errno;

        /* Return value conversion */
        switch (postRes)
        {
            /* Semaphore deleted */
        case 0:
            result = NAL_SUCCESS;
            break;

            /* Not a valid semaphore */
        case EINVAL:
            result = NAL_ENORES;
            break;

            /* Other error */
        default:
            result = NAL_EINTERNAL;
            break;
        }
    }
    return result;
}

/**
 * \brief Wait until the semaphore counter is higher than 0 and decrement it
 * @param[in] sem Handle on the semaphore to wait on
 * @param[in] dly Wait option, NAL_WAIT_FOREVER (blocking call) 
 * NAL_NO_WAIT (non blocking call). User defined structure (user defined timeout)
 * @return \n
 * NAL_SUCCESS: Semaphore decremented.
 * NAL_EINVAL: Invalid parameter.
 * NAL_ETIMEDOUT: Wait timeout.
 * NAL_ENORES: The semaphore handle does not correspond to any valid semaphore.
 * NAL_EINTERNAL: Other internal error
 */
int nal_semWait(T_nal_sem *sem, const T_nal_time * dly)
{
    struct timespec timeout = {0};
    T_nal_time currenttime = {0};
    int result = NAL_SUCCESS;
    int waitRes = 0;

    /* Sanity check */
    if ((sem == NULL) || (dly == NULL) || ((dly->sec <= 0) && (dly->usec <= 0)))
    {
        result = NAL_EINVAL;
    }
    else
    {
        /* Check timeout validity */
        if ((dly->sec == 0) && (dly->usec == 0))
            result = NAL_EINVAL; /* 0 sec timeouts are forbidden */
        else
        {
            /* Compute timeout with specific cases (no wait, timed wait, and blocking call) */
            if (dly->sec == NAL_NO_WAIT.sec)
            {
                /* No wait */
                waitRes = sem_trywait(sem);
            }
            else if (dly->sec == NAL_WAIT_FOREVER.sec)
            {
                /* Blocking call */
                waitRes = sem_wait(sem);
            }
            else
            {
                /* Timed wait
                 * For Posix: Use an absolute time (date)
                 * Compute the expiration date */
                nal_getPosixTime(&currenttime);
                timeout.tv_sec = currenttime.sec + dly->sec + (currenttime.usec + dly->usec) / 1000000;
                timeout.tv_nsec = ((currenttime.usec + dly->usec) % 1000000) * 1000;

                do
                {
                    /* Restart to wait if interrupted  by handler */
                    waitRes = sem_timedwait(sem, &timeout);
                }
                while (waitRes == -1 && errno == EINTR);
            }

            /* Retrieve the error if necessary */
#ifndef NON_COMPLIANT_TIMEDWAIT
            /* NON_COMPLIANT_TIMEDWAIT option must be set when using a non POSIX compliant OS
             * where sem_timedwait returns directly an error code, instead of -1 if error
             */
            if (waitRes == -1)
                waitRes = errno;
#endif
            /* Return value conversion */
            switch (waitRes)
            {
                /* Semaphore decremented */
            case 0:
                result = NAL_SUCCESS;
                break;

                /* Not a valid semaphore */
            case EINVAL:
                result = NAL_ENORES;
                break;

                /* Timeout while waiting (timed wait) */
            case ETIMEDOUT:
                /* Non blocking call with semaphore counter = 0 */
            case EAGAIN:
                result = NAL_ETIMEDOUT;
                break;

                /* Other error */
            default:
                result = NAL_EINTERNAL;
                break;
            }
        }
    }
    return result;
}

/**
 * \brief Clear the internal counter of the semaphore
 * @param[in] sem Handle on the semaphore to clear
 * @return \n
 * NAL_EINVAL: Invalid (NULL) pointers on parameter sem.
 * NAL_EIMPL: Function not implemented
 */
int nal_semClear(T_nal_sem *sem)
{
    int result = NAL_SUCCESS;
    int tryRes = 0;

    if (sem == NULL)
    {
        result = NAL_EINVAL;
    }
    else
    {
        /* Non blocking wait until counter reach 0 (EAGAIN) */
        do
        {
            tryRes = sem_trywait(sem);
        }
        while (tryRes == 0);
        /* Retrieve the error */
        tryRes = errno;

        /* Return value conversion */
        switch (tryRes)
        {
            /* Semaphore decremented to 0 */
        case EAGAIN:
            result = NAL_SUCCESS;
            break;

            /* Not a valid semaphore */
        case EINVAL:
            result = NAL_ENORES;
            break;

            /* Other error */
        default:
            result = NAL_EINTERNAL;
            break;
        }
    }
    return result;
}

#endif /* NO_NAL_RTOS_POSIX_SEM_API */

/*----------------------------------------------------------------------------*\
 *                            Queues management                           *
\*----------------------------------------------------------------------------*/

#ifdef NO_NAL_RTOS_POSIX_QUEUE_API
#warning "Queue API disabled..."
#else

/**
 * \brief Create and initialize a message queue
 * @param[in] queue Queue control block
 * @param[in] name Friendly name for the queue
 * @param[in] max_msg_length Size of the messages handled by the queue in bytes
 * @param[in] max_msgs Maximum number of messages stored by the queue
 * @param[in] work_buffer Not implemented
 * @param[in] work_buffer_size Not implemented
 * @return \n
 * NAL_SUCCESS: Queue created and initialized
 * NAL_EINVAL: Invalid parameter. 
 * NAL_EINTERNAL: Other internal error.
 */
int nal_queueCreate(T_nal_queue *queue, char *name, unsigned long max_msg_length,
                    unsigned long max_msgs, void *work_buffer, unsigned long work_buffer_size)
{
    struct mq_attr QueueAttributes = {0};
    int result = NAL_SUCCESS;
    int openRes = 0;
    int pid = 0;

    if ((queue == NULL) || (name == NULL) || (work_buffer == NULL))
    {
        result = NAL_EINVAL;
    }
    else
    {
        if (work_buffer_size < (max_msg_length * max_msgs))
        {
            result = NAL_EINVAL;
        }
        else
        {
            /* Compute a new queue name with PID
             * To avoid mixing queues between different apps
             * On POSIX systems, queues names must be: /something
             */
            queue->queue_name = (char*) malloc(QUEUE_NAME_SIZE + 1);
            if (queue->queue_name != NULL)
            {
                memset(queue->queue_name, 0, QUEUE_NAME_SIZE + 1);
                pid = nal_taskGetPid();
                snprintf(queue->queue_name, QUEUE_NAME_SIZE, "/%d_%s", pid, name);
            }
            else
            {
                result = NAL_ENOMEM;
            }

            /* If the name copy / creation succeedeed, continue */
            if (result == NAL_SUCCESS)
            {
                /* Save the message size inside the queue structure (for emssion / reception) */
                queue->msg_size = max_msg_length;

                /* Set the attributes: Messages size and maximum number of messages */
                QueueAttributes.mq_flags = 0;
                QueueAttributes.mq_maxmsg = max_msgs;
                QueueAttributes.mq_msgsize = queue->msg_size;

                /* Create the queue, bidirectionnal, RW, create by default,
                 * R/W permission for user only
                 * send back an error if a queue with the same name already exist */
                queue->queue_handler = mq_open(queue->queue_name, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, &QueueAttributes);

                /* Retrieve error if necessary */
                if (queue->queue_handler == (mqd_t) - 1)
                    openRes = errno;
                else
                    openRes = 0;

                /* Return value conversion */
                switch (openRes)
                {
                    /* Queue created */
                case 0:
                    result = NAL_SUCCESS;
                    break;

                    /* Queue already exist */
                case EACCES:
                case EEXIST:
                    result = NAL_EACCES;
                    break;

                    /* message length or max number of messages above system limitations */
                case EINVAL:
                    result = NAL_EINVAL;
                    break;

                    /* Too many queues or no space left. */
                case ENOMEM:
                case ENFILE:
                case ENOSPC:
                    result = NAL_ENOMEM;
                    break;

                    /* Other error */
                default:
                    result = NAL_EINTERNAL;
                    break;
                }
            }
        }
    }
    return result;
}

/**
 * \brief Create and initialize a message queue
 * @param queue Handle on the queue to delete
 * @return \n
 * NAL_SUCCESS : Queue deleted.
 * NAL_EINVAL : Invalid (NULL) pointers on parameter queue.
 * NAL_ENORES : The queue handle does not correspond to any valid queue.
 */
int nal_queueDelete(T_nal_queue *queue)
{
    int result = NAL_SUCCESS;
    int closeRes = 0;

    if ((queue == NULL) || ((queue->queue_handler == 0) && (queue->queue_name == NULL)))
    {
        result = NAL_EINVAL;
    }
    else
    {
        /* Close the queue */
        closeRes = mq_close(queue->queue_handler);

        if (closeRes == 0)
        {
            /* Destroy the queue */
            closeRes = mq_unlink(queue->queue_name);
            free(queue->queue_name);
            queue->queue_name = NULL; /*NGE_LEAK*/
        }

        /* Retrieve error if necessary */
        if (closeRes == -1)
            closeRes = errno;

        /* Return value conversion */
        switch (closeRes)
        {
            /* Queue deleted */
        case 0:
            result = NAL_SUCCESS;
            break;

            /* Invalid handler */
        case EBADF:
            result = NAL_ENORES;
            break;

            /* Permission denied */
        case EACCES:
            result = NAL_EACCES;
            break;

            /* Other error */
        default:
            result = NAL_EINTERNAL;
            break;
        }

    }
    return result;
}

/**
 * \brief Wait until the queue has an available message
 * @param queue Handle on the queue to wait on
 * @param buffer Pointer on a memory space where the message will be copied
 * @param dly Wait option, 
 * NAL_WAIT_FOREVER (blocking call) 
 * NAL_NO_WAIT (non blocking call)
 * User defined structure (user defined timeout)
 * @return \n
 * NAL_SUCCESS : Message received and copied into buffer.
 * NAL_ETIMEDOUT : Wait timeout:
 * NAL_EINVAL Invalid parameters
 * NAL_ENORES : The queue handle does not correspond to any valid queue.
 */
int nal_queueReceive(T_nal_queue *queue, void *buffer, const T_nal_time * dly)
{
    struct timespec timeLimit = {0};
    T_nal_time currentTime = {0};
    int result = NAL_SUCCESS;
    int recvRes = 0;
    unsigned int priority = 0u;

    if ((queue == NULL) || (buffer == NULL) || (dly == NULL) || ((dly->sec <= 0) && (dly->usec <= 0)))
    {
        result = NAL_EINVAL;
    }
    else
    {
        /* Check timeout validity */
        if ((dly->sec == 0) && (dly->usec == 0))
            result = NAL_EINVAL; /* 0 sec timeouts are forbidden */
        else
        {
            /* Compute timeout with specific cases (no wait, timed wait, and blocking call) */
            if (dly->sec == NAL_NO_WAIT.sec)
            {
                /* No wait
                 * Use a time structure with a value already elapsed */
                timeLimit.tv_sec = 0;
                timeLimit.tv_nsec = 0;
                recvRes = (int) mq_timedreceive(queue->queue_handler,
                                                (char *) buffer,
                                                queue->msg_size,
                                                &priority,
                                                &timeLimit);
            }
            else if (dly->sec == NAL_WAIT_FOREVER.sec)
            {
                /* Blocking call */
                recvRes = (int) mq_receive(queue->queue_handler,
                                           (char *) buffer,
                                           queue->msg_size,
                                           &priority);
            }
            else
            {
                /* Timed wait
                 * For Posix: Use an absolute time (date)
                 * Compute the expiration date */
                nal_getPosixTime(&currentTime);
                timeLimit.tv_sec = currentTime.sec + dly->sec + (currentTime.usec + dly->usec) / 1000000;
                timeLimit.tv_nsec = ((currentTime.usec + dly->usec) % 1000000) * 1000;
                recvRes = (int) mq_timedreceive(queue->queue_handler,
                                                (char *) buffer,
                                                queue->msg_size,
                                                &priority,
                                                &timeLimit);
            }

            /* RecvResult is the number of bytes received, or -1 */
            if (recvRes < 0)
                recvRes = errno;
            else
                recvRes = 0;

            /* Return value conversion */
            switch (recvRes)
            {
                /* Queue deleted */
            case 0:
                result = NAL_SUCCESS;
                break;

                /* Invalid handler */
            case EBADF:
                result = NAL_ENORES;
                break;

                /* Permission denied */
            case ETIMEDOUT:
                result = NAL_ETIMEDOUT;
                break;

                /* Other error */
            default:
                result = NAL_EINTERNAL;
                break;
            }
        }
    }
    return result;
}

/**
 * \brief Copy the message from the parameter buffer, and enqueue it
 * @param queue Handle on the queue to send into
 * @param buffer Pointer on the message to enqueue
 * @param dly Wait option, 
 * NAL_WAIT_FOREVER (blocking call) 
 * AL_NO_WAIT (non blocking call)
 * User defined structure (user defined timeout)
 * @return \n
 * NAL_SUCCESS: Message enqueued successfully.
 * NAL_EINVAL: Invalid parameters
 * NAL_ENORES: The queue handle does not correspond to any valid queue.
 */
int nal_queueSend(T_nal_queue *queue, void *buffer, const T_nal_time * dly)
{
    struct timespec timeLimit = {0};
    T_nal_time currentTime = {0};
    int result = NAL_SUCCESS;
    int sendRes = 0;

    if ((queue == NULL) || (buffer == NULL) || (dly == NULL) || ((dly->sec <= 0) && (dly->usec <= 0)))
    {
        result = NAL_EINVAL;
    }
    else
    {
        /* Check timeout validity */
        if ((dly->sec == 0) && (dly->usec == 0))
            result = NAL_EINVAL; /* 0 sec timeouts are forbidden */
        else
        {
            /* Compute timeout with specific cases (no wait, timed wait, and blocking call) */
            if (dly->sec == NAL_NO_WAIT.sec)
            {
                /* No wait
                 * Use a time structure with a value already elapsed */
                timeLimit.tv_sec = 0;
                timeLimit.tv_nsec = 0;
                sendRes = (int) mq_timedsend(queue->queue_handler,
                                             (const char *) buffer,
                                             queue->msg_size,
                                             MSG_DEFAULT_PRIO,
                                             &timeLimit);
            }
            else if (dly->sec == NAL_WAIT_FOREVER.sec)
            {
                /* Blocking call */
                sendRes = (int) mq_send(queue->queue_handler,
                                        (const char *) buffer,
                                        queue->msg_size,
                                        MSG_DEFAULT_PRIO);
            }
            else
            {
                /* Timed wait
                 * For Posix: Use an absolute time (date)
                 * Compute the expiration date */
                nal_getPosixTime(&currentTime);
                timeLimit.tv_sec = currentTime.sec + dly->sec + (currentTime.usec + dly->usec) / 1000000;
                timeLimit.tv_nsec = ((currentTime.usec + dly->usec) % 1000000) * 1000;
                sendRes = (int) mq_timedsend(queue->queue_handler,
                                             (const char *) buffer,
                                             queue->msg_size,
                                             MSG_DEFAULT_PRIO,
                                             &timeLimit);
            }

            if (sendRes == -1)
                sendRes = errno;

            /* Return value conversion */
            switch (sendRes)
            {
                /* Queue deleted */
            case 0:
                result = NAL_SUCCESS;
                break;

                /* Invalid handler */
            case EBADF:
                result = NAL_ENORES;
                break;

                /* Permission denied */
            case ETIMEDOUT:
                result = NAL_ETIMEDOUT;
                break;

                /* Other error */
            default:
                result = NAL_EINTERNAL;
                break;
            }
        }
    }
    return result;
}

#endif /* NO_NAL_RTOS_POSIX_QUEUE_API */

/*----------------------------------------------------------------------------*\
 *                              Time API                                      *
\*----------------------------------------------------------------------------*/

#ifdef NO_NAL_RTOS_POSIX_TIME_API
#warning "Time API disabled..."
#else

/**
 * \brief Stop a task for specified amount value
 * @param[in] dly sleep duration
 * @return \n
 * NAL_SUCCESS : call successful
 * NAL_EINVAL : invalid (NULL) parameter.
 */
int nal_sleep(T_nal_time * dly)
{
    int result = NAL_SUCCESS;

    if (dly == NULL)
    {
        result = NAL_EINVAL;
    }
    else
    {
        /* Limitation for sleep duration in microseconds (32 bits encoding) */
        if (dly->sec > 4292)
            sleep(dly->sec);
        else
            usleep(dly->sec * 1000000 + dly->usec);

        result = NAL_SUCCESS;
    }

    return ( result);
}

/**
 * \brief Returns the delay since 00:00:00 GMT, January 1, 1970
 * @param[out] time Delay since 00:00:00 GMT, January 1, 1970
 * @return \n
 * NAL_SUCCESS: call successful
 * NAL_EINVAL: invalid (NULL) parameter.
 */
int nal_getPosixTime(T_nal_time * time)
{
    struct timeval tv = {0};
    int result = NAL_SUCCESS;

    if (time == NULL)
    {
        result = NAL_EINVAL;
    }
    else
    {
        /* Get POSIX time fro system */
        gettimeofday(&tv, (struct timezone *) 0);

        /* Conversion to AL format */
        time->sec = tv.tv_sec;
        time->usec = tv.tv_usec;

        result = NAL_SUCCESS;
    }

    return ( result);
}

/** 
 * Absolute time is not always available on embedded systems.
 */
#ifdef NAL_HAVE_DATETIME

int nal_getUtcTime(T_nal_datetime *datetime)
{
    int result = NAL_SUCCESS;

    time_t rawtime;
    struct tm *timeinfo;

    /* Sanity check */
    if (NULL == datetime)
    {
        result = NAL_EINVAL;
        goto exit;
    }

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    datetime->sec = timeinfo->tm_sec;
    datetime->min = timeinfo->tm_min;
    datetime->hour = timeinfo->tm_hour;
    datetime->day = timeinfo->tm_mday;
    datetime->month = timeinfo->tm_mon;
    datetime->year = timeinfo->tm_year;
    datetime->wday = timeinfo->tm_wday;
    datetime->yday = timeinfo->tm_yday;
    datetime->isdst = timeinfo->tm_isdst;

exit:
    return result;
}

int nal_convertTimeToString(const T_nal_datetime *datetime, char **strdatetime, size_t *strdatetimeSize)
{
    int result = NAL_SUCCESS;

    char * lstrtime = NULL;
    struct tm timeinfo;
    
    /* Sanity check */
    if ((NULL == datetime) || (NULL == strdatetime) || (NULL != *strdatetime) || (NULL == strdatetimeSize))
    {
        result = NAL_EINVAL;
        goto exit;
    }

    timeinfo.tm_sec = datetime->sec;
    timeinfo.tm_min = datetime->min;
    timeinfo.tm_hour = datetime->hour;
    timeinfo.tm_mday = datetime->day;
    timeinfo.tm_mon = datetime->month;
    timeinfo.tm_year = datetime->year;
    timeinfo.tm_wday = datetime->wday;
    timeinfo.tm_yday = datetime->yday;
    timeinfo.tm_isdst = datetime->isdst;

    lstrtime = asctime(&timeinfo);

    /* Remove the \n added by asctime() */
    if (lstrtime[strlen(lstrtime) -1] == '\n')
    {
        lstrtime[strlen(lstrtime) -1] = '\0';
    }
    
    (*strdatetime) = NAL_MALLOC(strlen(lstrtime) * sizeof (char) + 1);
    if (NULL == (*strdatetime))
    {
        result = NAL_ENOMEM;
        goto exit;
    }
    memset((*strdatetime), 0, strlen(lstrtime) * sizeof (char) + 1);
    memcpy((*strdatetime), lstrtime, strlen(lstrtime) * sizeof (char));

    (*strdatetimeSize) = strlen(lstrtime);

exit:
    return result;
}
#endif /* NAL_HAVE_DATETIME */

#endif /* NO_NAL_RTOS_POSIX_TIME_API */

/*----------------------------------------------------------------------------*\
 *                             Mutexes API                                    *
\*----------------------------------------------------------------------------*/

#ifdef NO_NAL_RTOS_POSIX_MUTEX_API
#warning "Mutex API disabled..."
#else

/**
 * \brief Create and initialize a mutex
 * @param[out] mutex Handle to the mutex
 * @param[in] name Not implemented
 * @return \n
 * NAL_SUCCESS : mutex created and initialized.
 * NAL_EINVAL : invalid (NULL) pointers on parameters mutex or name.
 * NAL_EINTERNAL : internal error.
 * @todo Some return are used instead of the variable result!
 */
int nal_mutexInit(T_nal_mutex *mutex, const char* name)
{
    int result = NAL_SUCCESS;
    int initRes = 0;
    pthread_mutexattr_t mutexattr = {0};

    /* Initialize the mutex attributes variable (needed only for mutex init) */
    result = pthread_mutexattr_init(&mutexattr);
    if (result != 0)
        return (NAL_EINTERNAL);

    result = pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_RECURSIVE);
    if (result != 0)
    {
        /* release the mutex attributes object - not needed anymore */
        pthread_mutexattr_destroy(&mutexattr);
        return (NAL_EINTERNAL);
    }

    result = NAL_SUCCESS;
    if ((mutex == NULL) || (name == NULL))
    {
        result = NAL_EINVAL;
    }
    else if (mutex->init_flag == NAL_MUTEX_INITIALIZED_FLAG)
    {
        return NAL_EACCES;
    }
    else
    {
        /* Mutex initialization */
        initRes = pthread_mutex_init(&(mutex->mux), &mutexattr);

        /* Return value conversion */
        switch (initRes)
        {
            /* Mutex created */
        case 0:
            mutex->init_flag = NAL_MUTEX_INITIALIZED_FLAG;
            result = NAL_SUCCESS;
            break;

            /* Not enough resources to create the mutex */
        case EAGAIN:
        case ENOMEM:
            result = NAL_ENOMEM;
            break;

            /* Other error */
        default:
            result = NAL_EINTERNAL;
            break;
        }
    }
#ifdef SM_CPU_DCT /* PPCe500v2-gnu-linux platform */
    /* release the mutex attributes object - not needed anymore */
    pthread_mutexattr_destroy(&mutexattr);
#endif
    return result;
}

/**
 * \brief Deletes a mutex previously created
 * @param[in] mutex Handle on the mutex to delete
 * @return \n
 * NAL_SUCCESS: mutex deleted.
 * NAL_EINVAL: invalid (NULL) pointers on parameters mutex.
 * NAL_ENORES: the mutex handle does not correspond to any valid mutex.
 * @todo Some return are used instead of the variable result!
 */
int nal_mutexDelete(T_nal_mutex *mutex)
{
    int result = NAL_SUCCESS;
    int destroyRes = 0;

    if (mutex == NULL)
    {
        result = NAL_EINVAL;
    }
    else if (mutex->init_flag != NAL_MUTEX_INITIALIZED_FLAG)
    {
        return NAL_ENORES;
    }
    else
    {
        /* Mutex destruction */
        destroyRes = pthread_mutex_destroy(&(mutex->mux));

        /* Return value conversion */
        switch (destroyRes)
        {
            /* Mutex deleted */
        case 0:
            result = NAL_SUCCESS;
            break;

            /* Mutex locked */
        case EBUSY:
            result = NAL_EBUSY;
            break;

            /* Not a valid mutex */
        case EINVAL:
            result = NAL_ENORES;
            break;

            /* Other error */
        default:
            result = NAL_EINTERNAL;
            break;
        }
        mutex->init_flag = 0;
    }
    return result;
}

/**
 * \brief Lock the mutex
 * @param[in] mutex Handle on the mutex to lock
 * @return \n
 * NAL_SUCCESS: mutex locked.
 * NAL_EINVAL: invalid (NULL) pointers on parameters mutex.
 * NAL_ENORES: the mutex handle does not correspond to any valid mutex.
 * NAL_EINTERNAL: internal error.
 */
int nal_mutexLock(T_nal_mutex *mutex)
{
    int result = NAL_SUCCESS;
    int lockRes = 0;

    if (mutex == NULL)
    {
        result = NAL_EINVAL;
    }
    else
    {
        /* Lock the mutex */
        lockRes = pthread_mutex_lock(&(mutex->mux));

        /* Return value conversion */
        switch (lockRes)
        {
            /* Mutex locked */
        case 0:
            result = NAL_SUCCESS;
            break;

            /* Not a valid mutex */
        case EINVAL:
            result = NAL_ENORES;
            break;

            /* Other error */
        default:
            result = NAL_EINTERNAL;
            break;
        }
    }
    return result;
}

/**
 * \brief This function unlock a mutex previously locked by the same task
 * @param[in] mutex Handle on the mutex to lock
 * @return \n
 * NAL_SUCCESS: mutex unlocked.
 * NAL_EINVAL: invalid (NULL) pointers on parameters mutex.
 * NAL_ENORES: the mutex handle does not correspond to any valid mutex.
 */
int nal_mutexUnlock(T_nal_mutex *mutex)
{
    int result = NAL_SUCCESS;
    int unlockRes = 0;

    if (mutex == NULL)
    {
        result = NAL_EINVAL;
    }
    else
    {
        /*Unlock the mutex */
        unlockRes = pthread_mutex_unlock(&(mutex->mux));

        /* Return value conversion */
        switch (unlockRes)
        {
            /* Mutex unlocked */
        case 0:
            result = NAL_SUCCESS;
            break;

            /* Not a valid mutex */
        case EINVAL:
            result = NAL_ENORES;
            break;

            /* Could not be acquired because it was already locked */
        case EBUSY:
            result = NAL_EBUSY;
            break;

            /** 
             * could not be acquired because the maximum number of recursive 
             * locks for mutex has been exceeded 
             */
        case EAGAIN:
            result = NAL_EAGAIN;
            break;

            /* The current thread already owns the mutex */
        case EDEADLK:
            result = NAL_EINTERNAL;
            break;

            /* The current thread does not own the mutex */
        case EPERM:
            result = NAL_EACCES;
            break;

            /* Other error */
        default:
            result = NAL_EINTERNAL;
            break;
        }
    }
    return result;
}

#endif /* NO_NAL_RTOS_POSIX_MUTEX_API */
