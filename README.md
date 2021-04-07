# Smart Hub 
SmartHub is a next generation platform for managing IoT enhanced smart
building. The platform serves as the controller and integration hub for modules,
devices, controllers and sensors that will enable efficient and comfortable
operation of the building.

# Add your sensor or controller
To add a sensor you should write a class that inherit from `SensorAgentBase` and use the protested function `SendEvent` whenever you want to send event.
To add a controller you should write a class that inherit from `ControllerAgentBase` and implement `PublishEvent`.
Anyway you must include a function named `Create` that creates an instance of your `IAgent`, with the following signature:
```c++
extern "C"
{
    IAgent* Create(const DataAgent& a_data, ISubscribable* a_subscribable, IEventReceiver* a_eventReceiver);
}
```

Here's an example of how to use the so file (taken from `test_hub`):

```c++

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
```

this test includes TCP sensor which you can activate by visiting [http://localhost:5555/1/1/room_1_a/7/TestHVAC/lights](http://localhost:5555/1/1/room_1_a/7/TestHVAC/lights)
you can see that there is new print `Air Condition is On` of the HVAC controller every time you load this URL.

The URL structure:
```
http://localhost:5555/<building>/<floor>/<room>/<timestamp>/<type>/<log>
```

# Using TCP Sensor

`TcpServerSensor` is a base controller for creating a TCP sensor and also can be used for testing.
For running the test please run the following
```sh
$ make check
```
Then quickly visit [http://localhost:5555/1/1/room_1_a/7/TestHVAC/lights](http://localhost:5555/1/1/room_1_a/7/TestHVAC/lights)



# Using TCP Controller

`TcpServerController` is a base controller for creating a TCP controller, and also can be used for testing.
For running the test please run the following
```sh
$ make check
```

Then quickly go to [http://localhost:5555](http://localhost:5555/)
