cmake_minimum_required(VERSION 2.6)
project(client)

set(SRCS
 main.cpp
 Client.h
 Client.cpp)

add_executable(client ${SRCS})
