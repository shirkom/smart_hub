#ifndef ATOMICFLAG_H__
#define ATOMICFLAG_H__

namespace experis {
class AtomicFlag {
public:
    AtomicFlag();

    bool Acquire(); //flag on
    bool Release(); //flag off
    void WaitFor(); //while 
    bool IsUp();
private:
    bool m_flag;

}; 
} //experis
#endif // ATOMICFLAG_H__