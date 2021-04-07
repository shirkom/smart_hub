#pragma once

#include <list>
#include <cstddef>

#include "common.hpp"
#include "common_exc.hpp"
#include "waitable_queue.hpp"
#include "thread.hpp"
#include "atomicflag.hpp"

#include "shared_ptr.hpp"

namespace experis {

class WorkBase {
public:
    virtual ~WorkBase() {}
    virtual void operator()() const = 0;
};

DEFINE_EXCEPTION(PoolShutdown, "Pool is Shutdown")
DEFINE_EXCEPTION(WorkIsNull, "Work Is Null")

struct DataToThread;
struct ThreadHandler;

class ThreadPool : private Uncopyable {
public:
    explicit ThreadPool(size_t a_numOfWorkers); // create n executors ready to do some work
    ~ThreadPool(); 

    void Submit(const SharedPtr<WorkBase>& a_work);
    size_t WorkItemWaitCount() const NOEXCEPTIONS;

    size_t WorkerCount() const NOEXCEPTIONS; 
    size_t AddWorkers(size_t a_numOfWorkers); //return num of workers added
    void RemoveWorkers(size_t a_numOfWorkersToRemove);
    void RemoveWorkersWithoutCaution(size_t a_numOfWorkersToRemove);

    void ShutdownImmediate(); //no more new work, return immediately and leave work in the queue
    void Shutdown(); // stop accepting new work, wait for all current work to finish
    void Shutdown(size_t a_timeOut); // stop accepting new work, wait for maximun timeOut milliseconds to give chance for the pool to execute all pending work

private:
    static void threadFunc(DataToThread* a_dataToThread);

private:
    static size_t m_ThreadIdSequence;

private:
    void addNumOfWorkers(size_t a_numOfWorkers); 
    size_t GetCurrentMillis() const;
    void takeWork(SharedPtr<WorkBase>& a_work);
    void removeOneWorker(size_t a_threadId);
    void addToThreadToRemove(size_t a_threadId);
    bool shouldRemoveMyself();
    void increaseRemoveCounter(size_t a_numOfWorkersToRemove);

private:
    WaitableQueue<SharedPtr<WorkBase> > m_WorkBaseContainer; 
    std::list<ThreadHandler*> m_workers;
    WaitableQueue<size_t> m_threadToRemove;
    AtomicFlag m_isShutdown;
    size_t m_removeCounter;

    mutable Mutex m_workersMutex;
    mutable Mutex m_removeCounterMutex;
};

struct DataToThread {
    ThreadPool* m_threadPool;
    size_t m_id;
};

struct ThreadHandler {
    Thread<DataToThread>* m_thread;
    size_t m_id;
};

} //experis