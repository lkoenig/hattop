#include <stddef.h>
#include "thread.h"

void THREAD_create(thread_t *thread, void *(*thread_function)(void*), void *arg)
{
    if(thread) {
#ifdef _WIN32
        *thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)*thread_function, arg, 0, NULL);
#else
        /* Joinable thread */
        pthread_create(thread, NULL, thread_function, arg);
#endif
    } else {
        /* Detached thread */
        thread_t tmp;
#ifdef _WIN32
        tmp = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)*thread_function, arg, 0, NULL);
        CloseHandle(tmp);
#else
        pthread_create(&tmp, NULL, thread_function, arg);
        pthread_detach(tmp);
#endif
    }
}

void THREAD_join(thread_t thread)
{
#ifdef _WIN32
    WaitForSingleObject(thread, INFINITE);
#else
    pthread_join(thread, NULL);
#endif
}

void MUTEX_create(mutex_t *mutex)
{
#ifdef _WIN32
    *mutex = CreateMutex(NULL, FALSE, NULL);
#else
    pthread_mutex_init(mutex, NULL);
#endif
}

void MUTEX_destroy(mutex_t *mutex)
{
#ifdef _WIN32
    CloseHandle(*mutex);
#else
    pthread_mutex_destroy(mutex);
#endif
}

void MUTEX_lock(mutex_t *mutex)
{
#ifdef _WIN32
    WaitForSingleObject(*mutex, INFINITE);
#else
    pthread_mutex_lock(mutex);
#endif
}

void MUTEX_unlock(mutex_t *mutex)
{
#ifdef _WIN32
    ReleaseMutex(*mutex);
#else
    pthread_mutex_unlock(mutex);
#endif
}
