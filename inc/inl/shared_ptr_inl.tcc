#pragma once
#include "shared_ptr.hpp"
#include "locker.hpp"

namespace experis {

template <typename T>
T* SharedPtr<T>::s_nullSentinel = 0;

template <typename T>
SharedPtr<T>::~SharedPtr() 
{
    if(--(*m_count) == 0)
    {
        delete m_count;
        if (m_ptr != s_nullSentinel) 
        {
            delete m_ptr;
        }
        return;
    }
}
template <typename T>
SharedPtr<T>::SharedPtr(T* a_ptr)
: m_ptr(a_ptr ? a_ptr : s_nullSentinel)
{
    try
    {
        m_count = new AtomicNumber<size_t>(1);
    }
    catch(const std::bad_alloc& e)
    {
        delete m_ptr;
        throw;
    }
}
template <typename T>
SharedPtr<T>::SharedPtr(const SharedPtr& a_other) 
:m_ptr(a_other.m_ptr)
,m_count(a_other.m_count)
{    
    ++(*m_count);  
}
template <typename T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr& a_other) 
{
    if(*this != a_other)
    {
        decreaseCounter();
        m_ptr = a_other.m_ptr;
        m_count = a_other.m_count;
        ++(*m_count);
    }
    return *this; 
}
template <typename T>
void SharedPtr<T>::decreaseCounter()
{
    if(--(*m_count) == 0)
    {
        delete m_count;
        delete m_ptr;
    }        
}

template <typename T>
T* SharedPtr<T>::operator->() 
{
    return m_ptr;
}

template <typename T>
T& SharedPtr<T>::operator*() 
{
    if (0 == m_ptr)
    {
        throw PtrIsNull();
    }
    return *m_ptr; 
}

template<typename T>
const T* SharedPtr<T>::operator->() const
{
    return m_ptr;
}

template<typename T>
const T& SharedPtr<T>::operator*() const
{
    if (0 == m_ptr)
    {
        throw PtrIsNull();
    }
    return *m_ptr;
}

template <typename T>
SharedPtr<T>::operator FP() const
{
    return m_ptr == s_nullSentinel ? 0 : fp;
}

template <typename T>
SharedPtr<T>::operator FP() 
{
    return m_ptr == s_nullSentinel ? 0 : fp;
}

template <typename T>
void SharedPtr<T>::fp(void) 
{
    throw errorFuncDoNotNeedToCall();
}

template <typename T>
template<typename R>
SharedPtr<T>::SharedPtr(const SharedPtr<R>& a_other) 
: m_ptr(a_other.m_ptr)
, m_count(a_other.m_count)
{     
    ++(*m_count);  
}

template <typename T>
template <typename R>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<R>& a_other) 
{
    if(*this != a_other)
    {
        decreaseCounter();
        m_ptr = a_other.m_ptr;
        m_count = a_other.m_count;
        ++(*m_count);
    }
    return *this; 
}

template <typename T>
template <typename R>
bool SharedPtr<T>::operator==(const SharedPtr<R>& a_other) const
{
    return a_other.m_ptr == m_ptr;
}

template <typename T>
template <typename R>
bool SharedPtr<T>::operator!=(const SharedPtr<R>& a_other) const
{
    return !(a_other.m_ptr == m_ptr);
}

template <typename T>
size_t SharedPtr<T>::GetCount() 
{
    {return m_count->AtomicRead();}
}
} //experis


// PTR -> s_nullSentinel - if(PTR) -> if(NULL) -> FALSE
// PTR -> ANY_OTHER_PTR  - if(PTR) -> if(fp) -> TRUE
