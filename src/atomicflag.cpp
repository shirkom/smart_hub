
#include "../inc/atomicflag.hpp"

namespace experis {

AtomicFlag::AtomicFlag() 
: m_flag (false)
{
    
}

bool AtomicFlag::Acquire() 
{
    return __sync_bool_compare_and_swap(&m_flag, false, true);
}


bool AtomicFlag::Release() 
{
    return __sync_bool_compare_and_swap(&m_flag, true, false);
}


void AtomicFlag::WaitFor() 
{
    while(__sync_bool_compare_and_swap(&m_flag, false, true)) 
    {

    }
}

bool AtomicFlag::IsUp() 
{
    return __sync_bool_compare_and_swap(&m_flag, true, true);
}
} //experis
