#ifndef THREAD_POOL_INL_H__
#define THREAD_POOL_INL_H__

#include <sys/time.h>

#include "thread_pool.hpp"
#include "common.hpp"
#include "locker.hpp"
#include "mutex.hpp"
#include <algorithm>

namespace experis {

size_t ThreadPool::m_ThreadIdSequence = 0;

class EmptyTaskToRemove : public WorkBase {
public:
    ~EmptyTaskToRemove() {}
    void operator()() const {}
};

struct HasId{
    size_t id;
    HasId(size_t id) : id(id) {}
    bool operator()(ThreadHandler* p) const;
};

bool HasId::operator()(ThreadHandler* p) const
{
    return id == p->m_id; 
}

void ThreadPool::threadFunc(DataToThread* a_dataToThread)
{
    SharedPtr<WorkBase> work;
    ThreadPool * threadPool = a_dataToThread->m_threadPool;
    size_t id = a_dataToThread->m_id;
    delete a_dataToThread;
    while(true)
    {
        if(threadPool->shouldRemoveMyself())
        {
            threadPool->addToThreadToRemove(id);
            return;
        }
        try
        {
            threadPool->takeWork(work);
            (*work)();

        }
        catch(ShutDownEx& ex)
        {
            break;
        }
        catch(...){}
    }
}


bool ThreadPool::shouldRemoveMyself()
{
    Locker lkr(m_removeCounterMutex);
    if(m_removeCounter != 0)
    {
        --m_removeCounter;
        return true;
    }
    return false;
}

void ThreadPool::addToThreadToRemove(size_t a_threadId)
{
    m_threadToRemove.Enqueue(a_threadId);
}


void ThreadPool::takeWork(SharedPtr<WorkBase>& a_work)
{
    m_WorkBaseContainer.Dequeue(a_work);
}

ThreadPool::ThreadPool(size_t a_numOfWorkers)
: m_removeCounter(0)
{
    AddWorkers(a_numOfWorkers);    
    m_isShutdown.Release(); //false
}

ThreadPool::~ThreadPool() 
{
    if(!m_isShutdown.IsUp()) 
    {
        std::list<ThreadHandler*>::iterator itr = m_workers.begin();
        std::list<ThreadHandler*>::iterator itrEnd = m_workers.end();
        for(; itr != itrEnd; ++itr) 
        {
            delete *itr;
        }
    }
}

void ThreadPool::Submit(const SharedPtr<WorkBase>& a_work)
{
    if(0 == a_work)
    {
        throw WorkIsNull();
    }
    if(m_isShutdown.IsUp())
    {
        throw PoolShutdown();
    }
    m_WorkBaseContainer.Enqueue(a_work);  
}

size_t ThreadPool::WorkerCount() const NOEXCEPTIONS
{
    Locker lkr(m_workersMutex);
    return m_workers.size();
}

size_t ThreadPool::WorkItemWaitCount() const NOEXCEPTIONS
{
    return m_WorkBaseContainer.Size();
}

size_t ThreadPool::AddWorkers(size_t a_numOfWorkers) 
{
    size_t i = 0;
    Locker lkr(m_workersMutex);
    for(size_t i = 0; i < a_numOfWorkers; ++i) 
    {
        ThreadHandler* handler = new ThreadHandler;
        DataToThread* dataToThread = new DataToThread;
        handler->m_id = ++m_ThreadIdSequence;
        dataToThread->m_id = handler->m_id;
        dataToThread->m_threadPool = this;
        handler->m_thread = new Thread<DataToThread>(threadFunc, dataToThread);
        
        m_workers.push_back(handler);        
    }    
    return i;
}

void ThreadPool::RemoveWorkersWithoutCaution(size_t a_numOfWorkersToRemove) 
{
    for (size_t i = 0; i < a_numOfWorkersToRemove && WorkerCount() > 0; ++i)
    {
        ThreadHandler* handler = m_workers.back();
        Locker lkr(m_workersMutex);
        m_workers.pop_back();
        delete handler->m_thread;
        delete handler;
    }   
}

void ThreadPool::ShutdownImmediate() 
{   
    m_isShutdown.Acquire();
    m_WorkBaseContainer.Shutdown();
    RemoveWorkersWithoutCaution(WorkerCount());
}

void ThreadPool::Shutdown() 
{
    m_isShutdown.Acquire();
    if(WorkerCount() != 0)
    {
        while(!m_WorkBaseContainer.IsEmpty()) 
        {; } 
    }
    m_WorkBaseContainer.Shutdown();
    RemoveWorkersWithoutCaution(WorkerCount());
}

void ThreadPool::Shutdown(size_t a_timeOut) 
{
    m_isShutdown.Acquire();
    size_t startTime = GetCurrentMillis();
    if(WorkerCount() != 0)
    {
        while(!m_WorkBaseContainer.IsEmpty() && GetCurrentMillis() - startTime < a_timeOut) 
        {; } 
    }
    m_WorkBaseContainer.Shutdown();
    RemoveWorkersWithoutCaution(WorkerCount());
}

size_t ThreadPool::GetCurrentMillis() const
{
    struct timeval tp;
    gettimeofday(&tp, NULL);
    long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
    return ms;
}

void ThreadPool::RemoveWorkers(size_t a_numOfWorkersToRemove) 
{
    if(a_numOfWorkersToRemove > WorkerCount()) {
        a_numOfWorkersToRemove = WorkerCount();
    }

    for(size_t i = 0; i < a_numOfWorkersToRemove && WorkerCount() > 0; ++i)
    {
        {
            Locker lkr(m_removeCounterMutex);
            ++m_removeCounter;
        }
        SharedPtr<WorkBase> emptyTask(new EmptyTaskToRemove);
        m_WorkBaseContainer.Enqueue(emptyTask);
    }
    
    for(size_t i = 0; i < a_numOfWorkersToRemove && WorkerCount() > 0; ++i)
    {
        size_t id;
        m_threadToRemove.Dequeue(id);
        removeOneWorker(id);
    }
}


void ThreadPool::removeOneWorker(size_t a_threadId)
{
    std::list<ThreadHandler*>::iterator itr =
            std::find_if(m_workers.begin(), m_workers.end(), HasId(a_threadId));
    if(itr != m_workers.end()){
        ThreadHandler* handler = *itr;
        delete handler->m_thread;
        delete handler;
        m_workers.erase(itr);
    }
            std::remove_if(m_workers.begin(), m_workers.end(), HasId(a_threadId));
}


} // experis
#endif // THREAD_POOL_INL_H__