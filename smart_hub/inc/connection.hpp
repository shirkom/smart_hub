#ifndef __CONNECTION_H__
#define __CONNECTION_H__

extern "C" {
#include "ServerNet.h"
}
#include "unistd.h"

namespace experis {

class Network;

class Connection {
public:
    friend class Network;
    explicit Connection(int a_socket) : m_socket(a_socket), m_flag(false) {}
    ~Connection() {}
    void Close() {close(m_socket); m_flag = true; }
private:
    int m_socket;
    bool m_flag;
};
}//experis


#endif // __CONNECTION_H__