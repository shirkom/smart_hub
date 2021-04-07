#pragma once

#include "sensor_agent_base.hpp"
#include "network.hpp"
#include "thread.hpp"
#include "i_subscribable.hpp"
#include "parser_file.hpp"
#include "http_event_unserializer.hpp"


namespace experis {
class TcpServerSensor : public SensorAgentBase  {
public:
    friend class SensorTask;
    explicit TcpServerSensor(IEventReceiver* a_receiver, int a_port);
    virtual ~TcpServerSensor();

private:
    NetworkTask* m_task;
    Network* m_network;
    Thread<Network>* m_threadRunServer;
};

IAgent* CreateTcpServerSensor(const DataAgent& a_data, ISubscribable* a_subscribable, IEventReceiver* a_eventReceiver);

} //experis
