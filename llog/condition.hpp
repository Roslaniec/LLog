// 
// Copyright (C) 2013 Karol Roslaniec <llog@roslaniec.net>
// 
#ifndef LINKO_CONDITION_VARIABLE_HPP
#define LINKO_CONDITION_VARIABLE_HPP

#include <pthread.h>

namespace linko {

class mutex;

class condition
{
private:
    condition(condition const&);
    condition &operator=(condition const&);
    static void throw_condition_error();
    pthread_cond_t c;

public:
    condition();

    ~condition();

    void wait(mutex &m);

    template <typename Lock> void wait(Lock &lock)
        {
#           ifndef NDEBUG
            if(!lock.owns_lock()) throw_condition_error();
#           endif
            wait(*const_cast<mutex*>(lock.mutex()));
        }
    
    void notify_one() throw();
    
    void notify_all() throw();
};

}
#endif
