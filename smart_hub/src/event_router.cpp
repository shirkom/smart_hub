
#include <vector>
#include <algorithm>

#include "event_router.hpp"

namespace experis {

const size_t EventRouter::m_numOfThreads = 16;

///////////////////////////////////////////////

class PublishEventWork : public WorkBase {
public:
    PublishEventWork(SharedPtr<Event> a_sharedPtrEvent, IPublishable* a_publishable);
    ~PublishEventWork() {}
    void operator()() const; 
private:
    SharedPtr<Event> m_sharedPtrEvent;
    IPublishable* m_publishable;
};

PublishEventWork::PublishEventWork(SharedPtr<Event> a_sharedPtrEvent, IPublishable* a_publishable) 
: m_sharedPtrEvent(a_sharedPtrEvent)
, m_publishable(a_publishable)
{
    if(a_publishable)
    {
        m_publishable = a_publishable;
    }
    else
    {
        throw PtrIPublishableIsNull();
    }
}

void PublishEventWork::operator()() const
{
    m_publishable->PublishEvent(m_sharedPtrEvent);
}

//////////////////////////////////////////////////////////

class PublishUnaryFunc {
public:
    PublishUnaryFunc(SharedPtr<Event> a_sharedPtrEvent, ThreadPool& a_threadPool);
    void operator()(IPublishable*);

private:
    SharedPtr<Event> m_event;
    ThreadPool& m_threadPool;
};

PublishUnaryFunc::PublishUnaryFunc(SharedPtr<Event> a_sharedPtrEvent, ThreadPool& a_threadPool) 
: m_event(a_sharedPtrEvent)
, m_threadPool(a_threadPool)
{
    
}

void PublishUnaryFunc::operator()(IPublishable* a_publishable) 
{
    if(a_publishable)
    {
        SharedPtr<WorkBase> sharedPtrPublishEventWork(new PublishEventWork(m_event, a_publishable));
        m_threadPool.Submit(sharedPtrPublishEventWork);
    }
    else
    {
        throw PtrIPublishableIsNull();
    }
}

////////////////////////////////////////////////////////////////////////

EventRouter::EventRouter(RegistrationManager& a_registarationManager) 
: m_registarationManager(a_registarationManager)
, m_threadPool(m_numOfThreads)
{
    
}

void EventRouter::Subscribe(const std::string& a_type, 
                const std::string& a_building,
                const std::string& a_floor, 
                const std::string& a_room, 
                IPublishable* a_controller) 
{
    if(a_controller)
    {
        m_registarationManager.RegistrateController(a_type, a_building, a_floor, a_room, a_controller);
    }
    else
    {
        throw PtrIPublishableIsNull();
    } 
}


size_t EventRouter::SendEvent(SharedPtr<Event> a_sharedPtrEvent) 
{
    std::vector<IPublishable*> fits;
    m_registarationManager.FindFits(a_sharedPtrEvent, fits);

    std::for_each(fits.begin(), fits.end(), PublishUnaryFunc(a_sharedPtrEvent, m_threadPool));
    return fits.size();
}

void EventRouter::Shutdown() 
{
    m_threadPool.Shutdown();
}

EventRouter::~EventRouter() 
{

}

} //experis
