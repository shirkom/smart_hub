#include "mu_test.h"
#include "event_router.hpp"
#include "i_publishable.hpp"
#include "event_receiver.hpp"
#include "sensor_agent_base.hpp"
#include "controller_agent_base.hpp"
#include "registration_manager.hpp"

namespace experis {

////////////////SensorAgentTest////////////////

class SensorAgentTest : public SensorAgentBase
{
public:
    SensorAgentTest(IEventReceiver* a_receiver);
    virtual ~SensorAgentTest(){}
    void SendEvent(SharedPtr<Event> a_sharedPtrEvent); //for test only!!!! PublishEvent(SharedPtr<Event> a_sharedPtrEvent)
};

SensorAgentTest::SensorAgentTest(IEventReceiver* a_receiver) 
: SensorAgentBase(a_receiver)
{
    
}

void SensorAgentTest::SendEvent(SharedPtr<Event> a_sharedPtrEvent) 
{
    SensorAgentBase::SendEvent(a_sharedPtrEvent);
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
    virtual void PublishEvent(SharedPtr<Event> a_sharedPtrEvent);

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

void ControllerAgentTest::PublishEvent(SharedPtr<Event> a_sharedPtrEvent) 
{
    m_didReceivedEvent = true;
}

bool ControllerAgentTest::DidReceivedEvent() {return m_didReceivedEvent;}


///////////////////////TESTS//////////////////////////

BEGIN_TEST(check_SendEvent_flow_one_controller_one_sensor)
    RegistrationManager registrationManager;
    EventRouter eventRouter(registrationManager);
    EventReceiver eventReceiver(eventRouter);

    SensorAgentTest sensor(&eventReceiver);
    ControllerAgentTest controllerTest(&eventRouter, "1","PhilipsHue", "2", "room_1_a");

    Location location("1", "2", "room_1_a");
    Event event("7", "PhilipsHue", "lights", location);
    SharedPtr<Event> eventSharedPtr(new Event(event));

    sensor.SendEvent(eventSharedPtr);
    eventRouter.Shutdown();
    ASSERT_THAT(controllerTest.DidReceivedEvent());
END_TEST


BEGIN_TEST(check_SendEvent_flow_three_controller_one_sensor)
    RegistrationManager registrationManager;
    EventRouter eventRouter(registrationManager);
    EventReceiver eventReceiver(eventRouter);

    ControllerAgentTest controllerTest(&eventRouter, "1","PhilipsHue", "2", "room_1_a");
    ControllerAgentTest controllerTest2(&eventRouter, "1","PhilipsHue", "2", "room_5_a");
    ControllerAgentTest controllerTest3(&eventRouter, "1","PhilipsHue", "", "");


    SensorAgentTest sensor(&eventReceiver);
    Location location("1", "2", "room_1_a");
    Event event("7", "PhilipsHue", "lights", location);
    SharedPtr<Event> eventSharedPtr(new Event(event));

    sensor.SendEvent(eventSharedPtr);

    eventRouter.Shutdown();
    ASSERT_THAT(controllerTest.DidReceivedEvent());
    ASSERT_THAT(controllerTest2.DidReceivedEvent() == false);
    ASSERT_THAT(controllerTest3.DidReceivedEvent());
END_TEST

} //experis

TEST_SUITE(test_flow_msg_test)
    TEST(experis::check_SendEvent_flow_one_controller_one_sensor)
    TEST(experis::check_SendEvent_flow_three_controller_one_sensor)
END_SUITE

