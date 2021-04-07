
#include "../../../inc/common_exc.hpp"
#include "../../inc/i_agent.hpp"
#include "../../inc/controller_agent_base.hpp"

#include "../../inc/parser_file.hpp"
#include "../../inc/i_event_receiver.hpp"
#include "../../../inc/shared_ptr.hpp"

namespace experis
{
class ControllerAgentSprinkler : public ControllerAgentBase
{
public:
    ControllerAgentSprinkler(ISubscribable* a_subscribable, 
                        const std::string& a_building,
                        const std::string& a_type,
                        const std::string& a_floor,
                        const std::string& a_room);
    virtual ~ControllerAgentSprinkler(){}
    virtual void PublishEvent(SharedPtr<Event> a_sharedPtrEvent);
    
};

extern "C"
{
    IAgent* Create(const DataAgent& a_data, ISubscribable* a_subscribable, IEventReceiver* a_eventReceiver);
}
}//namespace experis

