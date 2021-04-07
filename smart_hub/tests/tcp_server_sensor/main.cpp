#include "mu_test.h"

#include "tcp_server_sensor.hpp"
#include "i_event_receiver.hpp"

#include <unistd.h>

namespace experis {

class EmptyEventReceiver : public IEventReceiver {
public:
    virtual ~EmptyEventReceiver() {}
    virtual void Receive(SharedPtr<Event> a_sharedPtrEvent);

};

void EmptyEventReceiver::Receive(SharedPtr<Event> a_sharedPtrEvent) 
{
    std::cout << "I got Event" << std::endl;
}



/////////////////////////

BEGIN_TEST(check_receive)

    EmptyEventReceiver e;
    TcpServerSensor tcpSensor(&e, 5555);
    std::cout << "start sleeping" << std::endl;
    sleep(5);

    ASSERT_PASS();
END_TEST



} //experis

TEST_SUITE(test_server_sensor)
    TEST(experis::check_receive)
   
END_SUITE

