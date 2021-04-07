#include <map>
#include <utility>
#include <algorithm>
#include <iostream>
#include <exception>
#include <new>
#include "controller_agent_sprinkler.hpp"

namespace experis
{
ControllerAgentSprinkler::ControllerAgentSprinkler(ISubscribable* a_subscribable, 
                        const std::string& a_building,
                        const std::string& a_type,
                        const std::string& a_floor,
                        const std::string& a_room) 
: ControllerAgentBase(a_subscribable, a_type, a_building, a_floor, a_room)
{
        
}

void ControllerAgentSprinkler::PublishEvent(SharedPtr<Event> a_sharedPtrEvent) 
{
    std::cout << "Sprinkler is on" << std::endl;
}

extern "C"
{
    IAgent* Create(const DataAgent& a_data, ISubscribable* a_subscribable, IEventReceiver* a_eventReceiver) 
    {
        ControllerAgentSprinkler *newControll = 0;
        try
        {
            newControll =  new ControllerAgentSprinkler(a_subscribable, "1", a_data.m_type, a_data.m_floor, a_data.m_room);
        } 
        catch (std::bad_alloc& ba)
        {
            throw;
        }
        return newControll;
    }
}

}// experis
