#include "http_event_unserializer.hpp"

namespace experis {
SharedPtr<Event> HttpEventUnserializer::Unserialize(const std::string a_msg) 
{
    std::vector<std::string> lines = Split(a_msg, "\n");
    std::vector<std::string> words = Split(lines[0], " ");
    std::vector<std::string> parts = Split(words[1], "/");

    if(6 != parts.size())
    {
        throw WrongArgs();
    }
    Location location(parts[0], parts[1], parts[2]);
    Event event(parts[3], parts[4], parts[5], location);
    
    SharedPtr<Event> eventSharedPtr(new Event(event));
    return eventSharedPtr;
}


std::vector<std::string> HttpEventUnserializer::Split(const std::string& a_str, std::string a_delimiter) {
    std::vector<std::string> tokens;
    size_t prev = 0, pos = 0;
     while (pos < a_str.length() && prev < a_str.length())  
     {
        pos = a_str.find(a_delimiter, prev);
        if (pos == std::string::npos) 
        {
            pos = a_str.length();
        }
        std::string token = a_str.substr(prev, pos - prev);
        if (!token.empty())
        {
            tokens.push_back(token);
        } 
        
        prev = pos + a_delimiter.length();
    } 
    return tokens;
}

} //experis