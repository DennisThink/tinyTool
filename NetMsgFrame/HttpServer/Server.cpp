#include <iostream>
#include <memory>
#ifndef ASIO_STANDALONE
#define ASIO_STANDALONE
#endif
#include "CommonFunction.h"
#include "CSourceServer.h"
#include "DaemonSvcApp.h"
#include "CommonConfig.h"
#include <asio/high_resolution_timer.hpp>
#include "Log.h"


using asio::ip::tcp;
using namespace std;
void GenerateCfgFile(std::string fileName)
{
    std::string strConfig=R"({
   "LogDir":"/home/test/Log/SourceServer/",
   "server":{
        "ip":"127.0.0.1",
        "port":9000
      }
    })";
    write_txtfile(fileName.c_str(),strConfig);
}




int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		PrintParamOption();
		return 0;
	}
    ParseParamResult result = ParseParam(argc,argv,GenerateCfgFile);
    if(result.m_bReturn)
    {
        return 0;
    }
    theApp.set_tag(result.m_strTag.c_str());

    theApp.list([result](pid_t pid, const std::string &cmdline) {
        printf("\033[30m[%d]\033[32m %s \033[36m", pid, cmdline.c_str());
        if (result.m_cmd == ServerCmd::CMD_STOP) {
			INFO(theApp.m_loger,"terminating....");
            //printf(" terminating...");
            auto v = kill(pid, SIGKILL);
            if (v == EPERM) {
                printf("\033[31mfail! no permission.");
                //INFO(theApp.m_loger,"\033[31mfail! no permission.");
                //flgs&=~1;
            } else
				printf("done \n");
				//INFO(theApp.m_loger,"done ");
        } else
            //flgs&=~1;
			printf("done \n");
    });

    std::string strcfg,errinfo;
    load_txtfile(result.m_cfgFile.c_str(), strcfg);
    if(!strcfg.length()){
		printf("no Configure\n");
        return 1;
    }
    auto cfg=json11::Json::parse(strcfg,errinfo,json11::JsonParse::COMMENTS);

    auto logger = CreateLogger(cfg);
    if(!logger)
    {
        std::cout<<"Can not Create Logger"<<std::endl;
        return 0;
    }
	theApp.m_loger=logger;
	theApp.m_loger->set_level(spdlog::level::debug);
   	SourceServer::CAPNsSess::ms_loger = theApp.m_loger;
	SourceServer::CSourceServer::ms_loger = theApp.m_loger;
	SourceServer::CSessManager::ms_loger = theApp.m_loger;

	WARN(theApp.m_loger,"Server: {}",g_SERVER_VERSION);
      auto worker=[cfg]()->int{     //后台任务
        SourceServer::CSourceServer server(theApp.m_ioService);
        INFO(theApp.m_loger,"starting Server");

        std::error_code ec;
        server.loadConfig(cfg["server"],ec);

        if(ec){
            ERROR(theApp.m_loger,"Bad config file, {}",ec.message());
            return 1;
        }
        INFO(theApp.m_loger,"starting Server2");
        try{
            INFO(theApp.m_loger,"starting Server3");
            server.start([](const std::error_code& ec){
                INFO(theApp.m_loger,"starting Server2");
                if(!ec)
                {
					INFO(theApp.m_loger,"The Service is up and running ");
                }
                else
				{
                   ERROR(theApp.m_loger,"ERRORFail to start error:{}",ec.message());
				}
            });
        }catch(exception e){
            ERROR(theApp.m_loger,"ERROR:Exception:{}",e.what());
        }
        theApp.m_ioService.run();
        return 0;
    };
    if(result.m_cmd == ServerCmd::CMD_START){
        auto rstm=time(nullptr)+10;  //最快10秒启动一次，防止意外条件下频繁重启
        theApp.on_worker_exit([&rstm](int status){
           //TODO: 检查子进程退出的原因，有些情况下是不需要重启的	
			if(BIND_FAILED_EXIT == (status>>8) && (0 == (status&0xff)))
			{
				WARN(theApp.m_loger,"Bind Failed ");
				exit(1);
			}
			ERROR(theApp.m_loger,"ERROR:On Worker Exit{}",status);
            auto tmnow=time(nullptr);
            if(tmnow<rstm){
                sleep(rstm-tmnow);
            }
			rstm=time(nullptr)+10;
        }).on_fork_error([](int level){
			WARN(theApp.m_loger,"on fork error :{}",level);
            sleep(10);
        }).daemon(worker);
    }
    return 0;
}
