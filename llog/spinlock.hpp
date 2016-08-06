// 
// Copyright (C) 2013 Karol Roslaniec <llog@roslaniec.net>
// 
#ifndef LINKO_SPINLOCK_HPP
#define LINKO_SPINLOCK_HPP

#include <pthread.h>

namespace linko {

class spinlock
{
private:
    spinlock(spinlock const&);
    spinlock& operator=(spinlock const&);
    pthread_spinlock_t m;

public:
    spinlock();

    ~spinlock();

    bool try_lock();
    
    void lock();
    
    void unlock();
    

    // typedef unique_lock<mutex> scoped_lock;
    // typedef detail::try_lock_wrapper<mutex> scoped_try_lock;

    // class scoped_lock
    // {
    // private:

    //     spinlock & sp_;

    //     scoped_lock( scoped_lock const & );
    //     scoped_lock & operator=( scoped_lock const & );

    // public:

    //     explicit scoped_lock( spinlock & sp ): sp_( sp )
    //     {
    //         sp.lock();
    //     }

    //     ~scoped_lock()
    //     {
    //         sp_.unlock();
    //     }
    // };
};


}
#endif
