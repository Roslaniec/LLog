// 
// Copyright (C) 2013 Karol Roslaniec <llog@roslaniec.net>
// 
#ifndef LINKO_LOG_HPP
#define LINKO_LOG_HPP

#include "log_cfg.hpp"
#include "loglevel.hpp"
#include "log_sink.hpp"
#include "log_streambuf.hpp"
#include <ostream>


namespace linko { 

class Log : public std::ostream
{
public:
    static Log *it() { return _instance; }

    static void createIt(LogSink &sink);
    
    static void destroyIt();

    LogLevel level() const { return _streambuf.level(); }

    Log &level(LogLevel level) { return _streambuf.level(level), *this; };

    Log &A() { return level(LogAlways); }

    Log &E() { return level(LogError); }

    Log &W() { return level(LogWarning); }

    Log &I() { return level(LogInfo); }

    Log &D() { return level(LogDebug); }

    Log &P() { return level(LogPrint); }

    bool silent() const { return _streambuf.silent(); }

    bool silent(LogLevel level) const { return _streambuf.silent(level); }

    const LogSink &sink() const { return _sink; }

protected:
    Log(const LogSink &sink, const LogLevel &sinkLevel);

    ~Log();

private:
    Log(const LogSink &sink);
    
    LogSink _sink;
    LogStreambuf _streambuf;
    static thread_local Log *_instance;
};


inline Log &log(LogLevel l) { return Log::it()->level(l); }

inline Log &logA() { return Log::it()->level(LogAlways); }

inline Log &logE() { return Log::it()->level(LogError); }

inline Log &logW() { return Log::it()->level(LogWarning); }

inline Log &logI() { return Log::it()->level(LogInfo); }

inline Log &logD() { return Log::it()->level(LogDebug); }

inline Log &logP() { return Log::it()->level(LogPrint); }

inline bool lvl(LogLevel l) {
    const Log *const log = Log::it();
    return log && !log->silent(l);
}

inline bool lvlW() { return lvl(LogWarning); }

inline bool lvlI() { return lvl(LogInfo); }

inline bool lvlD() { return lvl(LogDebug); }

inline bool lvlP() { return lvl(LogPrint); }


// class LogFlush
// {
// public:
//     LogFlush() {}
//     ~LogFlush() { Log::it()->flush(); }
// };


}

#ifdef NDEBUG
# define LOG_P(str) do {} while(0)
# define LOG_Px(cond, str) do {} while(0)
#else
# define LOG_P(str) \
    do { if (lvlP()) { logP() << str << '\n'; } } while(0)
# define LOG_Px(cond, str) \
    do { if (lvlP() && cond) { logP() << str << '\n'; } } while(0)
#endif


// Local Variables:
// mode: c++
// End:
#endif

