
#include "../inc/mutex.hpp"
#include "assert.h"
#include <errno.h>
#include <pthread.h>
#include <exception>


namespace experis {

Mutex::~Mutex() {
    int res = pthread_mutex_destroy(&m_mutex);
    switch (res) {
    case EBUSY:
        assert(!"The implementation has detected an attempt to destroy the object referenced by mutex while it is locked or referenced");
        break;
    case EINVAL:
        assert(!"Attribute is NULL only!");
        break;
    }
}

Mutex::Mutex() {
    int res = pthread_mutex_init(&m_mutex, 0);

    switch (res) {
    case EBUSY:
        assert(!"Can't reinitialize the mutex!");
        break;
    case EINVAL:
        assert(!"Attribute is NULL only!");
        break;
    case EAGAIN:
        assert(!"Only one mutex is being used!");
        break;
    case ENOMEM:
        throw NoMemoryException();
        break;
    case EPERM:
        throw NoPermissionsException();
        break;
    }
}

void Mutex::Lock() {
    int res = pthread_mutex_lock(&m_mutex);
    switch (res) {
    case EBUSY:
        throw BusyException();
        break;
    case EINVAL:
        assert(!"Attribute is NULL only!");
        break;
    case EAGAIN:
        assert(!"Only one mutex is being used!");
        break;
    case ENOMEM:
        throw NoMemoryException();
        break;
    case EDEADLK:
        throw DeadLockException();
        break;
    }
}

void Mutex::Unlock() {
    int res = pthread_mutex_unlock(&m_mutex);
    switch (res) {
    case EBUSY:
        throw BusyException();
        break;
    case EINVAL:
        assert(!"Attribute is NULL only!");
        break;
    case EAGAIN:
        assert(!"Only one mutex is being used!");
        break;
    case ENOMEM:
        throw NoMemoryException();
        break;
    case EPERM:
        throw NoPermissionsException();
        break;
    }
}

const pthread_mutex_t * Mutex::GetMutex() 
{
    return &m_mutex;
}
} //experis