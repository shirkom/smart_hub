
#include <dlfcn.h>
#include <iostream>

#include "agent_factory.hpp"

namespace experis {

IAgent* AgentFactory::ProduceAgent(const DataAgent& a_data, ISubscribable* a_subscribable, IEventReceiver* a_eventReceiver) 
{
    TypeToCreator::iterator it = m_typeToCreator.find(a_data.m_type);
    if(m_typeToCreator.end() == it)
    {
        std::cerr << "CreateFuncNotFound " << a_data.m_type << std::endl;
        throw CreateFuncNotFound();
    }
    return it->second(a_data, a_subscribable, a_eventReceiver);
}


void AgentFactory::AddCreator(std::string a_type, AgentCreator a_agentCreator) 
{
    m_typeToCreator.insert(std::pair<std::string, AgentCreator>(a_type, a_agentCreator));
}

void AgentFactory::AddCreatorFromSo(std::string a_type, std::string a_soPath)
{
    void* so = dlopen(a_soPath.data(), RTLD_LAZY);
    char *error;
    if ((error = dlerror()) != NULL)
    {
        fprintf(stderr, "%s\n", error);
        throw CanNotOpenSOFile();
    }
    void* func = dlsym(so, "Create");
    if ((error = dlerror()) != NULL) 
    {
        fprintf(stderr, "%s\n", error);
        throw CreateFuncNotFound();
    }
    AddCreator(a_type, (AgentCreator)func);
} 

} //experis
