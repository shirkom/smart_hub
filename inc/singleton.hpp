#ifndef SINGLETON_H_
#define SINGLETON_H_

#include <exception>

#include "common.hpp"
#include  "mutex.hpp"
#include  "atomic.hpp"
#include  "atomicflag.hpp"
#include "functor_builder.hpp"

namespace experis {

class AllocationFailed;

template<typename T>
class Singleton : private Uncopyable {
public:
    static T* GetInstance(const FunctorBuilder<T>& a_builderFunc); 

private:
    Singleton(); //m_flag will be created by defult ctor 
    ~Singleton();

private:
    static void WaitForFinish();

private:
    static AtomicFlag s_flag; 
    static T* s_instanceAtomicPtr;
    static Atomic<T*> s_atomicInstanceHandler;
};


class AllocationFailedSingleton : public std::exception {
public:
    virtual const char* what() const throw() { return "Allocation failed."; };
};

} //namespace experis

#include "inl/singleton_inl.hpp"

#endif // SINGLETON_H_
