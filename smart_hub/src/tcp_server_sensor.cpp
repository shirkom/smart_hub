#include "tcp_server_sensor.hpp"
#include "thread.hpp"
#include "locker.hpp"
#include "locker.hpp"

#include <iostream>
#include <vector>

namespace experis {

class SensorTask : public NetworkTask { 
public:
    SensorTask(TcpServerSensor *a_tcpServer);
    virtual ~SensorTask();
    virtual void HandlingTask(Connection a_connection, const std::string& a_message);
 
private: 
    TcpServerSensor* m_tcpServerController;
    HttpEventUnserializer m_httpEventUnserializer;

};

SensorTask::SensorTask(TcpServerSensor *a_tcpServer) 
: m_tcpServerController(a_tcpServer)
{
    
}

SensorTask::~SensorTask() 
{
    
}

std::vector<std::string> Split(const std::string& a_str, std::string a_delimiter) {
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

void SensorTask::HandlingTask(Connection a_connection, const std::string& a_message) 
{
    std::vector<std::string> lines = Split(a_message, "\n");
    std::vector<std::string> words = Split(lines[0], " ");
    std::vector<std::string> parts = Split(words[1], "/");

    if(6 != parts.size())
    {
        throw WrongArgs();
    }
    Location location(parts[0], parts[1], parts[2]);
    Event event(parts[3], parts[4], parts[5], location);
    
    SharedPtr<Event> eventSharedPtr(new Event(event));
    m_tcpServerController->SendEvent(eventSharedPtr);
}


///////////////////////////////////////////////////////////////////////////////////

void RunServerNet(Network* a_network)   //threadFunc
{
    a_network->Run();
}

TcpServerSensor::TcpServerSensor(IEventReceiver* a_receiver, int a_port)
: SensorAgentBase(a_receiver)
, m_task(new SensorTask(this))
, m_network(new Network(m_task, a_port))
, m_threadRunServer(new Thread<Network>(RunServerNet, m_network))
{
    
}

TcpServerSensor::~TcpServerSensor() 
{
    std::cout << "kill thread" << std::endl;
    m_threadRunServer->Kill();
    delete m_task;
    delete m_network; 
}

IAgent* CreateTcpServerSensor(const DataAgent& a_data, ISubscribable* a_subscribable, IEventReceiver* a_eventReceiver) 
{
    return new TcpServerSensor(a_eventReceiver, 5555);
}

} //experis;
