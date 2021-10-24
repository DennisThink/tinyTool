

#ifndef DEV_CHK_CDAEMONAPP_H
#define DEV_CHK_CDAEMONAPP_H
#include <functional>
#include <string>
#include <memory>
#include <iostream>
#include <dirent.h>
#include <sys/wait.h>
#include "json11.hpp"
#include "Log.h"
#define ASIO_STANDALONE
#include <asio.hpp>
class DaemonSvcApp
{
protected:
    std::function<void(int)> m_onworker_exit=[this](int status){
        std::cout<<"Statu"<<status<<std::endl;
    };
    std::function<void(int level)> m_on_fork_error=[this](int level){
        std::cout<<"Level "<<level<<std::endl;
        sleep(10);
    };
    bool m_bStopMon=false;
    const char* m_tag;
    const char* m_errmsg;
    int m_errno;
    int set_error(int err, const char* msg){m_errno=err;m_errmsg=msg;return err;}

    int parse_proc_cmdline(pid_t pid, const std::function<void(const char*, int no)>& fn);
    int enum_instance_by_tag(const char *tag, const std::function<void(int pid, const std::string& cmdline)> &callback);
public:
    asio::io_service m_ioService;
    std::shared_ptr<spdlog::logger> m_loger;
	std::shared_ptr<spdlog::sinks::daily_file_sink_st> m_DebugSink;
	json11::Json m_configJson;
public:
    DaemonSvcApp(){}

	
    void set_tag(const char* tag){m_tag=tag;}
    void list(const std::function<void(pid_t,const std::string&)>& proc);
    DaemonSvcApp& on_fork_error(const std::function<void(int level)> &fn){m_on_fork_error=fn; return *this; }
    DaemonSvcApp& on_worker_exit(const std::function<void(int)>& fn){m_onworker_exit=fn; return *this; }
    int daemon(const std::function<int()> &workproc);
};

extern DaemonSvcApp theApp;

#endif //DEV_CHK_CDAEMONAPP_H
