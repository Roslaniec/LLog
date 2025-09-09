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
#include <llog/log_sink_impl.hpp>
#include <llog/log_timestamp.hpp>
#include <cassert>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <mutex>

namespace linko {

#ifdef LINKO_LOG_MT
# define MUTEX_LOCK(mtx) mtx.lock()
# define MUTEX_UNLOCK(mtx) mtx.unlock()
# define MUTEX_GUARD(mtx) std::lock_guard<mutex_t> guard(mtx)
#else
# define MUTEX_LOCK(mtx) 
# define MUTEX_UNLOCK(mtx)
# define MUTEX_GUARD(mtx)
#endif 


#ifdef LINKO_LOG_THREAD
struct LogSinkImpl::Buffer {
    void *ptr;
    unsigned size;
    unsigned wrt;
    Buffer() : ptr(), size(), wrt() {}
};
#endif 


// MUST be called only once from main-thread
LogSink
LogSinkImpl::create(const std::string &dir,
                    const std::string &prog,
                    unsigned mode)
{
    return new LogSinkImpl(dir, prog, mode);
}

    
LogSinkImpl::LogSinkImpl(const std::string &dir,
                         const std::string &prog,
                         unsigned mode)
    : _fileLevel(LogAlways)
    , _mode(mode)
    , _dir(dir)
    , _prog(prog)
    , _time()
    , _tm()
    , _fd(-1)
    , _refs(0)
#   ifdef LINKO_LOG_MT
    , _mutex()
#   endif
#   ifdef LINKO_LOG_THREAD
    , _thread()
    , _cond()
    , _queue()
    , _running()
    , _terminate()
    , _free()
    , _free_lck()
#   endif
{
    if (_prog.empty()) mx_open(0);
    else rotate(std::time(0));
}


LogSinkImpl::~LogSinkImpl()
{
#   ifdef LINKO_LOG_THREAD
    stop();
#   endif
    mx_close();
}


#ifdef LINKO_LOG_THREAD
void
LogSinkImpl::start()
{
    if (_thread) return;
    _thread = new std::thread(&LogSinkImpl::run, this);
}

void
LogSinkImpl::stop()
{
    if (!_thread) return;

    // This implementation is safe to be called only from dtor
    _terminate = true;
    while (_thread->joinable() && _running) {
        _cond.notify_one();
        usleep(3);
    }
    if (_thread->joinable()) _thread->join();

    assert(_queue.empty());
    while (!_free.empty()) {
        std::free(_free.top().ptr);
        _free.pop();
    }
}

bool
LogSinkImpl::thr_write(const void* s, size_t n) 
{ 
    if (!n) return true;

    Buffer buf;
    if (FREE_SIZE) {
        std::lock_guard<spinlock> guard(_free_lck);
        if (!_free.empty()) { buf = _free.top(); _free.pop(); }
    }

    if (buf.size < n) {
        std::free(buf.ptr);
        buf.ptr = std::malloc(n);
        buf.size = n;
    }
    buf.wrt = n;
    std::memcpy(buf.ptr, s, n);

    std::lock_guard<mutex_t> guard(_mutex);
    _queue.push(buf);
    _cond.notify_one();
    return true;
}
#endif


bool
LogSinkImpl::write(const char* s, size_t n) 
{ 
#   ifdef LINKO_LOG_THREAD
    if (_thread) return thr_write(s, n);
#   endif    

#   if defined LINKO_LOG_WRITE_WO_MUTEX && defined LINKO_LOG_MT
    // Without mutex, it's much safer to always return true (success)!
    return mx_write(s, n), true;
#   else
    // It seems that mutex lock is unnecessary here!
    // Important is O_APPEND flag on open.
    // Performance cost of this lock is very big for high congestions.
    // Mutex is only important for correct rotate() operation.
    MUTEX_GUARD(_mutex);
    return mx_write(s, n);
#   endif
}


// bool
// LogSinkImpl::write_direct(char lvl, const char *prefix, const char *str)
// {
//     MUTEX_GUARD(_mutex);
//     LogTimestamp ts;
//     const std::size_t prefix_len = strlen(prefix);
//     while (*str) {
//         char buf[1024];
//         char *b = buf;
//         const char *e = buf + sizeof buf - 3;

//         assert(prefix_len + 10 < sizeof buf);
//         std::memcpy(b, prefix, prefix_len);        
//         b += prefix_len;

//         while (*str && *str != '\n' && b < e) *b++ = *str++;
//         *b++ = '\n';
//         *b++ = '\0';
//         ts.update(lvl);

//         if (!mx_print(ts, buf)) return false;
//         if (*str == '\n') ++str;
//     }
//     return true;
// }


void
LogSinkImpl::rotate(std::time_t time)
{
    if (_prog.empty()) return;
    const std::time_t minutes = time - time % (60*15); // 15min TZ resolution
    if (minutes == _time) return;

    MUTEX_GUARD(_mutex);
    if (minutes == _time) return;
    const bool print_msg = _time;
    _time = minutes;

    struct tm timeinfo;
    localtime_r(&time, &timeinfo);
    if (timeinfo.tm_year == _tm.tm_year
        && timeinfo.tm_mon == _tm.tm_mon
        && timeinfo.tm_mday == _tm.tm_mday) return;
    _tm = timeinfo;

#   ifdef LINKO_LOG_THREAD
    if (_thread) {
        _queue.push(Buffer());
        _cond.notify_one();
        return;
    }
#   endif

    mx_rotate(print_msg);
}


std::string
LogSinkImpl::filename()
{
    if (_prog.empty()) return std::string();
    MUTEX_GUARD(_mutex);
    return mx_filename();
}


std::string
LogSinkImpl::mx_filename() const
{
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y%m%d-", &_tm);
    return _dir + "/" + buffer + _prog + ".log";
}


void
LogSinkImpl::mx_rotate(bool print_msg)
{
    if (print_msg) mx_print('A', "Log file will be rotated!\n");
    const std::string name = mx_filename();
    mx_open(name.c_str());
}


// MUTEX must be hold externally
bool
LogSinkImpl::mx_write(const void* s, size_t n)
{
    ssize_t r;
    do {
        r = ::write(_fd, s, n);
    } while (r < 0 && errno == EINTR);
    return (size_t)r == n;
}


// MUTEX must be hold externally
bool
LogSinkImpl::mx_print(char lvl, const char* s)
{
    LogTimestamp ts;
    ts.update(lvl);
    return mx_print(ts, s);
}


// MUTEX must be hold externally
bool
LogSinkImpl::mx_print(const LogTimestamp &ts, const char* s)
{
    char buffer[128];
    size_t len = strlen(s);
    if (sizeof buffer < len + ts.SIZE) {
        mx_write(ts.data(), ts.SIZE);
        return mx_write(s, len); 
    } 
    memcpy(buffer, ts.data(), ts.SIZE);
    memcpy(buffer + ts.SIZE, s, len);
    return mx_write(buffer, ts.SIZE + len);
}


bool
LogSinkImpl::mx_open(const char *filename, LogLevel fLevel)
{
    level(fLevel);
    int fd = _fd;
    _fd = -1;
    if (filename) {
        _fd = ::open(filename, O_WRONLY | O_APPEND | O_CREAT, _mode);
        if (fd >= 0) ::close(fd);
    }
    else 
        _fd = 1;

    return _fd >= 0;
}


void
LogSinkImpl::mx_close()
{
    if (_fd < 0) return;
    if (!_prog.empty()) mx_print('A', "Log file will be closed!\n");
    int fd = _fd;
    _fd = -1;
    ::close(fd);
}


#ifdef LINKO_LOG_THREAD
void
LogSinkImpl::run()
{
    _running = true;
    mx_print('A', "Logging thread started\n");

    while (true) {
        std::unique_lock<mutex_t> lock(_mutex);
        while (!_terminate && _queue.empty()) _cond.wait(_mutex);
        if (_terminate && _queue.empty()) break;
        
        Buffer buf = _queue.front();
        _queue.pop();
        lock.unlock();

        if (!buf.ptr && !buf.size) mx_rotate(true);
        else {
            mx_write(buf.ptr, buf.wrt);
            if (FREE_SIZE) {
                std::lock_guard<spinlock> guard(_free_lck);
                if (_free.size() < FREE_SIZE) {
                    _free.push(buf);
                    buf.ptr = 0;
                }
            }
            if (buf.ptr) std::free(buf.ptr);
        }
    }

    mx_print('A', "Logging thread finished\n");
    _running = false;
}
#endif



//
// Convenient functions declared in log_instance.hpp
//

LogSink
LogSink_create(const std::string &dir, const std::string &prog, unsigned mode)
{
    return LogSinkImpl::create(dir, prog, mode);
}


void
start(const LogSink &sink)
{
#   ifdef LINKO_LOG_THREAD
    sink->start();
#   endif
}


LogLevel
get_level(const LogSink &sink)
{
    return sink->level();
}


void
set_level(const LogSink &sink, LogLevel level)
{
    sink->level(level);
}


void
rotate(const LogSink &sink, std::time_t time)
{
    sink->rotate(time);
}




}
