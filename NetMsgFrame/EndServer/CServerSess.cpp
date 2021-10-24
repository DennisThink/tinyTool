#include "CServerSess.h"
namespace EndServer
{
std::shared_ptr<spdlog::logger> CServerSess::ms_loger;

/**
 * @brief 处理消息的总入口，根据消息类型，在此处完成消息的分发，新增消息以后，需要在此处增加对应的处理分发代码
 * 
 * @param hdr 待处理的消息
 */
void CServerSess::handle_message(const TransBaseMsg_t *hdr)
{
    INFO(ms_loger, "handle_message MsgId {}", hdr->to_string());
    if (hdr != nullptr)
    {
        switch (hdr->GetType())
        {
        case MessageType::KeepAliveReq_Type:
        {
            KeepAliveReqMsg reqMsg;
            if (reqMsg.FromString(hdr->to_string()))
            {
                handleKeepAliveReq(reqMsg);
            }
        }
        break;
        case MessageType::KeepAliveRsp_Type:
        {
            KeepAliveRspMsg rspMsg;
            if (rspMsg.FromString(hdr->to_string()))
            {
                handleKeepAliveRsp(rspMsg);
            }
        }
        break;
        default:
        {
        }
        break;
        }
    }
}

/**
 * @brief 从socket读取数据
 * 
 */
void CServerSess::do_read()
{
    INFO(ms_loger, fmt::format("Server Read Data"));
    auto self = shared_from_this();
    m_socket.async_read_some(
        asio::buffer(m_recvbuf + m_recvpos, max_length - m_recvpos),
        [this, self](std::error_code ec, std::size_t length) {
            auto curlen = m_recvpos + length;
            ms_loger->info(
                "Server Read Data Server async_read_some:size:{:#x}  ec:{} {}",
                length, ec.message(), curlen);

            const Header *head = (const Header *)m_recvbuf;
            while (curlen >= sizeof(Header) && curlen >= head->m_length)
            {
                ms_loger->info("Server  read data {}", curlen);
                TransBaseMsg_t msg(m_recvbuf);
                handle_message(&msg);
                curlen -= msg.GetSize();
                memmove(m_recvbuf, m_recvbuf + msg.GetSize(), curlen);
            }

            m_recvpos = (uint32_t)curlen;
            if (m_recvpos < max_length && !ec)
            {
                ms_loger->info("Server  read data {}", curlen);
                do_read();
            }
            else
            {
                ms_loger->info("Server  read data {}", curlen);
                m_socket.close();
                m_bConnect = false;
            }
        });
}

/**
 * @brief 处理心跳请求消息
 * 
 * @param reqMsg 心跳请求消息
 */
void CServerSess::handleKeepAliveReq(const KeepAliveReqMsg &reqMsg)
{
    INFO(ms_loger, reqMsg.ToString());
    KeepAliveRspMsg msg("EndServer");
    auto pMsg =
        std::make_shared<TransBaseMsg_t>(msg.GetMsgType(), msg.ToString());
    SendMsg(pMsg);
}

/**
 * @brief 处理心跳回复消息
 * 
 * @param rspMsg 心跳回复消息
 */
void CServerSess::handleKeepAliveRsp(const KeepAliveRspMsg &rspMsg)
{
    INFO(ms_loger, "KeepAliveRsp:{}", rspMsg.ToString());
}
} // namespace EndServer