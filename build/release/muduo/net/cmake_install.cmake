# Install script for directory: /home/zhaozheng/code_test/muduo/muduo-master/muduo/net

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/zhaozheng/code_test/muduo/build/release-install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/zhaozheng/code_test/muduo/build/release/lib/libmuduo_net.a")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/zhaozheng/code_test/muduo/build/release/lib/libmuduo_net_cpp11.a")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/muduo/net" TYPE FILE FILES
    "/home/zhaozheng/code_test/muduo/muduo-master/muduo/net/Buffer.h"
    "/home/zhaozheng/code_test/muduo/muduo-master/muduo/net/Callbacks.h"
    "/home/zhaozheng/code_test/muduo/muduo-master/muduo/net/Channel.h"
    "/home/zhaozheng/code_test/muduo/muduo-master/muduo/net/Endian.h"
    "/home/zhaozheng/code_test/muduo/muduo-master/muduo/net/EventLoop.h"
    "/home/zhaozheng/code_test/muduo/muduo-master/muduo/net/EventLoopThread.h"
    "/home/zhaozheng/code_test/muduo/muduo-master/muduo/net/EventLoopThreadPool.h"
    "/home/zhaozheng/code_test/muduo/muduo-master/muduo/net/InetAddress.h"
    "/home/zhaozheng/code_test/muduo/muduo-master/muduo/net/TcpClient.h"
    "/home/zhaozheng/code_test/muduo/muduo-master/muduo/net/TcpConnection.h"
    "/home/zhaozheng/code_test/muduo/muduo-master/muduo/net/TcpServer.h"
    "/home/zhaozheng/code_test/muduo/muduo-master/muduo/net/TimerId.h"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/zhaozheng/code_test/muduo/build/release/muduo/net/http/cmake_install.cmake")
  include("/home/zhaozheng/code_test/muduo/build/release/muduo/net/inspect/cmake_install.cmake")
  include("/home/zhaozheng/code_test/muduo/build/release/muduo/net/tests/cmake_install.cmake")

endif()

