#include "controller_agent_base.hpp"

namespace experis {

ControllerAgentBase::ControllerAgentBase(ISubscribable* a_subscribable, 
                        const std::string& a_type,
                        const std::string& a_building,
                        const std::string& a_floor,
                        const std::string& a_room) 
{
    if(a_subscribable)
    {
        a_subscribable->Subscribe(a_type, a_building, a_floor, a_room, this);
    }
    else
    {
        throw PtrSubscribeableIsNull();
    }
}

ControllerAgentBase::~ControllerAgentBase() 
{
    /*empty*/
}

} //experis
