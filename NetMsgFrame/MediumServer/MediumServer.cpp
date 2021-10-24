﻿#include <iostream>
#include <memory>
#include "CMediumServer.h"
#include "DaemonSvcApp.h"
#include "CServerSess.h"
#include "CommonFunction.h"
#include "asio_common.h"
#include "Log.h"
#include "planform.h"

using asio::ip::tcp;
using namespace std;

void GenerateCfgFile(std::string fileName)
{
    std::string strConfig=R"({
   "LogDir":"/home/test/Log/SourceServer/",
   "NodeId":"MediumServer",
   "server":{
        "ip":"127.0.0.1",
        "port":9000
      },
    
    "clients":[
        {
            "ip":"127,0,0,1",
            "port":8000
        },
        {
            "ip":"127.0.0.1",
            "port":9000
        }
    })";
    write_txtfile(fileName.c_str(),strConfig);
}

#ifdef WIN32
int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		PrintParamOption();
		return 0;
	}
	ParseParamResult result = ParseParam(argc, argv, GenerateCfgFile);
	if (result.m_bReturn)
	{
		return 0;
	}

	std::string strcfg, errinfo;
	load_txtfile(result.m_cfgFile.c_str(), strcfg);
	std::cout << strcfg << std::endl;
	//std::cout << strcfg << std::endl;
	if (!strcfg.length()) {
		printf("no Configure\n");
		return 1;
	}
	auto cfg = json11::Json::parse(strcfg, errinfo, json11::JsonParse::COMMENTS);

	auto logger = CreateLogger(cfg);
	if (!logger)
	{
		std::cout << "Can not Create Logger" << std::endl;
		return 0;
	}
	MediumServer::CClientSess::ms_loger = logger;
	MediumServer::CServerSess::ms_loger = logger;
	MediumServer::CMediumServer::ms_loger = logger;
	MediumServer::CClientSessManager::ms_loger = logger;

	asio::io_service IoService;
	auto server = std::make_shared<MediumServer::CMediumServer>(IoService);
	INFO(logger, "starting Server");

	std::error_code ec;
	server->loadConfig(cfg, ec);
	server->start([logger](const std::error_code& ec) {
		INFO(logger, "starting Server2");
		if (!ec)
		{
			INFO(logger, "The Service is up and running ");
		}
		else
		{
			ERR(logger, "ERRORFail to start error:{}", ec.message());
		}
	});
	IoService.run();
	return 0;
}
#else
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
    MediumServer::CClientSess::ms_loger = theApp.m_loger;
	MediumServer::CServerSess::ms_loger = theApp.m_loger;
	MediumServer::CMediumServer::ms_loger = theApp.m_loger;
	MediumServer::CClientSessManager::ms_loger = theApp.m_loger;

	WARN(theApp.m_loger,"Server: {}",g_SERVER_VERSION);
      auto worker=[cfg]()->int{     //后台任务
        auto server = std::make_shared<MediumServer::CMediumServer>(theApp.m_ioService);
        INFO(theApp.m_loger,"starting Server");

        std::error_code ec;
        server->loadConfig(cfg,ec);

        if(ec){
            ERR(theApp.m_loger,"Bad config file, {}",ec.message());
            return 1;
        }
        INFO(theApp.m_loger,"starting Server2");
        try{
            INFO(theApp.m_loger,"starting Server3");
             server->start([](const std::error_code& ec){
                INFO(theApp.m_loger,"starting Server2");
                if(!ec)
                {
					INFO(theApp.m_loger,"The Service is up and running ");
                }
                else
				{
                   ERR(theApp.m_loger,"ERRORFail to start error:{}",ec.message());
				}
            });
        }catch(exception e){
            ERR(theApp.m_loger,"ERR:Exception:{}",e.what());
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
			ERR(theApp.m_loger,"ERR:On Worker Exit{}",status);
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
#endif
