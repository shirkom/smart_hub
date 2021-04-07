#ifndef CONDITION_INL_H__
#define CONDITION_INL_H__

#include "../condition.hpp"
#include "../mutex.hpp"

namespace experis {

template<typename Predicate>
void Condition::WaitWhile(Mutex& a_mutex, Predicate a_predicate) {
    while (a_predicate()) {
        wait(a_mutex);
    }
}

}//experis
#endif // CONDITION_INL_H__