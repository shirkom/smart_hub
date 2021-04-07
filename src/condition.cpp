
#include "condition.hpp"
#include <iostream>


namespace experis {

Condition::Condition() 
{
    pthread_cond_t cond;
    int result = pthread_cond_init(&cond, NULL);
    if(result != 0) {
        if(result == ENOMEM) {
            throw NoMemoryException();
        }
    }
    assert(EBUSY != result);
    assert(EAGAIN != result);
    assert(EINVAL != result);
    m_condition = cond;
}

void Condition::Signal() {
    pthread_cond_signal(&m_condition);
}

Condition::~Condition() {
    int result = pthread_cond_destroy(&m_condition);
    if(EBUSY == result) {
       std::terminate(); 
    }
    assert(EINVAL != result);
}

void Condition::wait(Mutex& a_mutex) {
    int result = pthread_cond_wait(&m_condition, &(a_mutex.m_mutex));
    assert(EINVAL != result);
    if(result) {
        throw MutexWasNotOwnedByTheCurrentThread();
    }
}

void Condition::Broadcast() {
    pthread_cond_broadcast(&m_condition);
}



} //experis
