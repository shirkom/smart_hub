#include "location.hpp"
#include <string>

namespace experis {

Location::Location(const std::string& a_building, const std::string& a_floor, const std::string& a_room)
: m_building(a_building)
, m_floor(a_floor)
, m_room(a_room)
{ }

const std::string& Location::GetBuilding() const 
{
    return m_building;
}

const std::string& Location::GetFloor() const
{
    return m_floor;
}

const std::string& Location::GetRoom() const
{
    return m_room;
}


} //experis
