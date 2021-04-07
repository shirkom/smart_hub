#ifndef THREAD_INL_H__
#define THREAD_INL_H__

#include "../thread.hpp"
#include <cassert>
#include <cerrno>


namespace experis {

typedef void* (*ThreadInnerFunc)(void* a_context);
typedef void (*ThreadFunc2)(void* a_context);

template <class T>
Thread<T>::~Thread() {
    int res = pthread_join(m_thread, NULL);

    switch (res) {
        case EDEADLK:
            assert(!"A deadlock was detected or thread specifies the calling thread.");
            break;
        case EINVAL:
            assert(!"thread is not a joinable thread, Another thread is already waiting to join with this thread");
            break;
        case ESRCH:
            assert(!"No thread with the ID thread could be found");
            break;
    }
}

template <class T>
void Thread<T>::CheckCtorResult(int a_res) {
    switch (a_res) {
        case EAGAIN:
            throw InsufficientResourcesException();
            break;
        case EINVAL:
            assert(!"Invalid settings in attr");
            break;
        case EPERM:
            assert(!"No permission to set the scheduling policy and parameters specified in attr.");
            break;
    }
}

template <class T>
Thread<T>::Thread(ThreadFunc a_func, T* a_context)
    : m_thread() {
    /* ThreadInnerFunc func = reinterpret_cast<ThreadInnerFunc>(a_func); */
    std::pair<ThreadFunc, T*>* args = new std::pair<ThreadFunc, T*>;
    args->first = a_func;
    args->second = a_context;

    int res = pthread_create(&m_thread, NULL, somefunc, args);

    CheckCtorResult(res);
}

template <class T>
Thread<T>::Thread(T a_functor) {
    int res = pthread_create(&m_thread, NULL, somefunc2, &a_functor);

    CheckCtorResult(res);
}

template <class T>
void* Thread<T>::somefunc2(void* a_functor) {
    T v = *(static_cast<T*>(a_functor));
    v();
    return NULL;
}

template <class T>
void* Thread<T>::somefunc(void* a_pair) {
    std::pair<ThreadFunc, void*>* args = static_cast<std::pair<ThreadFunc, void*>*>(a_pair);
    T* cont = static_cast<T*>(args->second);
    args->first(cont);

    return NULL;
}

template<class T>
void Thread<T>::Kill() 
{
    pthread_cancel(m_thread);
}


}  // namespace experis



#endif // THREAD_INL_H__