cmake_minimum_required(VERSION 3.0)
project(UnitTest)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

include_directories(/usr/include/curl)
include_directories(/usr/include/mysql/)
include_directories(../../include/common/)
include_directories(../../include/MySQL/)
include_directories(../../include/thirdparty/)
include_directories(../../include/thirdparty/spdlog/)
include_directories(../../include/thirdparty/json11/)
include_directories(../../include/thirdparty/fmt/include/)
include_directories(../../include/thirdparty/restclient_cpp/include/)



if(APPLE)
    include_directories(${PROJECT_SOURCE_DIR} "/usr/local/include" "/usr/local/ssl/include")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
    link_directories(/usr/local/lib)
else()
    include_directories(${PROJECT_SOURCE_DIR} "/usr/local/include")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -static-libgcc -static-libstdc++")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wno-unused-local-typedefs -Wno-reorder")
endif()

link_directories(/usr/local/lib64)
link_directories(/usr/lib/x86_64-linux-gnu/)
link_libraries(pthread dl)
link_directories(/usr/lib/x86_64-linux-gnu/)
link_libraries(curl mysqlclient)	
set(SERVER_FILES main.cpp
../../include/MySQL/CMySql.cpp
../../include/thirdparty/fmt/src/format.cc
../../include/thirdparty/fmt/src/posix.cc
../../include/thirdparty/json11/json11.cpp
../../include/thirdparty/restclient_cpp/source/restclient.cc
../../include/thirdparty/restclient_cpp/source/helpers.cc
../../include/thirdparty/restclient_cpp/source/connection.cc
)

SET(FMT_LEARN ./fmtLearn.cpp 
        ../../include/thirdparty/fmt/src/format.cc
        ../../include/thirdparty/fmt/src/posix.cc
        ../../include/thirdparty/json11/json11.cpp)

add_executable(UnitTest ${SERVER_FILES})
add_executable(fmtLearn ${FMT_LEARN})
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../../bin)  