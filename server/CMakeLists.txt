cmake_minimum_required(VERSION 2.6)
project(server)

set(SRCS
 main.cpp
 Server.h
 Server.cpp)

add_executable(server ${SRCS})
