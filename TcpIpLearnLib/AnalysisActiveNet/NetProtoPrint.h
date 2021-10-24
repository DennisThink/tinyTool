#ifndef _DENNIS_THINK_TCP_IP_NET_PROTO_PRINT_H_
#define _DENNIS_THINK_TCP_IP_NET_PROTO_PRINT_H_
#include <string>
#include "NetProtoStruct.h"
std::string MacAddrToString(const MacAddrHeader& macAddr);
std::string IpV4AddrToString(const IpV4AddrHeader& ip);
#endif