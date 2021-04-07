#include "mu_test.h"
#include "registration_manager.hpp"
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
    virtual void PublishEvent(SharedPtr<Event> a_sharedPtrEvent) {   }
private:
    const std::string& m_type;
    const std::string& m_room;
};

RegToTypeAndFloorPublishable::RegToTypeAndFloorPublishable(const std::string& a_type, const std::string& a_room) 
: m_type(a_type)
, m_room(a_room) 
{
    
}

/////////////////////////////////

BEGIN_TEST(check_registrateController)
    RegistrationManager rm;
    EmptyPublishable emptyPublishable("1");
    rm.RegistrateController("PhilipsHue","1", "2", "room_1_a",&emptyPublishable);
    ASSERT_PASS();
END_TEST

BEGIN_TEST(check_FindFits_one_sub)
    RegistrationManager rm;
    EmptyPublishable emptyPublishable("1");
    rm.RegistrateController("PhilipsHue","1", "2", "room_1_a",&emptyPublishable);
    
    Location location("1", "2", "room_1_a");
    Event event("7", "PhilipsHue", "lights", location);
    SharedPtr<Event> eventSharedPtr(new Event(event));
    std::vector<IPublishable*> results;

    rm.FindFits(eventSharedPtr, results);
    ASSERT_EQUAL(results[0], &emptyPublishable);
END_TEST

BEGIN_TEST(check_FindFits_3_sub_all_send)
    RegistrationManager rm;
    EmptyPublishable emptyPublishable("1");
    RegToTypePublishable regToTypePublishable("PhilipsHue");
    RegToTypeAndFloorPublishable regToTypeAndFloorPublishable("PhilipsHue", "room_1_a");
    rm.RegistrateController("PhilipsHue","1", "2", "room_1_a",&emptyPublishable);
    rm.RegistrateController("PhilipsHue", "", "", "", &regToTypePublishable);
    rm.RegistrateController("PhilipsHue","", "", "room_1_a", &regToTypeAndFloorPublishable);
    
    Location location("1", "2", "room_1_a");
    Event event("7", "PhilipsHue", "lights", location);
    SharedPtr<Event> eventSharedPtr(new Event(event));
    std::vector<IPublishable*> results;

    rm.FindFits(eventSharedPtr, results);
    ASSERT_EQUAL(results[0], &emptyPublishable);
    ASSERT_EQUAL(results[1], &regToTypePublishable);
    ASSERT_EQUAL(results[2], &regToTypeAndFloorPublishable);
END_TEST

BEGIN_TEST(check_FindFits_only_1fit)
    RegistrationManager rm;
    EmptyPublishable emptyPublishable("1");
    RegToTypePublishable regToTypePublishable("PhilipsHue");
    RegToTypeAndFloorPublishable regToTypeAndFloorPublishable("PhilipsHue", "room_16_a");

    rm.RegistrateController("ShirHue", "2", "2", "room_99_a", &emptyPublishable);
    rm.RegistrateController("PhilipsHue", "", "", "", &regToTypePublishable);
    rm.RegistrateController("PhilipsHue","", "", "room_16_a", &regToTypeAndFloorPublishable);
    
    Location location("1", "2", "room_1_a");
    Event event("7", "PhilipsHue", "lights", location);
    SharedPtr<Event> eventSharedPtr(new Event(event));
    std::vector<IPublishable*> results;

    rm.FindFits(eventSharedPtr, results);
    ASSERT_EQUAL(results.size(), 1);
    ASSERT_EQUAL(results[0], &regToTypePublishable);
END_TEST


BEGIN_TEST(check_FindFits_2fit)
    RegistrationManager rm;
    EmptyPublishable emptyPublishable("1");
    RegToTypePublishable regToTypePublishable("PhilipsHue");
    RegToTypeAndFloorPublishable regToTypeAndFloorPublishable("PhilipsHue", "room_16_a");

    rm.RegistrateController("", "", "2", "room_1_a", &emptyPublishable);
    rm.RegistrateController("PhilipsHue", "", "", "", &regToTypePublishable);
    rm.RegistrateController("","", "", "room_1_a",&regToTypeAndFloorPublishable);
    
    Location location("1", "2", "room_1_a");
    Event event("7", "ShirHue", "lights", location);
    SharedPtr<Event> eventSharedPtr(new Event(event));
    std::vector<IPublishable*> results;

    rm.FindFits(eventSharedPtr, results);
    ASSERT_EQUAL(results.size(), 2);
    ASSERT_EQUAL(results[0], &emptyPublishable);
    ASSERT_EQUAL(results[1], &regToTypeAndFloorPublishable);
END_TEST

BEGIN_TEST(check_FindFits_not_fit)
    RegistrationManager rm;
    EmptyPublishable emptyPublishable("1");
    RegToTypePublishable regToTypePublishable("PhilipsHue");
    RegToTypeAndFloorPublishable regToTypeAndFloorPublishable("PhilipsHue", "room_16_a");

    rm.RegistrateController("ShirHue", "2", "2", "room_99_a", &emptyPublishable);
    rm.RegistrateController("PhilipsHue", "6", "", "", &regToTypePublishable);
    rm.RegistrateController("","", "", "room_16_a", &regToTypeAndFloorPublishable);
    
    Location location("1", "2", "room_1_a");
    Event event("7", "PhilipsHue", "lights", location);
    SharedPtr<Event> eventSharedPtr(new Event(event));
    std::vector<IPublishable*> results;

    rm.FindFits(eventSharedPtr, results);
    ASSERT_EQUAL(results.size(), 0);
END_TEST



BEGIN_TEST(check_FindFits_1fit_specific_type_room_floor)
    RegistrationManager rm;
    EmptyPublishable emptyPublishable("1");
    RegToTypePublishable regToTypePublishable("PhilipsHue");
    RegToTypeAndFloorPublishable regToTypeAndFloorPublishable("PhilipsHue", "room_16_a");

    rm.RegistrateController( "ShirHue", "1", "2", "room_1_a", &emptyPublishable);
    rm.RegistrateController("PhilipsHue", "", "", "", &regToTypePublishable);
    rm.RegistrateController("PhilipsHue","", "", "room_1_a",&regToTypeAndFloorPublishable);
    
    Location location("1", "2", "room_1_a");
    Event event("7", "ShirHue", "lights", location);
    SharedPtr<Event> eventSharedPtr(new Event(event));
    std::vector<IPublishable*> results;

    rm.FindFits(eventSharedPtr, results);
    ASSERT_EQUAL(results.size(), 1);
    ASSERT_EQUAL(results[0], &emptyPublishable);
END_TEST

BEGIN_TEST(check_FindFits_1fit_specific_type_floor)
    RegistrationManager rm;
    EmptyPublishable emptyPublishable("1");
    RegToTypePublishable regToTypePublishable("PhilipsHue");
    RegToTypeAndFloorPublishable regToTypeAndFloorPublishable("PhilipsHue", "room_16_a");

    rm.RegistrateController( "ShirHue", "", "", "room_1_a", &emptyPublishable);
    rm.RegistrateController("PhilipsHue", "", "", "", &regToTypePublishable);
    rm.RegistrateController("PhilipsHue","", "", "room_1_a",&regToTypeAndFloorPublishable);
    
    Location location("1", "2", "room_1_a");
    Event event("7", "ShirHue", "lights", location);
    SharedPtr<Event> eventSharedPtr(new Event(event));
    std::vector<IPublishable*> results;

    rm.FindFits(eventSharedPtr, results);
    ASSERT_EQUAL(results.size(), 1);
    ASSERT_EQUAL(results[0], &emptyPublishable);
END_TEST

BEGIN_TEST(check_FindFits_1fit_specific_type)
    RegistrationManager rm;
    EmptyPublishable emptyPublishable("1");
    RegToTypePublishable regToTypePublishable("PhilipsHue");
    RegToTypeAndFloorPublishable regToTypeAndFloorPublishable("PhilipsHue", "room_16_a");

    rm.RegistrateController( "ShirHue", "", "", "", &emptyPublishable);
    rm.RegistrateController("PhilipsHue", "", "", "", &regToTypePublishable);
    rm.RegistrateController("PhilipsHue","", "", "room_1_a",&regToTypeAndFloorPublishable);
    
    Location location("1", "2", "room_1_a");
    Event event("7", "ShirHue", "lights", location);
    SharedPtr<Event> eventSharedPtr(new Event(event));
    std::vector<IPublishable*> results;

    rm.FindFits(eventSharedPtr, results);
    ASSERT_EQUAL(results.size(), 1);
    ASSERT_EQUAL(results[0], &emptyPublishable);
END_TEST

BEGIN_TEST(check_FindFits_1fit_sub_to_any)
    RegistrationManager rm;
    EmptyPublishable emptyPublishable("1");
    RegToTypePublishable regToTypePublishable("PhilipsHue");
    RegToTypeAndFloorPublishable regToTypeAndFloorPublishable("PhilipsHue", "room_16_a");

    rm.RegistrateController( "", "", "", "", &emptyPublishable);
    
    Location location("1", "2", "room_1_a");
    Event event("7", "ShirHue", "lights", location);
    SharedPtr<Event> eventSharedPtr(new Event(event));
    std::vector<IPublishable*> results;

    rm.FindFits(eventSharedPtr, results);
    ASSERT_EQUAL(results.size(), 1);
    ASSERT_EQUAL(results[0], &emptyPublishable);
END_TEST

} //experis

TEST_SUITE(test_controller)
    TEST(experis::check_registrateController)
    TEST(experis::check_FindFits_one_sub)
    TEST(experis::check_FindFits_3_sub_all_send)
    TEST(experis::check_FindFits_only_1fit)
    TEST(experis::check_FindFits_2fit)
    TEST(experis::check_FindFits_not_fit)

    TEST(experis::check_FindFits_1fit_specific_type_room_floor)
    TEST(experis::check_FindFits_1fit_specific_type_floor)
    TEST(experis::check_FindFits_1fit_specific_type)
    TEST(experis::check_FindFits_1fit_sub_to_any)
END_SUITE

