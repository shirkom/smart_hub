#ifndef ATOMIC_H__
#define ATOMIC_H__

 #include "common.hpp"

namespace experis {

template <class T>
class Atomic : public Uncopyable {
public:
    Atomic(T * a_val);
    bool AtomicExchange(T newValue);
    T AtomicRead();

private:
    T* m_value;
};

} //experis

#endif // ATOMIC_H__