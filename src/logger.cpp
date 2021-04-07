#include <csignal>
#include <iostream>

#include "../inc/logger.hpp"
#include "../inc/singleton.hpp"
#include "../inc/inl/singleton_inl.hpp"

namespace experis {
   

// static void signalHandler(int a_pid) {
//     Logger& l = Singleton<Logger>::GetInstance();
//     int severity = (l.GetSeverity() + 1) % Logger::NUM_OF_SEVERITY;
//     l.SetSeverity((Logger::Severity) severity);
// }

Logger::Logger()
: m_currentSeverity(NONE)
{/*empty*/}

void Logger::SetSeverity(Severity a_severity) {
    m_currentSeverity = a_severity;
}

Logger::Severity Logger::GetSeverity() const{
    return m_currentSeverity;
}

void Logger::Info(const std::string &a_msg) {
    m_mutex.Lock();
    if(m_currentSeverity == INFO) {
        std::cout << a_msg << std::endl;
    }
    m_mutex.Unlock();
}

void Logger::Warn(const std::string &a_msg) {
    m_mutex.Lock();
    if(m_currentSeverity == WARNING || m_currentSeverity == ERROR || m_currentSeverity == INFO) {
        std::cout << a_msg << std::endl;
    }
    m_mutex.Unlock();
}

void Logger::Error(const std::string &a_msg) {
    m_mutex.Lock();
    if(m_currentSeverity == ERROR || m_currentSeverity == INFO) {
        std::cout << a_msg << std::endl;
    }
    m_mutex.Unlock();
}

} //namespace experis