// 
// Copyright (C) 2013 Karol Roslaniec <llog@roslaniec.net>
// 
#ifndef LINKO_LOG_TIMESTAMP_HPP
#define LINKO_LOG_TIMESTAMP_HPP

#include <ctime>
#include <cstring>
#include <sys/time.h>

namespace linko {

class LogTimestamp
{
public:
    static const unsigned SIZE = 24;

    inline LogTimestamp();

    inline void update(char lvl);

    const char *data() const { return _buffer; }

    inline char *copy(char *dst);

private:
    time_t _time; // time in _buffer
    char _buffer[SIZE + 1];
};


LogTimestamp::LogTimestamp()
    : _time()
{
    std::strcpy(_buffer, "YYMMDD hhmmss.uuuuuu ?: "); 
}


char *
LogTimestamp::copy(char *dst)
{
    std::memcpy(dst, _buffer, SIZE);
    return dst + SIZE;
}

void
LogTimestamp::update(char lvl)
{
    struct timeval tv;
    if (0 != gettimeofday(&tv, NULL)) 
        return;

    _buffer[21] = lvl;

    // First update microseconds part (loop unrolled)
    _buffer[19] = '0' + tv.tv_usec % 10;
    _buffer[18] = '0' + tv.tv_usec/10 % 10;
    _buffer[17] = '0' + tv.tv_usec/100 % 10;
    _buffer[16] = '0' + tv.tv_usec/1000 % 10;
    _buffer[15] = '0' + tv.tv_usec/10000 % 10;
    _buffer[14] = '0' + tv.tv_usec/100000 % 10;

    if (_time == tv.tv_sec) 
	return;
    
    if (!_time || tv.tv_sec/900 != _time/900) { // assume 15min TZ resolution
        using namespace std;
        struct tm result;
        const time_t t = tv.tv_sec; // Windows hack
	strftime(_buffer, 10, "%y%m%d %H", localtime_r(&t, &result));
    }

    // hours cannot be calculated because of timezones!
    _time = tv.tv_sec;
    _buffer[12] = '0' + tv.tv_sec % 10;
    _buffer[11] = '0' + tv.tv_sec/10 % 6;
    _buffer[10] = '0' + tv.tv_sec/60 % 10;
    _buffer[ 9] = '0' + tv.tv_sec/600 % 6;
}


}
#endif
