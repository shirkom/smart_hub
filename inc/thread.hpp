#ifndef THREAD_H__
#define THREAD_H__

#include <pthread.h>

#include <exception>


namespace experis {
template <class T>
class Thread {
   public:
    class InsufficientResourcesException;

    typedef void (*ThreadFunc)(T* a_context);

    ~Thread();
    explicit Thread(ThreadFunc a_func, T* a_context);  // Could throw InsufficientResourcesException

    explicit Thread(T a_functor);

    void Kill();
    
   private:
    static void* somefunc(void* a_func);
    static void* somefunc2(void* a_func);
    const Thread& operator=(const Thread& a_t);
    Thread(const Thread& a_t);
    void CheckCtorResult(int a_res);

    pthread_t m_thread;
};



template <class T>
class Thread<T>::InsufficientResourcesException : public std::exception {
   public:
    virtual const char* what() const throw() { return "Insufficient resources to create another thread"; };
};
}  // namespace experis

#include "inl/thread_inl.hpp"

#endif  // THREAD_H__
