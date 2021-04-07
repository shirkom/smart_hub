#ifndef LOGGER_H_
#define LOGGER_H_
#include <string>

#include "singleton.hpp"
#include  "mutex.hpp"

namespace experis {
class Logger {

friend class Singleton<Logger>;

public:
    enum Severity { INFO,
                    WARNING,
                    ERROR,
                    NONE,
                    NUM_OF_SEVERITY };


public:
    void SetSeverity(Severity a_severity);
    Severity GetSeverity() const; 
    
    void Info(const std::string &a_msg);
    void Warn(const std::string &a_msg);
    void Error(const std::string &a_msg);
private:
    Logger();
    ~Logger();

private:
    Severity m_currentSeverity;
    Mutex m_mutex;
};
} //namespace experis

#endif // LOGGER_H_