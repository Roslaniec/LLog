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
#include <llog/spinlock.hpp>
#include <boost/thread/exceptions.hpp>
#include <boost/throw_exception.hpp>

namespace linko {

spinlock::spinlock()
{
    int const res = pthread_spin_init(&m, PTHREAD_PROCESS_PRIVATE);
    if(res) boost::throw_exception(
        boost::thread_resource_error(res, "pthread_spin_init failed"));
}


spinlock::~spinlock()
{
    const int res = pthread_spin_destroy(&m);
    if (res) boost::throw_exception(
        boost::lock_error(res, "pthread_spin_destroy failed"));
}


bool
spinlock::try_lock()
{
    const int res = pthread_spin_trylock(&m);
    if (res && (res != EBUSY)) boost::throw_exception(boost::lock_error(res));
    return !res;
}

    
void
spinlock::lock()
{
    const int res = pthread_spin_lock(&m);
    if (res) boost::throw_exception(
        boost::lock_error(res, "pthread_spin_lock failed"));
}

    
void
spinlock::unlock()
{
    const int res = pthread_spin_unlock(&m);
    if (res) boost::throw_exception(
        boost::lock_error(res, "pthread_spin_unlock failed"));
}


}
