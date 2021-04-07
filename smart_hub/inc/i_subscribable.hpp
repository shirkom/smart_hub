#pragma once 

#include <string>
#include "i_publishable.hpp"

namespace experis {

class ISubscribable {
public:
    virtual ~ISubscribable() {}
    virtual void Subscribe(
                const std::string& a_type, 
                const std::string& a_building,
                const std::string& a_floor, 
                const std::string& a_room, 
                IPublishable* a_controller) = 0;
};

} //experis
