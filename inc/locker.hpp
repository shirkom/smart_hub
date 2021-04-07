#pragma once

#include "mutex.hpp"

namespace experis {

class Locker {
public:
    inline Locker(Mutex& mutex);
    inline ~Locker();

private:
    Mutex& m_mutex;
};

inline Locker::Locker(Mutex& mutex)
: m_mutex(mutex)
{
    m_mutex.Lock();
}

inline Locker::~Locker()
{
    m_mutex.Unlock();
}

}// experis
