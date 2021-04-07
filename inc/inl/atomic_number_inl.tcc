#pragma once
#include "atomic_number.hpp"

namespace experis {

template<class T>
inline AtomicNumber<T>::AtomicNumber(T a_value)
    :m_number(a_value){
}

template<class T>
inline T AtomicNumber<T>::AtomicRead() 
{
    return __sync_val_compare_and_swap(&m_number, m_number, m_number);
}

template<class T>
inline void AtomicNumber<T>::AtomicExchange(T newValue)
{
    __sync_val_compare_and_swap(&m_number, m_number, newValue);
}

template<class T>
inline T AtomicNumber<T>::operator++()
{
   return __sync_add_and_fetch(&m_number, 1);
}

template<class T>
inline T AtomicNumber<T>::operator--()
{
    return __sync_sub_and_fetch(&m_number, 1);
}
}//experis