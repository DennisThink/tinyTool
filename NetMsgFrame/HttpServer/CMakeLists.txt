cmake_minimum_required(VERSION 3.0)
project(HttpToTcpServerDemo)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
include_directories(../include/thirdparty/)
include_directories(../include/thirdparty/asio/)
include_directories(../include/thirdparty/spdlog/)
include_directories(../include/thirdparty/json11/)
include_directories(../include/thirdparty/fmt/include/)
include_directories(../include/common/)
include_directories(../include/DaemonApp/)
include_directories(./other/)
if(APPLE)
    include_directories(${PROJECT_SOURCE_DIR} "/usr/local/include" "/usr/local/ssl/include")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wno-variadic-macros")
    link_directories(/usr/local/lib)
elseif(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 ")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /WX")
else()
    include_directories(${PROJECT_SOURCE_DIR} "/usr/local/include")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -static-libgcc -static-libstdc++")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
endif()

link_directories(/usr/local/lib64)
link_libraries(pthread dl)
	
set(SERVER_FILES 
		Server.cpp
		CSessManager.h CSessManager.cpp
        CSourceServer.cpp CSourceServer.h
        ../include/thirdparty/json11/json11.cpp
        CAPNsSess.cpp CAPNsSess.h
        CHttpServer.h CHttpServer.cpp
        ../include/DaemonApp/DaemonSvcApp.h 
        ../include/DaemonApp/DaemonSvcApp.cpp
        ../include/thirdparty/fmt/src/format.cc
        ../include/thirdparty/fmt/src/posix.cc
        )
SET(THIRD_SERVED_FILES 

./other/served/mux/empty_matcher.hpp
./other/served/mux/matchers.hpp
./other/served/mux/regex_matcher.cpp
./other/served/mux/regex_matcher.hpp
./other/served/mux/segment_matcher.hpp
./other/served/mux/static_matcher.cpp
./other/served/mux/static_matcher.hpp
./other/served/mux/variable_matcher.cpp
./other/served/mux/variable_matcher.hpp


./other/served/net/connection.cpp
./other/served/net/connection.hpp
./other/served/net/connection_manager.cpp
./other/served/net/connection_manager.hpp
./other/served/net/server.cpp
./other/served/net/server.hpp


./other/served/plugins/access_log.cpp


./other/served/methods_handler.cpp
./other/served/methods_handler.hpp
./other/served/methods.hpp
./other/served/multiplexer.cpp
./other/served/multiplexer.hpp
./other/served/parameters.cpp
./other/served/parameters.hpp

./other/served/plugins.hpp
./other/served/request.cpp
./other/served/request_error.hpp
./other/served/request.hpp
./other/served/request_parser.cpp
./other/served/request_parser.hpp
./other/served/request_parser_impl.cpp
./other/served/request_parser_impl.hpp
#./other/served/request_parser.rl

./other/served/response.cpp
./other/served/response.hpp
./other/served/served.hpp
./other/served/status.cpp
./other/served/status.hpp
./other/served/uri.cpp
./other/served/uri.hpp
./other/served/version.hpp
                 )
SET(ALL_SOURCE_FILE ${THIRD_SERVED_FILES} ${SERVER_FILES})
add_executable(HttpToTcpServer ${ALL_SOURCE_FILE})
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin) 
