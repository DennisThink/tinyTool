cmake_minimum_required(VERSION 3.0)
project(MediumServer)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
include_directories(../include/thirdparty/)
include_directories(../include/thirdparty/asio/)
include_directories(../include/thirdparty/spdlog/)
include_directories(../include/thirdparty/json11/)
include_directories(../include/thirdparty/fmt/include/)
include_directories(../include/common/)
include_directories(../include/DaemonApp/)
if(APPLE)
    include_directories(${PROJECT_SOURCE_DIR} "/usr/local/include" "/usr/local/ssl/include")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
    link_directories(/usr/local/lib)
elseif(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 ")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /WX")
else()
    include_directories(${PROJECT_SOURCE_DIR} "/usr/local/include")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -static-libgcc -static-libstdc++")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
endif()

if(WIN32)
else()
link_libraries(pthread)
endif()
set(SERVER_FILES 
		MediumServer.cpp
		CServerSess.h CServerSess.cpp
		CPushQueue.h CPushQueue.cpp
        CMediumServer.h CMediumServer.cpp 
        ../include/thirdparty/json11/json11.cpp
        CClientSess.h CClientSess.cpp 
        ../include/DaemonApp/DaemonSvcApp.h 
        ../include/DaemonApp/DaemonSvcApp.cpp
        #../include/thirdparty/fmt/src/format.cc
        #../include/thirdparty/fmt/src/posix.cc
        )
add_executable(MediumServer ${SERVER_FILES})
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin)  