// 
// Copyright (C) 2013 Karol Roslaniec <llog@roslaniec.net>
// 
#ifndef LINKO_LOG_INSTANCE_HPP
#define LINKO_LOG_INSTANCE_HPP

#include "log.hpp"

namespace linko {

class LogInstance
{
public:
    LogInstance(LogSink &sink) { Log::createIt(sink); }

    ~LogInstance() { Log::destroyIt(); }
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
