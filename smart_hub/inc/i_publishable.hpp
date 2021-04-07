#pragma once

#include "event.hpp"
#include "../../inc/shared_ptr.hpp"

namespace experis{
class IPublishable {
public:
    virtual ~IPublishable() {}
    virtual void PublishEvent(SharedPtr<Event> a_sharedPtrEvent) = 0;
private:
};
} //experis
