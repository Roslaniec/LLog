// 
// Copyright (C) 2013 Karol Roslaniec <llog@roslaniec.net>
// 
#ifndef LINKO_LOGGABLE_HPP
#define LINKO_LOGGABLE_HPP

#include "logger.hpp"
#include "loglevel.hpp"

/*
 * Normally Log instances are created for each thread.
 * Loggable/Logger classes create Log instance for a single object (module).
 * Motivation for it was to allow per class/module log level definition,
 * eg. you can anable Debug level for TFTP client module/class.
 *
 * IMPORTANT: 
 * Don't use it for short-living objects.
 * Might be used for long-living objects.
 */

namespace linko {

class Log;

class Loggable
{
public:
    Log &log(LogLevel l);
    
    Log &logA();
    
    Log &logE();
    
    Log &logW();
    
    Log &logI();
    
    Log &logD();
    
    Log &logP();
    
    bool lvl(LogLevel l);
    
    bool lvlW();
    
    bool lvlI();
    
    bool lvlD();
    
    bool lvlP(); 

    Logger logger() const { return _logger; }
    
protected: 
    Loggable(const Logger &logger) : _logger(logger) {}

private:
    Logger _logger;
};


}
#endif
