#include "event.hpp"


namespace experis {

Event::Event(const std::string& a_timestamp,
        const std::string& a_type,
        const std::string& a_dataPayload,
        const Location& a_location)
: m_timestamp(a_timestamp)
, m_type(a_type)
, m_dataPayload(a_dataPayload)
, m_location(a_location)
{
    
}


}

