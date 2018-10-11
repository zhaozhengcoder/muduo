# Install script for directory: /home/zhaozheng/code_test/muduo/muduo-master/examples

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

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/zhaozheng/code_test/muduo/build/release/examples/ace/ttcp/cmake_install.cmake")
  include("/home/zhaozheng/code_test/muduo/build/release/examples/asio/chat/cmake_install.cmake")
  include("/home/zhaozheng/code_test/muduo/build/release/examples/asio/tutorial/cmake_install.cmake")
  include("/home/zhaozheng/code_test/muduo/build/release/examples/fastcgi/cmake_install.cmake")
  include("/home/zhaozheng/code_test/muduo/build/release/examples/filetransfer/cmake_install.cmake")
  include("/home/zhaozheng/code_test/muduo/build/release/examples/hub/cmake_install.cmake")
  include("/home/zhaozheng/code_test/muduo/build/release/examples/idleconnection/cmake_install.cmake")
  include("/home/zhaozheng/code_test/muduo/build/release/examples/maxconnection/cmake_install.cmake")
  include("/home/zhaozheng/code_test/muduo/build/release/examples/memcached/client/cmake_install.cmake")
  include("/home/zhaozheng/code_test/muduo/build/release/examples/memcached/server/cmake_install.cmake")
  include("/home/zhaozheng/code_test/muduo/build/release/examples/multiplexer/cmake_install.cmake")
  include("/home/zhaozheng/code_test/muduo/build/release/examples/netty/discard/cmake_install.cmake")
  include("/home/zhaozheng/code_test/muduo/build/release/examples/netty/echo/cmake_install.cmake")
  include("/home/zhaozheng/code_test/muduo/build/release/examples/netty/uptime/cmake_install.cmake")
  include("/home/zhaozheng/code_test/muduo/build/release/examples/pingpong/cmake_install.cmake")
  include("/home/zhaozheng/code_test/muduo/build/release/examples/roundtrip/cmake_install.cmake")
  include("/home/zhaozheng/code_test/muduo/build/release/examples/shorturl/cmake_install.cmake")
  include("/home/zhaozheng/code_test/muduo/build/release/examples/simple/cmake_install.cmake")
  include("/home/zhaozheng/code_test/muduo/build/release/examples/socks4a/cmake_install.cmake")
  include("/home/zhaozheng/code_test/muduo/build/release/examples/sudoku/cmake_install.cmake")
  include("/home/zhaozheng/code_test/muduo/build/release/examples/twisted/finger/cmake_install.cmake")
  include("/home/zhaozheng/code_test/muduo/build/release/examples/wordcount/cmake_install.cmake")
  include("/home/zhaozheng/code_test/muduo/build/release/examples/zeromq/cmake_install.cmake")

endif()

