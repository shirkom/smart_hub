#include "hub.hpp"
#include <algorithm>

namespace experis {

class DestroyAgent {
public:
    void operator()(IAgent* a_iAgent);
};

void DestroyAgent::operator()(IAgent* a_iAgent) 
{
    delete a_iAgent;
}


////////////////////////////////////////////////////

void Hub::Run() 
{
    m_startUp.LoadFile(m_eventRouter, m_eventReceiver, m_agents);
}


Hub::Hub(StartUp& a_startUp)
: m_eventRouter()
, m_eventReceiver()
, m_startUp(a_startUp)
, m_agents()
{
    try {
        m_registrationManager = new RegistrationManager;
        m_eventRouter = new EventRouter(*m_registrationManager);
        m_eventReceiver = new EventReceiver(*m_eventRouter);
    }
    catch (std::bad_alloc& ba)
    {
        throw;
    }
}

Hub::~Hub()
{
    std::for_each(m_agents.begin(), m_agents.end(), DestroyAgent());
    delete m_eventReceiver;
    delete m_eventRouter;
    delete m_registrationManager;
}

void Hub::Shutdown() 
{
    m_eventRouter->Shutdown();
}

} //experis

