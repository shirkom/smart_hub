// extern "C" {
// #include "ServerNet.h"
// }
#include "mu_test.h"

#include "tcp_server_controller.hpp"
#include "i_subscribable.hpp"
#include <unistd.h>

namespace experis {

class EmptySubscribable : public ISubscribable {
public:
    virtual ~EmptySubscribable() {}
    virtual void Subscribe(
                const std::string& a_type, 
                const std::string& a_building,
                const std::string& a_floor, 
                const std::string& a_room, 
                IPublishable* a_controller) {}
   

};
/////////////////////////
/*for this test port 5555*/

BEGIN_TEST(check_send)

    EmptySubscribable emptySub;
    TcpServerController tcpController(&emptySub, 
                                    "building-1",
                                    "type-ShirHus",
                                    "floor3",
                                    "room_a1_",
                                    5555);
    std::cout << "start sleeping" << std::endl;
    sleep(5);

    Location location("1", "2", "room_1_a");
    Event event("7", "PhilipsHue", "loglights", location);
    SharedPtr<Event> eventSharedPtr(new Event(event));
    tcpController.PublishEvent(eventSharedPtr);

    ASSERT_PASS();
END_TEST



} //experis

TEST_SUITE(test_server_controller)
    TEST(experis::check_send)
   
END_SUITE

