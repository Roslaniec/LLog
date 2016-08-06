/******************************************************************************
  Copyright (C) 2013 Karol Roslaniec <llog@roslaniec.net>
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
******************************************************************************/
#include <llog/loggable.hpp>
#include <llog/logger_impl.hpp>

namespace linko {

Log &Loggable::log(LogLevel l) { return _logger->level(l); }
    
Log &Loggable::logA() { return _logger->A(); }

Log &Loggable::logE() { return _logger->E(); }

Log &Loggable::logW() { return _logger->W(); }

Log &Loggable::logI() { return _logger->I(); }

Log &Loggable::logD() { return _logger->D(); }

Log &Loggable::logP() { return _logger->P(); }

bool Loggable::lvl(LogLevel l) { return !_logger->silent(l); }

bool Loggable::lvlW() { return !_logger->silent(LogWarning); }
    
bool Loggable::lvlI() { return !_logger->silent(LogInfo); }

bool Loggable::lvlD() { return !_logger->silent(LogDebug); }

bool Loggable::lvlP() { return !_logger->silent(LogPrint); }

}
