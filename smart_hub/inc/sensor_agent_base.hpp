#pragma once

#include "event.hpp"
#include "i_event_receiver.hpp"
#include "i_agent.hpp"
#include "common.hpp"
#include "shared_ptr.hpp"

namespace experis {
class SensorAgentBase : public IAgent, private Uncopyable {
public:
    explicit SensorAgentBase(IEventReceiver* a_receiver);
    virtual ~SensorAgentBase();

protected:
    void SendEvent(SharedPtr<Event> a_sharedPtrEvent);

private:
    IEventReceiver* m_receiver;
    
};




} //experis
