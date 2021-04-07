#include <string>

#include "event_marshal.hpp"
#include "event.hpp"
#include "shared_ptr.hpp"

namespace experis {

std::string EventMarshal::Marshal(SharedPtr<Event> a_sharedPtrEvent) 
{
    return a_sharedPtrEvent->m_timestamp + " " + a_sharedPtrEvent->m_dataPayload + " " 
            + a_sharedPtrEvent->m_type + " " + a_sharedPtrEvent->m_location.GetBuilding() 
            + " " + a_sharedPtrEvent->m_location.GetFloor() + " " + a_sharedPtrEvent->m_location.GetRoom();
}

}//experis;
