#include <algorithm>
#include <string>

#include "start_up.hpp"
#include "sensor_agent_base.hpp"
#include "controller_agent_base.hpp"
#include "agent_factory.hpp"
#include "common_exc.hpp"


namespace experis {


class AgentDataToAgentPtr {
public:
    explicit AgentDataToAgentPtr(AgentFactory& a_agentFactory, ISubscribable* a_subscribable,
                                 IEventReceiver* a_eventReceiver);
    IAgent* operator()(DataAgent a_dataAgent);
private:
    AgentFactory& m_agentFactory;
    ISubscribable* m_subscribable;
    IEventReceiver* m_eventReceiver;
};

DEFINE_EXCEPTION(NullPtr, "Send Null Ptr")

AgentDataToAgentPtr::AgentDataToAgentPtr(AgentFactory& a_agentFactory, ISubscribable* a_subscribable,
                                        IEventReceiver* a_eventReceiver)
: m_agentFactory(a_agentFactory)
{
    if(0 == a_subscribable || 0 == a_eventReceiver)
    {
        throw NullPtr();
    }
    m_subscribable = a_subscribable;
    m_eventReceiver = a_eventReceiver;
}

IAgent* AgentDataToAgentPtr::operator()(DataAgent a_dataAgent) 
{
    return m_agentFactory.ProduceAgent(a_dataAgent, m_subscribable, m_eventReceiver);
}


////////////////SensorAgentTest////////////////

class SensorAgentTest : public SensorAgentBase
{
public:
    SensorAgentTest(IEventReceiver* a_receiver);
    virtual ~SensorAgentTest(){}
    void SendEvent(SharedPtr<Event> a_sharedPtrEvent);
};

SensorAgentTest::SensorAgentTest(IEventReceiver* a_receiver) 
: SensorAgentBase(a_receiver)
{
    
}

void SensorAgentTest::SendEvent(SharedPtr<Event> a_sharedPtrEvent) 
{
    SensorAgentBase::SendEvent( a_sharedPtrEvent);
}

////////////////ControllerAgentTest////////////////

class ControllerAgentTest : public ControllerAgentBase
{
public:
    ControllerAgentTest(ISubscribable* a_subscribable, 
                        const std::string& a_building,
                        const std::string& a_type,
                        const std::string& a_floor,
                        const std::string& a_room);
    virtual ~ControllerAgentTest(){}
    virtual void PublishEvent(const Event& a_event);

    bool DidReceivedEvent();

private:
    bool m_didReceivedEvent;

};

ControllerAgentTest::ControllerAgentTest(ISubscribable* a_subscribable, 
                        const std::string& a_building,
                        const std::string& a_type,
                        const std::string& a_floor,
                        const std::string& a_room) 
: ControllerAgentBase(a_subscribable, a_type, a_building, a_floor, a_room)
, m_didReceivedEvent(false)
{
    
}

void ControllerAgentTest::PublishEvent(const Event& a_event) 
{
    m_didReceivedEvent = true;
}

bool ControllerAgentTest::DidReceivedEvent() {return m_didReceivedEvent;}

//////////////////////////////////////////////////////////////////////////////////////

StartUp::StartUp(AgentFactory& a_agentFactory, const std::string &a_iniFilePath) 
: m_parser(new FileParser(a_iniFilePath))
, m_agentFactory(a_agentFactory)
{
    
}

StartUp::~StartUp() 
{
    delete m_parser;
}

void StartUp::LoadFile(EventRouter* a_eventRouter,
                    EventReceiver* a_eventReceiver,
                    std::vector<IAgent*>& a_agents) const
{
    if(0 == a_eventRouter)
    {
        throw EventRouterIsNull();
    }
    if(0 == a_eventReceiver)
    {
        throw EventReceiverIsNull();
    }
    std::vector<DataAgent> dataAgents = m_parser->Parser();

    AgentDataToAgentPtr a = AgentDataToAgentPtr(m_agentFactory, a_eventRouter, a_eventReceiver);

    std::transform(dataAgents.begin(), dataAgents.end(), std::back_inserter(a_agents), a);
}

} //experis
