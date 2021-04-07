#include "event_receiver.hpp"
#include "event_router.hpp"

namespace experis {

void EventReceiver::Receive(SharedPtr<Event> a_sharedPtrEvent) 
{
    m_eventRouter.SendEvent(a_sharedPtrEvent);
}

EventReceiver::EventReceiver(EventRouter& a_eventRouter) 
: m_eventRouter(a_eventRouter)
{
    
}

} // experis
