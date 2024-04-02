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
#include <llog/log.hpp>
#include <llog/log_streambuf.hpp>
#include <llog/log_sink.hpp>
#include <cassert>


namespace linko { 

thread_local Log *Log::_instance = 0;


void
Log::createIt(LogSink &sink)
{
    assert(!_instance);
    _instance = new Log(sink);
}

    
void
Log::destroyIt()
{
    delete _instance;
    _instance = 0;
}


Log::Log(const LogSink &sink)
    : std::ostream(0)
    , _sink(sink)
    , _streambuf(sink)
{
    rdbuf(&_streambuf);
}


Log::Log(const LogSink &sink, const LogLevel &sinkLevel)
    : std::ostream(0)
    , _sink(sink)
    , _streambuf(sink, sinkLevel)
{
    rdbuf(&_streambuf);
}


Log::~Log()
{
}




} 
