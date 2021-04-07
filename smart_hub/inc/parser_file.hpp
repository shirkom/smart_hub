#pragma once
#include <string>
#include <fstream>
#include <vector>
#include "../../inc/common_exc.hpp"
#include "../../inc/common.hpp"



namespace experis {

struct DataAgent {
    std::string m_id; 
    std::string m_type;
    std::string m_room;
    std::string m_floor;
    std::string m_log;
    std::string m_config;
};

DEFINE_EXCEPTION(CanNotOpenFile, "Can't Open File")
DEFINE_EXCEPTION(NotFound, "Not Found")
DEFINE_EXCEPTION(PtrDataAgentIsNull, "PtrData Agent Is Null")



class FileParser : private Uncopyable {
public:
    FileParser(const std::string& a_fileName);
    ~FileParser();
    std::vector<DataAgent> Parser() const;

private:
    void createDataAgent(std::string a_line, DataAgent* a_dataAgent) const;

private:
    const std::string& m_fileName;
    
};

} //experis

