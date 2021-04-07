#pragma once 

#include <string>
#include "event.hpp"
#include "shared_ptr.hpp"

namespace experis {

class IEventReceiver {
public:
    virtual ~IEventReceiver() {}
    virtual void Receive(SharedPtr<Event> a_sharedPtrEvent) = 0;
};

} //experis
