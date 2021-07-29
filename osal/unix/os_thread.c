/*
 * Copyright (c) 2018-2021 Qinglong<sysu.zqlong@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include "osal/os_thread.h"

os_thread os_thread_create(struct os_thread_attr *attr, void *(*cb)(void *arg), void *arg)
{
    pthread_attr_t tattr;
    int detachstate = PTHREAD_CREATE_JOINABLE;
    if (attr && !attr->joinable)
        detachstate = PTHREAD_CREATE_DETACHED;
    pthread_attr_init(&tattr);
    pthread_attr_setdetachstate(&tattr, detachstate);
#if defined(OS_RTOS)
    if (attr != NULL) {
        struct sched_param tsched;
        tsched.sched_priority = attr->priority;
        pthread_attr_setschedparam(&tattr, &tsched);
        pthread_attr_setstacksize(&tattr, attr->stacksize);
    }
#endif
    pthread_t tid;
    int ret = pthread_create(&tid, &tattr, cb, arg);
    pthread_attr_destroy(&tattr);
    if (ret != 0)
        return NULL;
    return (os_thread)tid;
}

int os_thread_join(os_thread thread, void **retval)
{
    return pthread_join((pthread_t)thread, retval);
}

int os_thread_detach(os_thread thread)
{
    return pthread_detach((pthread_t)thread);
}

os_mutex os_mutex_create()
{
    pthread_mutex_t *mutex = calloc(1, sizeof(pthread_mutex_t));
    if (mutex == NULL)
        return NULL;
    if (pthread_mutex_init(mutex, NULL) != 0) {
        free(mutex);
        return NULL;
    }
    return (os_mutex)mutex;
}

int os_mutex_lock(os_mutex mutex)
{
    return pthread_mutex_lock((pthread_mutex_t *)mutex);
}

int os_mutex_trylock(os_mutex mutex)
{
    return pthread_mutex_trylock((pthread_mutex_t *)mutex);
}

int os_mutex_unlock(os_mutex mutex)
{
    return pthread_mutex_unlock((pthread_mutex_t *)mutex);
}

void os_mutex_destroy(os_mutex mutex)
{
    pthread_mutex_destroy((pthread_mutex_t *)mutex);
    free(mutex);
}

os_cond os_cond_create()
{
    pthread_cond_t *cond = calloc(1, sizeof(pthread_cond_t));
    if (cond == NULL)
        return NULL;
    pthread_condattr_t attr;
    pthread_condattr_init(&attr);
#if !defined(OS_APPLE)
    pthread_condattr_setclock(&attr, CLOCK_MONOTONIC);
#endif
    int ret = pthread_cond_init(cond, &attr);
    pthread_condattr_destroy(&attr);
    if (ret != 0) {
        free(cond);
        return NULL;
    }
    return (os_cond)cond;
}

int os_cond_wait(os_cond cond, os_mutex mutex)
{
    return pthread_cond_wait((pthread_cond_t *)cond, (pthread_mutex_t *)mutex);
}

int os_cond_timedwait(os_cond cond, os_mutex mutex, unsigned long usec)
{
    struct timespec ts;
#if !defined(OS_APPLE)
    clock_gettime(CLOCK_MONOTONIC, &ts);
#else
    clock_gettime(CLOCK_REALTIME, &ts);
#endif
    ts.tv_sec += usec / 1000000;
    ts.tv_nsec += (usec % 1000000) * 1000;
    while (ts.tv_nsec >= 1000000000L) {
        ts.tv_sec++;
        ts.tv_nsec %= 1000000000L;
    }
    return pthread_cond_timedwait((pthread_cond_t *)cond, (pthread_mutex_t *)mutex, &ts);
}

int os_cond_signal(os_cond cond)
{
    return pthread_cond_signal((pthread_cond_t *)cond);
}

void os_cond_destroy(os_cond cond)
{
    pthread_cond_destroy((pthread_cond_t *)cond);
    free(cond);
}

void os_thread_sleep_usec(unsigned long usec)
{
    usleep(usec);
}

void os_thread_sleep_msec(unsigned long msec)
{
    usleep(msec * 1000);
}
