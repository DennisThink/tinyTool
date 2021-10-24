//
// Created by 王耀华 on 16/10/10.
//

#include "CHttpServer.h"

namespace SourceServer
{
    std::shared_ptr<spdlog::logger> CHttpServer::ms_loger;

    void CHttpServer::OnTimer()
    {
        INFO(ms_loger, "{0} {1} On Timer at CHttpServer", __FILE__, __LINE__);
    }

    void CHttpServer::InitHandler()
    {
        auto self = shared_from_this();

        mux.handle("/customers/{id}")
		.get([self,this](served::response & res, const served::request & req) {
			(void) res;
			(void) req;
			// read customer req.params["id"]
		})
		.put([](served::response & res, const served::request & req) {
			(void) res;
			(void) req;
			// update customer req.params["id"]
		})
		.del([](served::response & res, const served::request & req) {
			(void) res;
			(void) req;
			// delete customer req.params["id"]
		});

    }
} // namespace SourceServer