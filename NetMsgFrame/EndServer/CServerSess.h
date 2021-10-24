#ifndef _END_SERVER_C_SERVER_SESS_H_
#define _END_SERVER_C_SERVER_SESS_H_

#include "CommonMsg.h"
#include "Log.h"
#include "asio_common.h"
#include "json11.hpp"

using tcp = asio::ip::tcp;

namespace EndServer {
class CEndServer;
class CServerSess : public std::enable_shared_from_this<CServerSess> {
public:
  static std::shared_ptr<spdlog::logger> ms_loger;

private:
  CEndServer *m_server;
  tcp::socket m_socket;
  bool m_bConnect = false;
  enum { max_length = 16384, msg_max_len = 8192 };

  char m_recvbuf[max_length];
  uint32_t m_recvpos = 0;
  bool m_bForked = false;

  //接收数据
  void do_read();
  void handle_message(const TransBaseMsg_t *msg);
  

public:
  CServerSess(tcp::socket socket, CEndServer *server)
      : m_server(server), m_socket(std::move(socket)), m_bConnect(false) {}
  virtual ~CServerSess() {}
  void Start() {
    ms_loger->info("Start Receive Socket ");
    m_recvpos = 0;
    do_read();
  }

  bool IsConnected() { return m_bConnect; }

  void SendMsg(std::shared_ptr<TransBaseMsg_t> msg) {
    INFO(ms_loger, "send message ");
    auto self = shared_from_this();
    asio::async_write(
        m_socket, asio::buffer(msg->GetData(), msg->GetSize()),
        [this, self, msg](std::error_code ec, std::size_t length) {
          if (ec) {
            INFO(ms_loger, " send message error");
            // TODO:log
            m_socket.close();
            m_bConnect = false;
          } else {
            ms_loger->info("Send Succeed {}", length);
          }
        });
  }
protected:
    void handleKeepAliveReq(const KeepAliveReqMsg &reqMsg);
    void handleKeepAliveRsp(const KeepAliveRspMsg &rspMsg);
};
} // namespace EndServer
#endif
