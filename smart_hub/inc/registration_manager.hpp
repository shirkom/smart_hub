#pragma once 

#include <string>
#include <vector>
#include "event.hpp"
#include "location.hpp"
#include "i_publishable.hpp"
#include "common.hpp"
#include "common_exc.hpp"
#include "shared_ptr.hpp"


namespace experis {

DEFINE_EXCEPTION(PublishableIsNull, "Publishable Is Null")

struct DataController {
    IPublishable* m_controller;
    std::string m_type; 
    Location* m_location;
};

class RegistrationManager : private Uncopyable{
public:
    RegistrationManager();
    ~RegistrationManager();

    void RegistrateController(const std::string& a_type, 
                             const std::string& a_building, 
                             const std::string& a_floor, 
                             const std::string& a_room, 
                             IPublishable* a_controller);

    void FindFits(SharedPtr<Event> sharedPtrEvent, std::vector<IPublishable*>& results) const;

private:
   std::vector<DataController*>* m_container;
};






} //experis

