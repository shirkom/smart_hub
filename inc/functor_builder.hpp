#pragma once

namespace experis {
template <class T>
class FunctorBuilder {
public:
    virtual ~FunctorBuilder() {}
    virtual T* operator()() const = 0;
};
} //experis