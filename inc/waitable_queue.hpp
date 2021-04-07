#pragma  once 

#include <queue>
#include "mutex.hpp"
#include "common.hpp"
#include "common_exc.hpp"
#include "condition.hpp"

namespace experis {

template <class T>
class WaitableQueue : private Uncopyable {
public:
    template <typename R>
    friend class PredicateWaitableQueueIsFull;
    
    template <typename U>
    friend class PredicateWaitableQueueIsEmpty;

    explicit WaitableQueue(size_t a_maxSize = 0);
    void Enqueue(const T& a_value); 
    void Dequeue(T& a_deletedValue);

    void Clear();
    void Shutdown() NOEXCEPTIONS;
  
    bool IsEmpty() const NOEXCEPTIONS;
    bool IsFull() const NOEXCEPTIONS;

    size_t Size() const NOEXCEPTIONS;
    size_t MaxSize() const;

private:
    bool isEmptyNoneLocking() const NOEXCEPTIONS;
    bool isFullNoneLocking() const NOEXCEPTIONS;
    size_t sizeNoneLocking() const NOEXCEPTIONS;
    bool isWaitingDequeue() const NOEXCEPTIONS;
    bool isWaitingEnqueue() const NOEXCEPTIONS;
    void addWaiterEnqueue() NOEXCEPTIONS;
    void removeWaiterEnqueue() NOEXCEPTIONS;
    void addWaiterDequeue() NOEXCEPTIONS;
    void removeWaiterDequeue() NOEXCEPTIONS;
    
private:
    typedef std::queue<T> Container;
    Container m_container;
    mutable Mutex m_mutex;
    Condition m_notEmpty;
    Condition m_notFull;
    bool m_isShutdown;
    size_t m_countThreadsWaitingEnqueue;
    size_t m_countThreadsWaitingDequeue;
    const size_t m_maxSize; //if m_maxSize is inf - size unlimited
};

DEFINE_EXCEPTION(ShutDownEx, "Shutdown called")

template <class T>
class PredicateWaitableQueueIsFull {
public:
    PredicateWaitableQueueIsFull(const WaitableQueue<T>& a_waitableQueue);
    virtual bool operator()() const ;
private:
    const WaitableQueue<T>& m_waitableQueue;
};

template<class T>
PredicateWaitableQueueIsFull<T>::PredicateWaitableQueueIsFull(const WaitableQueue<T>& a_waitableQueue) 
 : m_waitableQueue(a_waitableQueue){}
 
template<class T>
bool PredicateWaitableQueueIsFull<T>::operator()() const 
{
   return m_waitableQueue.isFullNoneLocking() && !m_waitableQueue.m_isShutdown;;
}

template <class T>
class PredicateWaitableQueueIsEmpty {
public:
    PredicateWaitableQueueIsEmpty(const WaitableQueue<T>& a_waitableQueue);
    virtual bool operator()() const;
private:
    const WaitableQueue<T>& m_waitableQueue;
};

template<class T>
PredicateWaitableQueueIsEmpty<T>::PredicateWaitableQueueIsEmpty(const WaitableQueue<T>& a_waitableQueue) 
: m_waitableQueue(a_waitableQueue){}
 
template<class T>
bool PredicateWaitableQueueIsEmpty<T>::operator()() const 
{
    return m_waitableQueue.isEmptyNoneLocking() && !m_waitableQueue.m_isShutdown;
}
} //experis

#include "inl/waitable_queue.tcc"
