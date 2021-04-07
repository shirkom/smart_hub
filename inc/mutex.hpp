
#ifndef MUTEX_H__
#define MUTEX_H__

#include <exception>
#include <pthread.h>
#include "common.hpp"


namespace experis {


class Mutex : private Uncopyable {
    friend class Condition;

    class BusyException;
    class DeadLockException;
    class NoMemoryException;
    class NoPermissionsException;
public:
    ~Mutex();        
    Mutex();  
    void Lock();  
    void Unlock(); 
       
private:
   const pthread_mutex_t * GetMutex();

private:
    pthread_mutex_t m_mutex;
};

class Mutex::BusyException : public std::exception {
public:
    virtual const char* what() const throw() { return "The implementation has detected an attempt to destroy the object referenced by mutex while it is locked or referenced"; };
};

class Mutex::DeadLockException : public std::exception {
public:
    virtual const char* what() const throw() { return "The current thread already owns the mutex."; };
};

class Mutex::NoMemoryException : public std::exception {
public:
    virtual const char* what() const throw() { return "Insufficient memory exists to initialize the mutex."; };
};

class Mutex::NoPermissionsException : public std::exception {
public:
    virtual const char* what() const throw() { return "The caller does not have the privilege to perform the operation."; };
};

}
#endif // __MUTEX_H__