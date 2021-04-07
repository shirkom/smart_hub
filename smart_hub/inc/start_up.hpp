#pragma once

#include <vector>
#include <string>

#include "i_agent.hpp"
#include "event_receiver.hpp"
#include "event_router.hpp"
#include "parser_file.hpp"
#include "agent_factory.hpp"
#include "common.hpp"
#include "common_exc.hpp"

namespace experis {

DEFINE_EXCEPTION(EventReceiverIsNull, "Event Receiver Is Null")
DEFINE_EXCEPTION(EventRouterIsNull, "Event RouterIs Null")

class StartUp : private Uncopyable {
public:

void LoadFile(EventRouter* a_eventRouter,
                EventReceiver* a_eventReceiver,
                std::vector<IAgent*>& a_agents) const;
    StartUp(AgentFactory& a_agentFactory, const std::string& a_iniFilePath);
    ~StartUp();

private:
    const FileParser *m_parser; 
    AgentFactory& m_agentFactory;
};





} //experis