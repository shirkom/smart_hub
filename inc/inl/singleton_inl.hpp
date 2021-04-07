#ifndef SINGLETON_INL_H_
#define SINGLETON_INL_H_

#include "../singleton.hpp"
#include "../atomicflag.hpp"
#include "../atomic.hpp"

namespace experis {

template<class T>
AtomicFlag Singleton<T>::s_flag;

template<class T>
T* Singleton<T>::s_instanceAtomicPtr = 0;

template<class T>
Atomic<T*> Singleton<T>::s_atomicInstanceHandler(&s_instanceAtomicPtr);

template<class T>    
T* Singleton<T>::GetInstance(const FunctorBuilder<T>& a_builderFunc) 
{    
    if(0 == s_atomicInstanceHandler.AtomicRead()) 
    {
        if(s_flag.Acquire()) 
        {
            try {
                s_atomicInstanceHandler.AtomicExchange(a_builderFunc());
            }
            catch(...) {
                s_flag.Release();
                throw;
            }
        }
        else
        {
            s_flag.WaitFor();
            if(0 == s_atomicInstanceHandler.AtomicRead()) 
            {
                throw AllocationFailedSingleton();
            }
        }
    }
    return s_atomicInstanceHandler.AtomicRead();
}  


}//namespace experis
#endif // SINGLETON_INL_H_
