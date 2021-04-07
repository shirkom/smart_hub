#pragma once 

#include <string>
#include <vector>
#include "event.hpp"
#include "i_subscribable.hpp"
#include "i_publishable.hpp"
#include "registration_manager.hpp"
#include "thread_pool.hpp"
#include "common_exc.hpp"

namespace experis {

DEFINE_EXCEPTION(PtrIPublishableIsNull, "Ptr IPublishable IsN ull")



class EventRouter : public ISubscribable, private Uncopyable {
public:
    explicit EventRouter(RegistrationManager& a_registarationManager);
    ~EventRouter();
    void Subscribe(const std::string& a_type, 
                const std::string& a_building,
                const std::string& a_floor, 
                const std::string& a_room, 
                IPublishable* a_controller);

    virtual size_t SendEvent(SharedPtr<Event> a_sharedPtrEvent);
    void Shutdown();

private:
    static const size_t m_numOfThreads;

private:
    RegistrationManager& m_registarationManager;
    ThreadPool m_threadPool;
};




} //experis
