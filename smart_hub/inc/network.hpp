#ifndef __NETWORK_H__
#define __NETWORK_H__

extern "C" {
    #include "ServerNet.h"
}
#include "connection.hpp"
#include <string>       /*string*/
#include <unistd.h>     /*close*/

namespace experis {
class Connection;

class NetworkTask {
public:
    explicit NetworkTask(){}
    virtual ~NetworkTask(){}
    virtual void HandlingTask(Connection a_connection, const std::string& a_message) = 0;
private:

};

class Network {
public:
    static void Send(Connection a_connection, std::string& a_message, size_t a_sizeSend); 
    explicit Network(NetworkTask* a_networkTask, int a_port);
    ~Network();
    void Run();

private:    
    Network(const Network& a_networkToCopy);
    const Network& operator=(const Network& a_networkToAssignmet);

    ServerNet* m_serverNet;
};

} //namespace experis
#endif // __NETWORK_H__
