#include "../atomic.hpp"

namespace experis
{

template<class T>
Atomic<T>::Atomic(T * a_val) 
: m_value(a_val)
{
    
}

template <class T>
T Atomic<T>::AtomicRead() 
{ 
   return __sync_val_compare_and_swap(m_value, *m_value, *m_value);
}

template <class T>
bool Atomic<T>::AtomicExchange(T a_newValue) 
{
    return __sync_bool_compare_and_swap(m_value, *m_value, a_newValue);
}


}//namespace experis

