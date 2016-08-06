// 
// Copyright (C) 2013 Karol Roslaniec <llog@roslaniec.net>
// 
#ifndef LINKO_LOGGER_IMPL_HPP
#define LINKO_LOGGER_IMPL_HPP

#include "logger.hpp"
#include "log_sink.hpp"
#include "log.hpp"

namespace linko {

Logger createLogger(const LogSink &sink);

class LoggerImpl : public Log
{
public:
    void sink_level (LogLevel level) { _sinkLevel = level; }

private:
    LoggerImpl(const LogSink &sink);

    LogLevel _sinkLevel;
    unsigned _refs;

    friend class Loggable;
    friend Logger createLogger(const LogSink &sink);
    friend void intrusive_ptr_add_ref(LoggerImpl *ptr);
    friend void intrusive_ptr_release(LoggerImpl *ptr);
};


}
#endif
