cmake_minimum_required(VERSION 3.0)
project(EmailSender)

include_directories(/usr/include/mysql/)
#include_directories(/usr/include/curl)

include_directories(../../include/common/)
include_directories(../../include/MySQL/)
include_directories(../../include/email/)

include_directories(../../include/thirdparty/)
include_directories(../../include/thirdparty/json11/)

include_directories(../../include/thirdparty/fmt/include/)


link_libraries(mysqlclient)

SET(EMAIL_LIB  ../../include/email/SendEmail.cpp)

SET(MYSQL_LIB ../../include/MySQL/CMySql.cpp)

SET(FMT_LIB ../../include/thirdparty/fmt/src/format.cc
../../include/thirdparty/fmt/src/posix.cc)
SET(SOURCE_FILE ./EmailSenderMain.cpp 
../../include/thirdparty/json11/json11.cpp
               ${EMAIL_LIB}
               ${MYSQL_LIB}
               ${FMT_LIB})

add_executable(EmailSender ${SOURCE_FILE})
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../../bin) 