// 
// Copyright (C) 2013 Karol Roslaniec <llog@roslaniec.net>
// 
#ifndef LINKO_LOGGER_HPP
#define LINKO_LOGGER_HPP

#include <llog/intrusive_ptr.hpp>

namespace linko {

class LoggerImpl;
typedef intrusive_ptr<LoggerImpl> Logger;

void intrusive_ptr_add_ref(LoggerImpl *ptr);
void intrusive_ptr_release(LoggerImpl *ptr);


}
#endif
