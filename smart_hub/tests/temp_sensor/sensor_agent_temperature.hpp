#pragma once
#include <csignal>

#include "sensor_agent_base.hpp"
#include "i_event_receiver.hpp"
#include "i_subscribable.hpp"
#include "parser_file.hpp" // struct DataAgent
#include "thread.hpp"

namespace experis {
class SensorAgentTemperature : public SensorAgentBase
{
public:
    SensorAgentTemperature(IEventReceiver* a_receiver);
    virtual ~SensorAgentTemperature();

// public:
//     static IAgent* Create(const DataAgent& a_data, ISubscribable* a_subscribable, IEventReceiver* a_eventReceiver);

private:
    static void SendTemp(SensorAgentTemperature* sensor);
    Thread<SensorAgentTemperature>* thread;
};

extern "C"
{
    IAgent* Create(const DataAgent& a_data, ISubscribable* a_subscribable, IEventReceiver* a_eventReceiver);
} 

} //experis
