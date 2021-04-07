
#include "registration_manager.hpp"
#include <algorithm>
#include <iostream>
#include "shared_ptr.hpp"
#include "common_exc.hpp"

namespace experis {

DEFINE_EXCEPTION(PtrDataControllerIsNull, "Ptr DataController Is Null")

struct Destroy
{
    void operator()(DataController* a_dataController);
    
};
void Destroy::operator()(DataController* a_dataController) 
{
    if(a_dataController)
    {
        delete a_dataController->m_location; 
        delete a_dataController;
    }
}



struct FindFunc
{
    FindFunc(SharedPtr<Event> a_sharedPtrEvent, std::vector<IPublishable*>& a_results);
    void operator()(DataController* a_dataController);
    
private:
    bool IsFit(DataController* a_dataController);
private:
    SharedPtr<Event> m_event;
    std::vector<IPublishable*>& m_results;
};

FindFunc::FindFunc(SharedPtr<Event> a_sharedPtrEvent, std::vector<IPublishable*>& a_results) 
: m_event(a_sharedPtrEvent)
, m_results(a_results)
{
    
}

void FindFunc::operator()(DataController* a_dataController) 
{
    if(IsFit(a_dataController))
    {
        m_results.push_back(a_dataController->m_controller);
    }
}

bool FindFunc::IsFit(DataController* a_dataController)
{
    if(0 == a_dataController)
    {
        throw PtrDataControllerIsNull();
    }
    bool isTypeFit = m_event->m_type == a_dataController->m_type || "" == a_dataController->m_type;

    // std::cout << "type -" << m_event->m_type << "-" << a_dataController->m_type << isTypeFit << std::endl;

    bool isBuildingFit = m_event->m_location.GetBuilding() == a_dataController->m_location->GetBuilding()
        || "" == a_dataController->m_location->GetBuilding();

    // std::cout << "building" << "-" << m_event->m_location.GetBuilding() << "-" << a_dataController->m_location->GetBuilding() << std::endl;
    
    bool isFloorFit = m_event->m_location.GetFloor() == a_dataController->m_location->GetFloor()
        || "" == a_dataController->m_location->GetFloor();

    // std::cout << "floor" << "-" << m_event->m_location.GetFloor() << "-" << a_dataController->m_location->GetFloor() << std::endl;
    
    bool isRoomFit = m_event->m_location.GetRoom() == a_dataController->m_location->GetRoom()
        || "" == a_dataController->m_location->GetRoom();

    // std::cout << "room" << "-" << m_event->m_location.GetRoom() << "-" << a_dataController->m_location->GetRoom() << std::endl;

    bool a = isTypeFit && isBuildingFit && isFloorFit && isRoomFit;
    // std::cout << "yes or no" << a << std::endl;
    return a; 
}

///////////////////////////////////////////////////////////////

RegistrationManager::RegistrationManager() 
{
    try
    {
        m_container = new std::vector<DataController*>;
    }
    catch (std::bad_alloc& ba) {
        throw;
    }
}

RegistrationManager::~RegistrationManager() 
{
    std::for_each(m_container->begin(), m_container->end(), Destroy());
    delete m_container;
}

void RegistrationManager::RegistrateController(const std::string& a_type, 
                const std::string& a_building,
                const std::string& a_floor, 
                const std::string& a_room, 
                IPublishable* a_controller) 
{
    if(0 == a_controller)
    {
        throw PublishableIsNull();
    }
    DataController* dataController = 0;
    Location* location = 0;
    try {
        dataController = new DataController;
        location = new Location(a_building, a_floor, a_room);
    }
    catch (std::bad_alloc& ba) {
        throw;
    }
    dataController->m_location = location;
    dataController->m_type = a_type;
    dataController->m_controller = a_controller;
    m_container->push_back(dataController);
}

void RegistrationManager::FindFits(SharedPtr<Event> a_sharedPtrEvent, std::vector<IPublishable*>& results) const
{
	std::for_each(m_container->begin(), m_container->end(), FindFunc(a_sharedPtrEvent, results));
}

} //experis
