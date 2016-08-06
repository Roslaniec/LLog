// 
// Copyright (C) 2013 Karol Roslaniec <llog@roslaniec.net>
// 
#ifndef LINKO_LOG_STREAMBUF_HPP
#define LINKO_LOG_STREAMBUF_HPP

#include "loglevel.hpp"
#include "log_cfg.hpp"
#include "log_sink.hpp"
#include "log_timestamp.hpp"
#include <streambuf>

namespace linko {

class LogStreambuf : public std::streambuf 
{
public:
    LogStreambuf(const LogSink &sink);

    LogStreambuf(const LogSink &sink, const LogLevel &sinkLevel);

    virtual ~LogStreambuf();

    LogLevel level() const { return _msgLevel; }

    void level(LogLevel level) { _msgLevel = level; }

    bool silent(LogLevel level) const { return _sinkLevel < level; }

    bool silent() const { return silent(_msgLevel); }

protected:
    virtual int_type overflow(int_type ch);

    virtual int sync();

private:
    inline void print_timestamp();

    inline size_t size() const { return _pptr - _buffer; }

    inline bool full() const { return LINKO_LOG_BUFFER_LENGTH <= size(); }

    LogSink _sink;
    const LogLevel &_sinkLevel;
    LogLevel _msgLevel;
    bool _printTime;
    LogTimestamp _logTimestamp;
    char _buffer[LogTimestamp::SIZE + LINKO_LOG_BUFFER_LENGTH + 1];
    char *_pptr;
};

}
#endif
