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
#include <llog/log_streambuf.hpp>
#include <llog/log_sink_impl.hpp>


namespace linko {


LogStreambuf::LogStreambuf(const LogSink &sink)
    : _sink(sink),
      _sinkLevel(sink->level_ref()),
      _msgLevel(LogInfo), 
      _printTime(true), 
      _logTimestamp(),
      _pptr(_buffer)
{
}


LogStreambuf::LogStreambuf(const LogSink &sink, const LogLevel &sinkLevel)
    : _sink(sink),
      _sinkLevel(sinkLevel),
      _msgLevel(LogInfo), 
      _printTime(true), 
      _logTimestamp(),
      _pptr(_buffer)
{
}


LogStreambuf::~LogStreambuf()
{
    sync();
}


LogStreambuf::int_type
LogStreambuf::overflow(int_type ch)
{
    if (full() && sync() != 0) return traits_type::eof();
    if (silent()) return traits_type::not_eof(ch);
    if (_printTime) print_timestamp();
    _printTime = ch == '\n';
    *_pptr++ = ch;
    return traits_type::not_eof(ch);
}


int
LogStreambuf::sync()
{
    if (_pptr == _buffer) return 0;
    bool r = _sink->write(_buffer, _pptr - _buffer);
    _pptr = _buffer;
    return r ? 0 : -1; // -1 on failure
}


void
LogStreambuf::print_timestamp()
{
    if (LogPrint == _msgLevel) return;
    _logTimestamp.update(level2char(_msgLevel));
    _pptr = _logTimestamp.copy(_pptr);
}




}
