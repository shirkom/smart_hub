#pragma once

#include <string>

namespace experis {
class Location {
public:
    Location(const std::string& a_building, const std::string& a_floor, const std::string& a_room);

    const std::string& GetBuilding() const;
    const std::string& GetFloor() const;
    const std::string& GetRoom() const;

private:
    const std::string m_building;
    const std::string m_floor;
    const std::string m_room;
};



} //experis
