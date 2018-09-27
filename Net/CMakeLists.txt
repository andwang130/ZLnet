cmake_minimum_required(VERSION 3.10)
project(Net)

set(CMAKE_CXX_STANDARD 11)

add_executable(Net main.cpp Acceptor.cpp Acceptor.h Mboost.cpp Mboost.h Socket.cpp Socket.h TcpServer.cpp TcpServer.h)