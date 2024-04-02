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
#include <llog/thread_id.hpp>
#include <llog/log.hpp>
#include <llog/log_instance.hpp>
#include <llog/log_sink_impl.hpp>
#include <llog/timer.hpp>
#include <iostream>
# include <thread>

using std::thread;
using namespace linko;

LogSink logSink;


void test_log(Log &log, int count)
{
#   ifdef LINKO_LOG_MT
    const int id = thread_id();
#   else
    const int id = 0;
#   endif
    log.A() << id << " Test starting..." << std::endl;
    for (int i = 0; i < count; ++i) {
        log.I() << id << " Info " << i << '\n';
        log.W() << id << " Warn " << i << std::endl;
        log.E() << id << " Err " << i << std::endl;
        log.D() << id << " Debug " << i << "\n";
    }
    log.A() << id << " ...test finished" << std::endl;
}


#ifndef LINKO_LOG_MT
void test_log(linko::old::LogOld &log, int count)
{
    log.A() << "Test starting..." << std::endl;
    for (int i = 0; i < count; ++i) {
        log.I() << "Info " << i << '\n';
        log.W() << "Warn " << i << std::endl;
        log.E() << "Err " << i << std::endl;
        log.D() << "Debug " << i << "\n";
    }
    log.A() << "...test finished" << std::endl;
}
#endif


#ifdef LINKO_LOG_MT
void
mt_test_proc(int count) 
{
    LogInstance instance(logSink);
    // Log::createIt(logSink);
    test_log(*Log::it(), count);
    // Log::destroyIt();
}

void
test_multi_thread(int count)
{
    thread thr1(mt_test_proc, count);
#   if 1
    thread thr2(mt_test_proc, count);
    thread thr3(mt_test_proc, count);
    thread thr4(mt_test_proc, count);
    thread thr5(mt_test_proc, count);
    
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    std::time_t time = std::time(0);
    time += 3600*24;
    logSink->rotate(time);
    
    thr5.join();
    thr4.join();
    thr3.join();
    thr2.join();
#   endif
    thr1.join();
}

#else

void
test_single_thread(int count) 
{
    Log::createIt(logSink); 
    test_log(*Log::it(), count);

    std::time_t time = std::time(0);
    time += 3600*24;
    logSink->rotate(time);
    test_log(*Log::it(), count);

    time += 3600*24;
    logSink->rotate(time);
    test_log(*Log::it(), count);

    Log::destroyIt();
}

void
test_single_thread_old(int count) 
{
    using namespace linko::old;
    LogOld::createIt("/tmp", "testold");

    test_log(*LogOld::it(), count);
    std::time_t time = std::time(0);
    time += 3600*24;
    LogOld::it()->rotate(time);
    test_log(*LogOld::it(), count);

    time += 3600*24;
    LogOld::it()->rotate(time);
    test_log(*LogOld::it(), count);

    LogOld::destroyIt();
}
#endif



int main() {
    logSink = LogSinkImpl::create("/tmp", "testnew");
    logSink->start();
    // logSink->write_direct('E', "[TEST] ", "This is\nmultiline\nprotobuf error");

    Timer start;

#   ifdef LINKO_LOG_MT
    test_multi_thread(10000);
#   else
    test_single_thread(10000);
    test_single_thread_old(10000);
#   endif

    int dur = Timer::now() - start;
    std::cout << "Test finished in " << dur << " usec" << std::endl;
}
