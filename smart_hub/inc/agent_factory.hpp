#pragma once

#include <map>

#include "i_agent.hpp"
#include "parser_file.hpp"
#include "i_event_receiver.hpp"
#include "i_subscribable.hpp"
#include "common_exc.hpp"

namespace experis {

DEFINE_EXCEPTION(CreateFuncNotFound, "CreateFunc Not Found")
DEFINE_EXCEPTION(CanNotOpenSOFile, "Can Not Open SO File")

class AgentFactory
{
public:
    typedef IAgent* (*AgentCreator)(const DataAgent& a_data, ISubscribable* a_subscribable, IEventReceiver* a_eventReceiver);
    typedef std::map<std::string, AgentCreator> TypeToCreator;
    
    IAgent* ProduceAgent(const DataAgent& a_data, ISubscribable* a_subscribable, IEventReceiver* a_eventReceiver);
    void AddCreator(std::string a_type, AgentCreator a_agentCreator);
    void AddCreatorFromSo(std::string a_type, std::string a_soPath);

private:
    TypeToCreator m_typeToCreator;

};


} //experis

