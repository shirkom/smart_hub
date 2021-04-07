#pragma once

#include "event.hpp"
#include "i_subscribable.hpp"
#include "i_publishable.hpp"
#include "i_agent.hpp"
#include "../../inc/common.hpp"
#include "../../inc/common_exc.hpp"

namespace experis {

DEFINE_EXCEPTION(PtrSubscribeableIsNull, "Ptr Subscribeable Is Null")

class ControllerAgentBase : public IPublishable, public IAgent, private Uncopyable 
{
public:
    ControllerAgentBase(ISubscribable* a_subscribable, 
                        const std::string& a_building,
                        const std::string& a_type,
                        const std::string& a_floor,
                        const std::string& a_room);
    virtual ~ControllerAgentBase();

private:

};

} //experis
