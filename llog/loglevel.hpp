// 
// Copyright (C) 2013 Karol Roslaniec <llog@roslaniec.net>
// 
#ifndef LINKO_LOGLEVEL_HPP
#define LINKO_LOGLEVEL_HPP

namespace linko { 

enum LogLevel {
    LogAlways,
    LogError,
    LogWarning,
    LogInfo,
    LogDebug,
    LogPrint,
    LogLevelEND
};


LogLevel char2level(char lname);

static const char LogLevelChar[LogLevelEND + 1] = 
{ 'A', 'E', 'W', 'I', 'D', 'P', 'X' };

static inline char level2char(LogLevel level) 
{ 
    return LogLevelChar[level]; 
}


}


// Local Variables:
// mode: c++
// End:
#endif
