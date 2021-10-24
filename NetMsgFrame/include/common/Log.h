
#ifndef __LOG_H_
#define __LOG_H_
#include <spdlog/spdlog.h>
#include <spdlog/sinks/sink.h>
#include <spdlog/sinks/daily_file_sink.h>
//#include <spdlog/sinks/syslog_sink.h>
#ifdef WIN32
#define INFO(LOG,...) LOG->info(__VA_ARGS__)
#define WARN(LOG,...) LOG->warn(__VA_ARGS__)
#define ERR(LOG,...)  LOG->error(__VA_ARGS__)
#define DBG(LOG,...) LOG->debug(__VA_ARGS__)
#else
#define INFO(LOG,msg...) LOG->info(msg)
#define WARN(LOG,msg...) LOG->warn(msg)
#define ERR(LOG,msg...)  LOG->error(msg)
#define DBG(LOG,msg...) LOG->debug(msg)
#endif

#endif