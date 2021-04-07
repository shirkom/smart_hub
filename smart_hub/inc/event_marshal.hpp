#pragma once

#include "common.hpp"
#include "event.hpp"
#include "shared_ptr.hpp"

namespace experis {

class EventMarshal : private Uncopyable
{
public:
    std::string Marshal(SharedPtr<Event> a_sharedPtrEvent);
};

} // experis
