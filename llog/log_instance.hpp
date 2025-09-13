// 
// Copyright (C) 2013 Karol Roslaniec <llog@roslaniec.net>
// 
#ifndef LINKO_LOG_INSTANCE_HPP
#define LINKO_LOG_INSTANCE_HPP

#include "log.hpp"

namespace linko {

/*
 * Initialize Log::it() object.
 * Create this object at the very beginning (after creating a new thread)
 */
class LogInstance
{
public:
    LogInstance(LogSink &sink) { Log::createIt(sink); }

    ~LogInstance() { Log::destroyIt(); }
};


/*
 * Reentrant (a.k.a. recursive) version of LogInstance.
 * Log:it() object will be created only if it is null.
 * Will be destroyed only if created by this LogInstanceR instance.
 */
class LogInstanceR
{
public:
    LogInstanceR(LogSink &sink)
        : _b(!Log::it()) { if (_b) Log::createIt(sink); }

    ~LogInstanceR() { if (_b) Log::destroyIt(); }
    
private:
    bool _b;
};


LogSink LogSink_create(const std::string &dir,
                       const std::string &prog,
                       unsigned mode = 0644);

void start(const LogSink &sink);

LogLevel get_level(const LogSink &sink);

void set_level(const LogSink &sink, LogLevel level);

void rotate(const LogSink &sink, std::time_t time);

}

#endif
