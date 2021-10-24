
//主要用来生成版本号和编译日期的输出
#ifndef _COMMON_CONFIG_H_
#define _COMMON_CONFIG_H_
#include <string>
static std::string g_SERVER_VERSION = "v0.0.1 "+ std::string(__DATE__)+" "+std::string(__TIME__)+" "+std::string(__VERSION__);
#endif
