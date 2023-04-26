// 
// Copyright (C) 2013 Karol Roslaniec <llog@roslaniec.net>
// 
#ifndef LINKO_TIMER_HPP
#define LINKO_TIMER_HPP

#include <sys/time.h>

namespace linko {


struct Timer {
    unsigned long usec; // microseconds

    // Default constructor initializes to current time.
    // Why not to 0? Because 0 actually is some arbitrary time, not so
    // far away from now (because time is stored in microseconds),
    // therefore it cannot represent something like null-time.
    Timer() { *this = now(); }

    Timer(unsigned long usec) : usec(usec) {}

    Timer(const Timer &time) : usec(time.usec) {}

    // Better don't cast. Use usec directly.
    // operator unsigned long() const { return usec; }

#   if 201103L <= __cplusplus    
    Timer &operator=(const Timer &time) = default;
#   endif

    Timer &operator++() 
        { ++usec; return *this; }

    Timer &operator--() 
        { --usec; return *this; }

    // minus returns signed int!
    signed int operator-(const Timer &time) const
        { return minus(usec, time.usec); }

    bool operator>(const Timer &time) const 
        { return minus(usec, time.usec) > 0; }

    bool operator>=(const Timer &time) const 
        { return minus(usec, time.usec) >= 0; }

    bool operator<(const Timer &time) const 
        { return !(*this >= time); }

    bool operator<=(const Timer &time) const 
        { return !(*this > time); }

    static Timer now()
        {
            struct timeval tv;
            gettimeofday(&tv, 0);
            return Timer(tv.tv_sec * 1000000UL + tv.tv_usec);
        }

private:
    signed int minus(unsigned int usec1, unsigned int usec2) const
        { return (signed)(usec1 - usec2); }

    // Adding two absolute times do not make sense
    Timer &operator+(const Timer &time);
};



}
#endif
