#define _GNU_SOURCE // use the gnu extension so we have usleep available
#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

// Optional: use these functions to add debug or error prints to your application
#define DEBUG_LOG(msg,...)
//#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)

void* threadfunc(void* thread_param)
{
    struct thread_data* args = (struct thread_data*) thread_param;

    // Sleep wait_to_obtain_ms milliseconds
    int ret = usleep(args->wait_to_obtain_ms * 1000);
    if (ret != 0)
    {
        ERROR_LOG("usleep");
        return thread_param;
    }

    // obtain mutex 
    pthread_mutex_lock(args->mutex);

    // Sleep wait_to_release_ms milliseconds
    ret = usleep(args->wait_to_release_ms * 1000);
    if (ret != 0)
    {
        ERROR_LOG("usleep");
        return thread_param;
    }

    // release mutex
    pthread_mutex_unlock(args->mutex);

    // If we reached here the thread completed with success
    args->thread_complete_success = true;

    return thread_param;
}


bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex,int wait_to_obtain_ms, int wait_to_release_ms)
{
    // Allocate memory for thread data
    struct thread_data* args = malloc(sizeof(struct thread_data));
    if (args == NULL)
    {
        ERROR_LOG("malloc");
        return false;
    }

    // Setup mutex and wait arguments
    args->mutex = mutex;
    args->wait_to_obtain_ms = wait_to_obtain_ms;
    args->wait_to_release_ms = wait_to_release_ms;

    // Create a thread with threadfunc and args
    int ret = pthread_create(thread, NULL, threadfunc, args);
    if (ret != 0)
    {
        errno = ret;
        ERROR_LOG("pthread_create");
        return false;
    }

    // Success
    return true;
}

