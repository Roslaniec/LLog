// 
// Copyright (C) 2013 Karol Roslaniec <llog@roslaniec.net>
// 
#ifndef LINKO_LOG_CFG_HPP
#define LINKO_LOG_CFG_HPP

/*
 * Enable log multi-threading
 */
#define LINKO_LOG_MT 

#define LINKO_THREAD_LOCAL __thread

/*
 * There is big benefit of disabling WRITE_MUTEX in case of
 * log congestions, ie. if many threads try to log at the same moment.
 * But in practice, log congestions are rather rare and for single thread
 * the cost of mutex is small (~0.5%).
 * Without this mutex log rotation might be incorrect (eg. missing lines).
 */ 
// #define LINKO_LOG_WRITE_WO_MUTEX

#define LINKO_LOG_BUFFER_LENGTH 8192

/*
 * LogSink will be writting to separate thread
 */
#define LINKO_LOG_THREAD

#endif
