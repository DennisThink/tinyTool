cmake_minimum_required(VERSION 3.0)
project(WeatherSpider)

include_directories(/usr/include/mysql/)
include_directories(/usr/include/curl)

include_directories(../../include/common/)
include_directories(../../include/MySQL/)
include_directories(../../include/Spider/)

include_directories(../../include/thirdparty/)
include_directories(../../include/thirdparty/asio/)
include_directories(../../include/thirdparty/spdlog/)
include_directories(../../include/thirdparty/json11/)
include_directories(../../include/thirdparty/fmt/include/)
include_directories(../../include/thirdparty/restclient_cpp/include/)

link_directories(/usr/lib/x86_64-linux-gnu/)


if(APPLE)
    include_directories(${PROJECT_SOURCE_DIR} "/usr/local/include" "/usr/local/ssl/include")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wno-variadic-macros")
    link_directories(/usr/local/lib)
else()
    include_directories(${PROJECT_SOURCE_DIR} "/usr/local/include")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -static-libgcc -static-libstdc++")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wno-variadic-macros -Wno-implicit-fallthrough -Wno-unused-local-typedefs")
endif()


link_libraries(curl)
link_libraries(mysqlclient)
SET(SPIDER_SOURCE ./WeatherSpider.cpp 

../../include/MySQL/CMySql.cpp 
../../include/Spider/CWeatherSpider.cpp
../../include/thirdparty/restclient_cpp/source/restclient.cc
../../include/thirdparty/restclient_cpp/source/helpers.cc
../../include/thirdparty/restclient_cpp/source/connection.cc
../../include/thirdparty/fmt/src/format.cc
../../include/thirdparty/fmt/src/posix.cc
../../include/thirdparty/json11/json11.cpp
)

SET(CITY_CODE_SOURCE ./GetCityCode.cpp
../../include/thirdparty/restclient_cpp/source/restclient.cc
../../include/thirdparty/restclient_cpp/source/helpers.cc
../../include/thirdparty/restclient_cpp/source/connection.cc
../../include/thirdparty/fmt/src/format.cc
../../include/thirdparty/fmt/src/posix.cc)

add_executable(WeatherSpider ${SPIDER_SOURCE})
add_executable(GetCityCode ${CITY_CODE_SOURCE})
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../../bin) 