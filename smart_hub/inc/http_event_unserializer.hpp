#pragma once

#include <string>
#include <vector>
#include "shared_ptr.hpp"
#include "event.hpp"
#include "common_exc.hpp"

namespace experis {
class HttpEventUnserializer
{
public:
    SharedPtr<Event> Unserialize(const std::string a_msg);
    
private: 
    std::vector<std::string> Split(const std::string& a_str, std::string a_delimiter);
};

DEFINE_EXCEPTION(WrongArgs, "WrongArgs")

} //experis
