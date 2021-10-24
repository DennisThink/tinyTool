#ifndef _DENNIS_THINK_TCP_IP_NET_PROTO_STRUCT_H_
#define _DENNIS_THINK_TCP_IP_NET_PROTO_STRUCT_H_
//Mac地址头部
struct MacAddrHeader
{
    unsigned char mDestMacAddr[6];
    unsigned char mSrcMacAddr[6];
    unsigned short m_type;
};


//IP地址头部
struct IpV4AddrHeader
{
    unsigned char m_VersionHeaderLength;
    unsigned char m_Tos;
    unsigned short m_totalLength;
    
    unsigned short m_identification;
    unsigned short m_flagsFragmentOffSet;
    
    unsigned char  m_ttl;
    unsigned char  m_proto;
    unsigned short  m_headerCheckSum;

    unsigned int  m_srcIpAddr;
    
    unsigned int  m_dstIpAddr;
};
#endif