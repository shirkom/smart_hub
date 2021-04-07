#pragma once

#include <unistd.h>
#include "locker.hpp"
#include "mutex.hpp"
#include "common_exc.hpp"
#include "common.hpp"
#include "atomic_number.hpp"

namespace experis {

template <typename T>
class SharedPtr {
public:
    template<typename D>
    friend class SharedPtr; 

public:   
    typedef void (*FP)(void);

    ~SharedPtr();
    explicit SharedPtr(T* a_ptr = 0); 
    SharedPtr(const SharedPtr& a_other);
    SharedPtr& operator=(const SharedPtr& a_other);

    T* operator->();
    const T* operator->() const;

    T& operator*();
    const T& operator*() const;

    template <typename R>
    SharedPtr(const SharedPtr<R>& a_other);
    
    template <typename R>
    SharedPtr& operator=(const SharedPtr<R>& a_other);
    
    template <typename R>
    bool operator==(const SharedPtr<R>& a_other) const;

    template <typename R>
    bool operator!=(const SharedPtr<R>& a_other) const;

    operator FP() const;
    operator FP();
    static void fp(void);

    size_t GetCount();

private:
    void init(T* a_ptr);
    void init(const SharedPtr& a_other);
    void decreaseCounter();

private:
    static T* s_nullSentinel; //SharedPtr??

private:
    T* m_ptr;
    AtomicNumber<size_t>* m_count;
    
};

DEFINE_EXCEPTION(PtrIsNull, "Ptr Is Null")
DEFINE_EXCEPTION(errorFuncDoNotNeedToCall, "Error  - Func Do Not Need To Call")


} // experis

#include "inl/shared_ptr_inl.tcc"


