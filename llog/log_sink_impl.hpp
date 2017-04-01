// 
// Copyright (C) 2013 Karol Roslaniec <llog@roslaniec.net>
// 
#ifndef LINKO_LOG_SINK_IMPL_HPP
#define LINKO_LOG_SINK_IMPL_HPP

#include "log_cfg.hpp"
#include "loglevel.hpp"
#include "log_sink.hpp"
#include "spinlock.hpp"
#include <ctime>
#include <iosfwd>
#include <string>
#include <boost/atomic.hpp>
#include <boost/noncopyable.hpp>

#ifdef LINKO_LOG_MT
# if LINKO_LOG_BOOST_THREAD
#  include <boost/thread/thread.hpp>
# else
#  include <thread>
# endif
#include "mutex.hpp"
#include "condition.hpp"
#include <queue>
#include <stack>
#endif 

namespace linko {

class LogTimestamp;

class LogSinkImpl : private boost::noncopyable
{
public:
    static LogSink create(const std::string &dir, const std::string &prog);
    
    void level (LogLevel level) { _fileLevel = level; }

    LogLevel level() const { return _fileLevel; }

    const LogLevel &level_ref() const { return _fileLevel; }

    bool silent(LogLevel level) const { return _fileLevel < level; }

    void rotate(std::time_t time);

    bool write(const char* s, size_t n);

#   ifdef LINKO_LOG_THREAD
    void start(); // Start logging thread
#   endif

    // DEPRECATED
    // bool write_direct(char lvl, const char *prefix, const char *str);

private:
    LogSinkImpl(const std::string &dir, const std::string &prog);

    ~LogSinkImpl();

    bool mx_open(const char *filename, LogLevel fileLevel = LogInfo);

    void mx_close();
    
    bool mx_write(const void* s, size_t n);

    bool mx_print(char lvl, const char* s);

    bool mx_print(const LogTimestamp &ts, const char* s);

    void mx_rotate(bool print_msg);

    LogLevel _fileLevel;
    std::string _dir;
    std::string _prog;
    std::time_t _time;
    int _fd;
    boost::atomic<unsigned> _refs;

#   ifdef LINKO_LOG_MT
    typedef linko::mutex mutex_t;
    mutex_t _mutex;
#   endif 

#   ifdef LINKO_LOG_THREAD
    void run();
    void stop(); // Stop logging thread
    inline bool thr_write(const void* s, size_t n);
#   if LINKO_LOG_BOOST_THREAD
    boost::thread *_thread;
#   else
    std::thread *_thread;
#   endif
    linko::condition _cond;
    struct Buffer;
    std::queue<Buffer> _queue; // free
    volatile bool _running;
    volatile bool _terminate;
    static const unsigned FREE_SIZE = 10;
    std::stack<Buffer, std::vector<Buffer> > _free; // free
    spinlock _free_lck;
#   endif 

    friend void intrusive_ptr_add_ref(LogSinkImpl *ptr);
    friend void intrusive_ptr_release(LogSinkImpl *ptr);
};


}
#endif
