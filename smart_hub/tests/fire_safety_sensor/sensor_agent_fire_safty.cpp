#include "sensor_agent_fire_safty.hpp"
#include <iostream>

namespace experis {

SensorAgentFireSafety::SensorAgentFireSafety(IEventReceiver* a_receiver) 
: SensorAgentBase(a_receiver)
{
    thread = new Thread<SensorAgentFireSafety>(SensorAgentFireSafety::SendTemp, this);
}

experis::SensorAgentFireSafety::~SensorAgentFireSafety() 
{
    thread->Kill();
    delete thread;
}

void SensorAgentFireSafety::SendTemp(SensorAgentFireSafety* sensor)
{
    sleep(1);
    while(true)
    {
        Location location("1", "1", "room_1_a");
        Event event("7", "TestSprinkler", "sprinkel_on", location);
        SharedPtr<Event> eventSharedPtr(new Event(event));
        std::cout << "sending FireSafety" << std::endl;
        sensor->SendEvent(eventSharedPtr);
        sleep(3);
    }
}

extern "C"
{
    IAgent* Create(const DataAgent& a_data, ISubscribable* a_subscribable, IEventReceiver* a_eventReceiver) 
    {
        SensorAgentFireSafety* newSensor = 0;
        try
        {
            newSensor = new SensorAgentFireSafety(a_eventReceiver);
        }
        catch (std::bad_alloc& ba)
        {
            throw;
        }
        return newSensor;
    }
}

} //experis
