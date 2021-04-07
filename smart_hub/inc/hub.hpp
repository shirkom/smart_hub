#pragma once

#include "registration_manager.hpp"
#include "event_router.hpp"
#include "event_receiver.hpp"
#include "i_event_receiver.hpp"
#include "start_up.hpp"
#include "common.hpp"

namespace experis {

class Hub {
public:
    void Run();
    explicit Hub(StartUp& a_startUp);
    ~Hub();
    void Shutdown();
private:
    RegistrationManager* m_registrationManager;
    EventRouter* m_eventRouter;
    EventReceiver* m_eventReceiver;

    StartUp& m_startUp;
    std::vector<IAgent*> m_agents;
};



} //experis