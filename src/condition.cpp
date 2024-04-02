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
#include <llog/condition.hpp>
#include <llog/mutex.hpp>
#include <llog/system_error.hpp>
#include <cassert>

namespace linko {

condition::condition()
{
    int const res = pthread_cond_init(&c, NULL);
    if (res) throw_system_error(res, "pthread_cond_init failed");
}


condition::~condition()
{
    int const res = pthread_cond_destroy(&c);
    if (res) throw_system_error(res, "pthread_cond_destroy failed");
}


void
condition::throw_condition_error()
{
    throw_system_error(EINVAL, "linko::condition error");
}


void
condition::wait(mutex &m)
{
    pthread_mutex_t* the_mutex = m.native_handle();
    int const res = pthread_cond_wait(&c, the_mutex);
    if (res) throw_system_error(res, "pthread_cond_wait failed");
}


void
condition::notify_one() throw()
{
    int const res = pthread_cond_signal(&c);
    assert(!res); (void)res;
}


void
condition::notify_all() throw()
{
    int const res = pthread_cond_broadcast(&c);
    assert(!res); (void)res;
}


}
