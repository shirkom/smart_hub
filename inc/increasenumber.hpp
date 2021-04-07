#ifndef INCREASENUMBER_H__
#define INCREASENUMBER_H__


namespace experis { 
class IncreaseNumber {
public:
    IncreaseNumber(): m_number(0) {/*empty*/}
    int Inc(){return ++m_number;}
    int GetNum(){return m_number;}

private:
    int m_number;
};
} //experis
#endif //  INCREASENUMBER_H__