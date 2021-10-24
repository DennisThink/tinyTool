
#ifndef _COMMON_LOG_H_
#define _COMMON_LOG_H_
#include <spdlog/spdlog.h>
#include <spdlog/sinks/sink.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/syslog_sink.h>
#define INFO(LOG,msg...) LOG->info(msg)
#define WARN(LOG,msg...) LOG->warn(msg)
#define ERROR(LOG,msg...)  LOG->error(msg)
#define DBG(LOG,msg...) LOG->debug(msg)
#endif