#pragma once 

#include <string>
#include "event.hpp"
#include "event_router.hpp"
#include "i_event_receiver.hpp"
#include "common.hpp"

namespace experis {

class EventReceiver : public IEventReceiver, private Uncopyable {
public:
    EventReceiver(EventRouter& a_eventRouter);
    virtual void Receive(SharedPtr<Event> a_sharedPtrEvent);

private:
    EventRouter& m_eventRouter;
};

} //experis
