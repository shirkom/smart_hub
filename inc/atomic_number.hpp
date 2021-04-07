#pragma once

#include "common.hpp"

namespace experis {

template <class T>
class AtomicNumber {
public:
    explicit AtomicNumber(T a_value);
    void AtomicExchange(T newValue);
    T AtomicRead();
    T operator++();
    T operator--();

private:
    T m_number;
    const AtomicNumber& operator=(const AtomicNumber& a_atomic);
    AtomicNumber(AtomicNumber& a_other);
};

}// experis

#include "inl/atomic_number_inl.tcc"
