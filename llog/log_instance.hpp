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

}

#endif
