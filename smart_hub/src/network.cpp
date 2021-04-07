extern "C" {
#include "ServerNet.h"
}
#include "connection.hpp"
#include <string>
#include "network.hpp"


namespace experis {

static const int PORT = 5555;
static const int BACKLOG = 10; 
static const int MAX_SIZE_BUFFER = 1024;

typedef ServerNetErr (*GotMsg)(void *_recieved, size_t _size, int _sock, void* _appContext);
typedef ServerNetErr (*Fail)(ServerNetErr _status, int _errno, void* _appContext);
typedef void (*NewClient)(int _sock, void* _appContext);
typedef void (*CloseClient)(int _sock, void* _appContext);

static int GotMessage( void *_data, size_t _size, int _clientSock, void* _server) {
    std::string string = static_cast<char*>(_data);
    Connection connection (_clientSock);
    static_cast<NetworkTask*>(_server)->HandlingTask(connection, string);
    return 0;
} 

Network::Network( NetworkTask* a_networkTask, int a_port) 
: m_serverNet(ServerNetCreate(BACKLOG,a_port,MAX_SIZE_BUFFER, (GotMsg)(GotMessage), NULL, NULL, NULL, a_networkTask))
{/*empty*/}

Network::~Network() {
    ServerNetDestroy(&m_serverNet);
}

void Network::Send(Connection a_connection, std::string& a_message, size_t a_sizeSend) {
    ServerNetSend(a_connection.m_socket, const_cast<char*>(a_message.c_str()), a_sizeSend);
}

void Network::Run() {
    ServerNetRun(m_serverNet);    
}


}//experis