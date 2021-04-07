
#include "mu_test.h"

#include "event_router.hpp"

#include "i_publishable.hpp"

namespace experis {

class EmptyPublishable : public IPublishable
{
public:
    explicit EmptyPublishable(const std::string& a_name);
    virtual void PublishEvent(SharedPtr<Event> a_sharedPtrEvent) {   }
private:
    const std::string& m_name;
};

EmptyPublishable::EmptyPublishable(const std::string& a_name) 
: m_name(a_name){
    
}

////////////////////////////////

class RegToTypePublishable : public IPublishable
{
public:
    explicit RegToTypePublishable(const std::string& a_type);
    virtual void PublishEvent(SharedPtr<Event> a_sharedPtrEvent) {   }
private:
    const std::string& m_type;
};

RegToTypePublishable::RegToTypePublishable(const std::string& a_type) 
: m_type(a_type){
    
}

/////////////////////////////////

class RegToTypeAndFloorPublishable : public IPublishable
{
public:
    explicit RegToTypeAndFloorPublishable(const std::string& a_type, const std::string& a_floor);
    virtual void PublishEvent(SharedPtr<Event> a_sharedPtrEvent);
private:
    const std::string& m_type;
    const std::string& m_room;
};

RegToTypeAndFloorPublishable::RegToTypeAndFloorPublishable(const std::string& a_type, const std::string& a_room) 
: m_type(a_type)
, m_room(a_room) 
{
    
}

void RegToTypeAndFloorPublishable::PublishEvent(SharedPtr<Event> a_sharedPtrEvent) 
{
    
}


/////////////////////////////////

BEGIN_TEST(check_Subscribe)
    RegistrationManager rm;
    EventRouter er(rm);
    EmptyPublishable emptyPublishable("1");
    er.Subscribe("PhilipsHue","1", "room_1_a", "2", &emptyPublishable);
    er.Shutdown();
    ASSERT_PASS();
END_TEST

BEGIN_TEST(check_SendEvent_all_fit)
    RegistrationManager rm;
    EventRouter er(rm);
    EmptyPublishable emptyPublishable("1");
    RegToTypePublishable regToTypePublishable("PhilipsHue");
    RegToTypeAndFloorPublishable regToTypeAndFloorPublishable("PhilipsHue", "room_16_a");

    er.Subscribe("PhilipsHue","1", "room_1_a", "2", &emptyPublishable);
    er.Subscribe("PhilipsHue", "", "", "", &regToTypePublishable);
    er.Subscribe("","", "room_1_a", "", &regToTypeAndFloorPublishable);

    Location location("1", "room_1_a", "2");
    Event event("7", "PhilipsHue", "lights", location);
    SharedPtr<Event> eventSharedPtr(new Event(event));

   // SharedPtr<WorkBase> sharedPtrPublishEventWork(new PublishEventWork(m_event, a_publishable));

    ASSERT_EQUAL(er.SendEvent(eventSharedPtr), 3);
    er.Shutdown();
END_TEST

BEGIN_TEST(check_SendEvent_two_fit)
    RegistrationManager rm;
    EventRouter er(rm);
    EmptyPublishable emptyPublishable("1");
    RegToTypePublishable regToTypePublishable("PhilipsHue");
    RegToTypeAndFloorPublishable regToTypeAndFloorPublishable("PhilipsHue", "room_16_a");

    er.Subscribe("PhilipsHue","1", "room_1_a", "2", &emptyPublishable);
    er.Subscribe("PhilipsHue", "", "", "", &regToTypePublishable);
    er.Subscribe("","", "room_151_a", "", &regToTypeAndFloorPublishable);

    Location location("1", "room_1_a", "2");
    Event event("7", "PhilipsHue", "lights", location);
    SharedPtr<Event> eventSharedPtr(new Event(event));

    ASSERT_EQUAL(er.SendEvent(eventSharedPtr), 2);
    er.Shutdown();
END_TEST


} //experis

TEST_SUITE(test_event_router)
    TEST(experis::check_Subscribe)
    TEST(experis::check_SendEvent_all_fit)
    TEST(experis::check_SendEvent_two_fit)
END_SUITE

