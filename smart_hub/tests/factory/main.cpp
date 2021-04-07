#include "mu_test.h"
#include "hub.hpp"
#include "agent_factory.hpp"
#include "controller_agent_HVAC.hpp"


namespace experis {


class EmptySubscribable: public ISubscribable
{
public:
   virtual void Subscribe(
                const std::string& a_type, 
                const std::string& a_building,
                const std::string& a_floor, 
                const std::string& a_room, 
                IPublishable* a_controller) { }
};

class EmptyEventReceiver : public IEventReceiver {
public:
    virtual void Receive(SharedPtr<Event> a_sharedPtrEvent){ }
};

////////////////////////////////////////


BEGIN_TEST(test_factory)
    AgentFactory agentFactory;
    agentFactory.AddCreatorFromSo("TestHVAC", "../hvac_controller/controller_agent_HVAC.so");
    //agentFactory.AddCreator("TestHVAC", Create);
    EmptySubscribable emptySubscribable;
    EmptyEventReceiver emptyEventReceiver;
    DataAgent dataAgent = {"545", "TestHVAC", "room_1_a", "1","hvac_log",
                         "iot:10.10.1.64; tmp:77; shutdown: Fire_Detected|ROOM_EMPTY"};
    IPublishable *iagent = dynamic_cast<IPublishable*>(agentFactory.ProduceAgent(dataAgent, &emptySubscribable, &emptyEventReceiver));

    Location location("1", "2", "room_1_a");
    Event event("7", "PhilipsHue", "lights", location);
    SharedPtr<Event> eventSharedPtr(new Event(event));

    iagent->PublishEvent(eventSharedPtr);

    ASSERT_THAT(iagent != 0);
    delete iagent;
END_TEST




} //experis

TEST_SUITE(tests_factory)
    TEST(experis::test_factory)

END_SUITE

