#pragma once

#include "controller_agent_base.hpp"
#include "network.hpp"
#include "thread.hpp"
#include "mutex.hpp"
#include "event_marshal.hpp"

namespace experis {
class TcpServerController : public ControllerAgentBase  {
public:
    friend class ControllerTask;
    TcpServerController(ISubscribable* a_subscribable, 
                        const std::string& a_building,
                        const std::string& a_type,
                        const std::string& a_floor,
                        const std::string& a_room,
                        int a_port);
    virtual ~TcpServerController();
    virtual void PublishEvent(SharedPtr<Event> a_sharedPtrEvent);

private:
    NetworkTask* m_task;
    Network* m_network;
    bool m_isConnect;
    Connection m_connection;
    Thread<Network>* m_threadRunServer;
    Mutex m_mutex;

    EventMarshal* m_marselEvent; 
};


} //experis
