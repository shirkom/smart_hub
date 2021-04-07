#ifndef CONDITION_H__
#define CONDITION_H__

#include "mutex.hpp"
#include "common.hpp"
#include "common_exc.hpp"

#include <cassert>
#include <pthread.h>
#include <errno.h>
#include <exception>

namespace experis {

class ConditionBeingUsed;
class MutexWasNotOwnedByTheCurrentThread;

class Condition : private Uncopyable {
public:
    Condition();
    ~Condition();

    void Broadcast();
    

    template<typename Predicate>
    void WaitWhile(Mutex& a_mutex, Predicate a_predicate);

    void Signal();

private:
    //Condition(Condition& a_conditionToCopy);
   // Condition& operator=(Condition& a_conditionToCopy);
   void wait(Mutex& a_mutex);
private:

    pthread_cond_t m_condition;
};

DEFINE_EXCEPTION(ConditionBeingUsed, "Condition Being Used")
DEFINE_EXCEPTION(MutexWasNotOwnedByTheCurrentThread, "Condition Being Used")
DEFINE_EXCEPTION(NoMemoryException, "Condition Being Used")
 

} //experis

#include "inl/condition_inl.tcc"

#endif // CONDITION_H__
