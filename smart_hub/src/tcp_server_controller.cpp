#include "tcp_server_controller.hpp"
#include "thread.hpp"
#include "locker.hpp"
#include <iostream>

namespace experis {

class ControllerTask : public NetworkTask { 
public:
    ControllerTask(TcpServerController *a_tcpServer);
    virtual ~ControllerTask();
    virtual void HandlingTask(Connection a_connection, const std::string& a_message);
 
private: 
    TcpServerController* m_tcpServerController;

};

ControllerTask::ControllerTask(TcpServerController *a_tcpServer) 
: m_tcpServerController(a_tcpServer)
{
    
}

ControllerTask::~ControllerTask() 
{
    
}

void ControllerTask::HandlingTask(Connection a_connection, const std::string& a_message) 
{
    Locker lkr(m_tcpServerController->m_mutex);
    if(!m_tcpServerController->m_isConnect)
    {
        m_tcpServerController->m_isConnect = true;
        m_tcpServerController->m_connection = a_connection;
    }
    else
    {
        std::string replay = "HTTP/1.1 200 OK\r\n\r\n Error - allready connected! \r\n";
        Network::Send(a_connection, replay, replay.size());
        a_connection.Close();
    }

}

///////////////////////////////////////////////////////////////////////////////////

void RunServerNet(Network* a_network)   //threadFunc
{
    a_network->Run();
}

TcpServerController::TcpServerController(ISubscribable* a_subscribable, 
                        const std::string& a_building,
                        const std::string& a_type,
                        const std::string& a_floor,
                        const std::string& a_room,
                        int a_port) 
: ControllerAgentBase(a_subscribable, a_building, a_type, a_floor, a_room)
, m_task(new ControllerTask(this))
, m_network(new Network(m_task, a_port))
, m_isConnect(false)
, m_connection(-1)
, m_threadRunServer(new Thread<Network>(RunServerNet, m_network))
, m_mutex()
, m_marselEvent(new EventMarshal)
{

}

TcpServerController::~TcpServerController() 
{
    std::cout << "kill thread" << std::endl;
    m_threadRunServer->Kill();
    m_connection.Close();
    delete m_task;
    delete m_network; 
    delete m_marselEvent;
}

void TcpServerController::PublishEvent(SharedPtr<Event> a_sharedPtrEvent) 
{
    Locker lkr(m_mutex);
    std::string replay = "HTTP/1.1 200 OK\r\n\r\n" + m_marselEvent->Marshal(a_sharedPtrEvent) + "\r\n";
    if(m_isConnect)
    {
        Network::Send(m_connection, replay, replay.size());
    }
}

} //experis;
