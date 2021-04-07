#pragma once
#include <csignal>

#include "sensor_agent_base.hpp"
#include "i_event_receiver.hpp"
#include "i_subscribable.hpp"
#include "parser_file.hpp" // struct DataAgent
#include "thread.hpp"


namespace experis {

class SensorAgentFireSafety : public SensorAgentBase
{
public:
    SensorAgentFireSafety(IEventReceiver* a_receiver);
    virtual ~SensorAgentFireSafety();

private:
    static void SendTemp(SensorAgentFireSafety* sensor);
    Thread<SensorAgentFireSafety>* thread;
};

extern "C"
{
    IAgent* Create(const DataAgent& a_data, ISubscribable* a_subscribable, IEventReceiver* a_eventReceiver);
} 

} //experis
