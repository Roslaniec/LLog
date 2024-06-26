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
#include <llog/mutex.hpp>
#include <llog/system_error.hpp>

namespace linko {

mutex::mutex()
{
    int const res = pthread_mutex_init(&m, NULL);
    if (res) throw_system_error(res, "pthread_mutex_init failed");
}


mutex::~mutex()
{
    const int res = pthread_mutex_destroy(&m);
    if (res) throw_system_error(res, "pthread_mutex_destroy failed");
}


bool
mutex::try_lock()
{
    const int res = pthread_mutex_trylock(&m);
    if (res && (res != EBUSY))
        throw_system_error(res, "pthread_mutex_trylock failed");
    return !res;
}

    
void
mutex::lock()
{
    const int res = pthread_mutex_lock(&m);
    if (res) throw_system_error(res, "pthread_mutex_lock failed");
}


void
mutex::unlock()
{
    const int res = pthread_mutex_unlock(&m);
    if (res) throw_system_error(res, "pthread_mutex_unlock failed");
}


}
