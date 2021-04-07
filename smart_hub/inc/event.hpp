#pragma once

#include "location.hpp"
#include <string>


namespace experis{
class Event {
public:
    Event(const std::string& a_timestamp,
        const std::string& a_type,
        const std::string& a_dataPayload,
        const Location& a_location);
        
public:
    const std::string& m_timestamp;
    const std::string& m_type;
    const std::string& m_dataPayload;
    const Location& m_location;
};




} //experis
