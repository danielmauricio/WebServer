cmake_minimum_required(VERSION 3.2)
project(WebServer)

set (CMAKE_C_FLAGS "-pthread")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")
set(SOURCE_FILES main.c)
add_executable(WebServer ${SOURCE_FILES})