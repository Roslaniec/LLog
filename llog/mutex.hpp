// 
// Copyright (C) 2013 Karol Roslaniec <llog@roslaniec.net>
// 
#ifndef LINKO_MUTEX_HPP
#define LINKO_MUTEX_HPP

#include <pthread.h>

namespace linko {

class mutex
{
private:
    mutex(mutex const&);
    mutex &operator=(mutex const&);
    pthread_mutex_t m;

public:
    mutex();

    ~mutex();

    bool try_lock();
    
    void lock();
    
    void unlock();
    
    typedef pthread_mutex_t* native_handle_type;
    native_handle_type native_handle() { return &m; }
};

}
#endif
