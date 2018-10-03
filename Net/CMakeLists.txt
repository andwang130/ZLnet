cmake_minimum_required(VERSION 3.10)
project(Net)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -pthread")#多线程需要的参数
add_executable(Net main.cpp Acceptor.cpp Acceptor.h Mboost.cpp Mboost.h Socket.cpp Socket.h TcpServer.cpp TcpServer.h Types.h StringPiece.h EventloopThread.cpp EventloopThread.h CurrentThread.h Eventloop.cpp Channel.cpp Epollpoller.cpp CurrentThread.h EventloopThreadpool.cpp EventloopThreadpool.h CallbackS.h)