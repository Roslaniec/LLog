// 
// Copyright (C) 2024 Karol Roslaniec <llog@roslaniec.net>
// 
#ifndef LINKO_SYSTEM_ERROR_HPP
#define LINKO_SYSTEM_ERROR_HPP

#include <system_error>

namespace linko {


inline void
throw_system_error(int err, const char *msg)
{
    throw std::system_error(err, std::generic_category(), msg);
}


}
#endif
