// 
// Copyright (C) 2013 Karol Roslaniec <llog@roslaniec.net>
// 
#ifndef LINKO_LOG_SINK_HPP
#define LINKO_LOG_SINK_HPP

#include <boost/intrusive_ptr.hpp>

namespace linko {

class LogSinkImpl;
typedef boost::intrusive_ptr<LogSinkImpl> LogSink;

void intrusive_ptr_add_ref(LogSinkImpl *ptr);
void intrusive_ptr_release(LogSinkImpl *ptr);

}
#endif
