
#include "sensor_agent_base.hpp"
#include "shared_ptr.hpp"
#include <iostream>

namespace experis {

SensorAgentBase::SensorAgentBase(IEventReceiver* a_receiver) 
: m_receiver(a_receiver)
{
    
}

SensorAgentBase::~SensorAgentBase() 
{
    /*empty*/
}



void SensorAgentBase::SendEvent(SharedPtr<Event> a_sharedPtrEvent) 
{
    m_receiver->Receive(a_sharedPtrEvent);
}
} //experis
