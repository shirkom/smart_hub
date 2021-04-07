
#include "parser_file.hpp"
#include "event.hpp"
#include <string>
#include <vector>
#include <iostream>

#include <algorithm>

namespace experis {


FileParser::FileParser(const std::string& a_fileName) 
: m_fileName(a_fileName)
{ 
}

FileParser::~FileParser() 
{
}

void FileParser::createDataAgent(std::string a_line, DataAgent* a_dataAgent) const
{
    if(!a_dataAgent)
    {
        throw PtrDataAgentIsNull();
    }

    if(a_line.at(0) == '[')
    {
        a_dataAgent->m_id = a_line.substr(1, a_line.size() - 2);
        return;
    }
    size_t found = a_line.find(" = "); 
    if (found == std::string::npos)
    {
        return; 
    }
    std::string value = a_line.substr(found + 3);
    std::string key = a_line.substr(0, found);

    if(key == "type")
    {
        a_dataAgent->m_type = value;
    }
    else if(key == "room")
    {
        a_dataAgent->m_room = value;
    }
    else if(key == "floor")
    {
        a_dataAgent->m_floor = value;
    }
    else if(key == "log")
    {
        a_dataAgent->m_log = value;
    }
    else if(key == "config")
    {
        a_dataAgent->m_config = value;
    }
}


std::vector<DataAgent> FileParser::Parser() const
{
    std::vector<DataAgent> dataAgents;
    std::vector<std::string> vecOfTokens;
    std::string line = "";
    DataAgent dataAgent;

    std::ifstream m_file(m_fileName.c_str());

    if (!m_file.is_open()) 
    {
        throw CanNotOpenFile();
    }

    while(getline(m_file, line))
    {
        if("" == line)
        {
            dataAgents.push_back(dataAgent);
            dataAgent = DataAgent();
        } 
        else
        {
            createDataAgent(line, &dataAgent);
        }
    }
    dataAgents.push_back(dataAgent);

    m_file.close();
    return dataAgents;
}

} //experis
