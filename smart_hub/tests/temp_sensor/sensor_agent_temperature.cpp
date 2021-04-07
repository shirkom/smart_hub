#include "sensor_agent_temperature.hpp"
#include <iostream>

namespace experis {
SensorAgentTemperature::SensorAgentTemperature(IEventReceiver* a_receiver) 
: SensorAgentBase(a_receiver)
{
    thread = new Thread<SensorAgentTemperature>(SensorAgentTemperature::SendTemp, this);
}

experis::SensorAgentTemperature::~SensorAgentTemperature() 
{
    thread->Kill();
    delete thread;
}

void SensorAgentTemperature::SendTemp(SensorAgentTemperature* sensor)
{
    sleep(1);
    while(true)
    {
        Location location("1", "1", "room_1_a");
        Event event("7", "TestHVAC", "lights", location);
        SharedPtr<Event> eventSharedPtr(new Event(event));
        std::cout << "sending Temperature" << std::endl;
        sensor->SendEvent(eventSharedPtr);
        sleep(3);
    }
}

extern "C"
{
    IAgent* Create(const DataAgent& a_data, ISubscribable* a_subscribable, IEventReceiver* a_eventReceiver) 
    {

        SensorAgentTemperature* newSensor = 0;
        try
        {
            newSensor = new SensorAgentTemperature(a_eventReceiver);
        }
        catch (std::bad_alloc& ba)
        {
            throw;
        }
        return newSensor;
    }
}

} //experis
