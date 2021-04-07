#pragma once 

#include "waitable_queue.hpp"
#include "locker.hpp"
#include <iostream>

namespace experis {


template<class T>
WaitableQueue<T>::WaitableQueue(size_t a_maxSize)
: m_isShutdown(false)
, m_countThreadsWaitingEnqueue(0)
, m_countThreadsWaitingDequeue(0)
, m_maxSize(a_maxSize)
{
    
}

template <class T>
void WaitableQueue<T>::Enqueue(const T& a_val) {
    Locker lkr(m_mutex);
    if(m_isShutdown) {
        throw ShutDownEx();
    }
    addWaiterEnqueue();
    PredicateWaitableQueueIsFull<T> functorIsFull(*this);
    m_notFull.WaitWhile(m_mutex, functorIsFull);
    removeWaiterEnqueue();
    
    if(m_isShutdown) {
        m_notFull.Broadcast();
        throw ShutDownEx();
    }
    assert(!isFullNoneLocking());
    m_container.push(a_val);
    if(isWaitingDequeue())
    {
        m_notEmpty.Broadcast();
    }
}

template <class T>
void WaitableQueue<T>::Dequeue(T& a_item) {   
    Locker lkr(m_mutex);
    if(m_isShutdown) {
        throw ShutDownEx();
    }
     
    addWaiterDequeue();
    PredicateWaitableQueueIsEmpty<T> functorIsEmpty(*this);
    m_notEmpty.WaitWhile(m_mutex, functorIsEmpty);
    removeWaiterDequeue();
     
    
    if(m_isShutdown) {
        m_notEmpty.Broadcast();
        
        throw ShutDownEx();
    }

    assert(!isEmptyNoneLocking());

    a_item = m_container.front();
    m_container.pop();
    
    if(isWaitingEnqueue())
    {
        m_notFull.Broadcast();
    }
}

template<class T>
void WaitableQueue<T>::Shutdown() NOEXCEPTIONS
{
    {
        Locker lkr(m_mutex);
        m_isShutdown = true;
        m_notEmpty.Broadcast();
        m_notFull.Broadcast();
    }
    while(true) 
    {
        Locker lkr(m_mutex);
        if(!isWaitingDequeue() && !isWaitingEnqueue()) 
        {
            break;
        }
    }
}

template<class T>
void WaitableQueue<T>::Clear() 
{
    Locker lkr(m_mutex);
    for(size_t i = 0; i < sizeNoneLocking(); ++i) 
    {
        m_container.pop();
    }
}

template<class T>
bool WaitableQueue<T>::IsEmpty() const NOEXCEPTIONS
{
   return Size() == 0;
}

template<class T>
bool WaitableQueue<T>::IsFull() const NOEXCEPTIONS
{
    Locker lkr(m_mutex);
    return sizeNoneLocking() == MaxSize(); 
}

template<class T>
size_t WaitableQueue<T>::MaxSize() const
{
   if (m_maxSize == 0)
   {
       return ~((size_t)(0));
   }
   return m_maxSize;
}

template<class T>
size_t WaitableQueue<T>::Size() const NOEXCEPTIONS
{
   Locker lkr(m_mutex);
   return m_container.size();
}

template<class T>
size_t WaitableQueue<T>::sizeNoneLocking() const NOEXCEPTIONS
{
   return m_container.size();
}

template<class T>
bool WaitableQueue<T>::isEmptyNoneLocking() const NOEXCEPTIONS
{
   return sizeNoneLocking() == 0;
}

template<class T>
bool WaitableQueue<T>::isFullNoneLocking() const NOEXCEPTIONS
{
   return sizeNoneLocking() == MaxSize();
}

template<class T>
bool WaitableQueue<T>::isWaitingEnqueue() const NOEXCEPTIONS
{
    return m_countThreadsWaitingEnqueue > 0;
}

template<class T>
void WaitableQueue<T>::addWaiterEnqueue() NOEXCEPTIONS
{
    ++m_countThreadsWaitingEnqueue;
}

template<class T>
void WaitableQueue<T>::removeWaiterEnqueue() NOEXCEPTIONS
{
    --m_countThreadsWaitingEnqueue;
}

template<class T>
bool WaitableQueue<T>::isWaitingDequeue() const NOEXCEPTIONS
{
    return m_countThreadsWaitingDequeue > 0;
}

template<class T>
void WaitableQueue<T>::addWaiterDequeue() NOEXCEPTIONS
{
    ++m_countThreadsWaitingDequeue;
}

template<class T>
void WaitableQueue<T>::removeWaiterDequeue() NOEXCEPTIONS
{
    --m_countThreadsWaitingDequeue;
}

} //experis  
