#include "mu_test.h"
#include "hub.hpp"
#include "agent_factory.hpp"
#include "tcp_server_sensor.hpp"


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


BEGIN_TEST(test_hub)
    AgentFactory agentFactory;
    agentFactory.AddCreatorFromSo("TestHVAC", "../hvac_controller/controller_agent_HVAC.so");
    agentFactory.AddCreatorFromSo("TestSprinkler", "../sprinkler_controller/controller_agent_sprinkler.so");
    agentFactory.AddCreatorFromSo("ambient_temp", "../temp_sensor/sensor_agent_temperature.so");
    agentFactory.AddCreatorFromSo("TestFire", "../fire_safety_sensor/sensor_agent_fire_safty.so");
    agentFactory.AddCreator("SensorServerHttp", CreateTcpServerSensor);

    const std::string path = "./text.ini";
    StartUp startUp(agentFactory, path);
    Hub hub(startUp);
    hub.Run();
    sleep(50);
    hub.Shutdown();
    ASSERT_PASS();
END_TEST




} //experis

TEST_SUITE(tests_hub)
    TEST(experis::test_hub)

END_SUITE

