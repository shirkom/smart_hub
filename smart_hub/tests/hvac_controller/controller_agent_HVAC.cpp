#include <map>
#include <utility>
#include <algorithm>
#include <iostream>
#include <exception>
#include <new>
#include "controller_agent_HVAC.hpp"
#include "common.hpp"

namespace experis
{
ControllerAgentHVAC::ControllerAgentHVAC(ISubscribable* a_subscribable, 
                        const std::string& a_building,
                        const std::string& a_type,
                        const std::string& a_floor,
                        const std::string& a_room) 
: ControllerAgentBase(a_subscribable, a_type, a_building, a_floor, a_room)
{
        
}

void ControllerAgentHVAC::PublishEvent(SharedPtr<Event> a_sharedPtrEvent) 
{
    std::cout << "Air Condition is On" << std::endl;
}

extern "C"
{
    IAgent* Create(const DataAgent& a_data, ISubscribable* a_subscribable, IEventReceiver* a_eventReceiver) 
    {
        std::cout << "creating" << std::endl;
        ControllerAgentHVAC *newControll = 0;
        try
        {
            newControll = new ControllerAgentHVAC(a_subscribable, "1", a_data.m_type, a_data.m_floor, a_data.m_room);
        } catch (std::bad_alloc& ba)
        {
            throw;
        }
        return newControll;
    }
}
//////////////////////////////////////////



}//namespace experis