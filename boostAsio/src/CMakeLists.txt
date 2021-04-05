cmake_minimum_required(VERSION 2.8)

project(boostAsio C CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

set(CXX_FLAGS
 -g
 # -DVALGRIND
 -DCHECK_PTHREAD_RETURN_VALUE
 -D_FILE_OFFSET_BITS=64
 -Wall
 -Wextra
 #-Werror
 -Wconversion
 # -Wno-unused-parameter
 -Wold-style-cast
 -Woverloaded-virtual
 -Wpointer-arith
 -Wshadow
 -Wwrite-strings
 -march=native
 # -MMD
 -std=c++11
 -rdynamic
 -pthread   # check
 )
if(CMAKE_BUILD_BITS EQUAL 32)
  list(APPEND CXX_FLAGS "-m32")
endif()
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  list(APPEND CXX_FLAGS "-Wno-null-dereference")
  list(APPEND CXX_FLAGS "-Wno-sign-conversion")
  list(APPEND CXX_FLAGS "-Wno-unused-local-typedef")
  list(APPEND CXX_FLAGS "-Wthread-safety")
  list(REMOVE_ITEM CXX_FLAGS "-rdynamic")
endif()
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
message(STATUS "CXX_FLAGS = " ${CMAKE_CXX_FLAGS} " " ${CMAKE_CXX_FLAGS_${BUILD_TYPE}})

find_package(Boost REQUIRED COMPONENTS system thread)

INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

ADD_EXECUTABLE( 01_syn_timer 01_syn_timer.cc )
TARGET_LINK_LIBRARIES( 01_syn_timer LINK_PUBLIC ${Boost_LIBRARIES} )

ADD_EXECUTABLE( 02_async_timer 02_async_timer.cc )
TARGET_LINK_LIBRARIES( 02_async_timer LINK_PUBLIC ${Boost_LIBRARIES} )

ADD_EXECUTABLE( 03_bind_arg_handler 03_bind_arg_handler.cc )
TARGET_LINK_LIBRARIES( 03_bind_arg_handler LINK_PUBLIC ${Boost_LIBRARIES} )

ADD_EXECUTABLE( 04_mem_handler 04_mem_handler.cc )
TARGET_LINK_LIBRARIES( 04_mem_handler LINK_PUBLIC ${Boost_LIBRARIES} )

ADD_EXECUTABLE( 05_sync_handler_mutlti 05_sync_handler_mutlti.cc )
TARGET_LINK_LIBRARIES( 05_sync_handler_mutlti LINK_PUBLIC ${Boost_LIBRARIES} )

ADD_EXECUTABLE( 06_sync_tcpServ 06_sync_tcpServ.cc )
TARGET_LINK_LIBRARIES( 06_sync_tcpServ LINK_PUBLIC ${Boost_LIBRARIES} )

ADD_EXECUTABLE( 07_echo_server_async 07_echo_server_async.cc )
TARGET_LINK_LIBRARIES( 07_echo_server_async LINK_PUBLIC ${Boost_LIBRARIES} )


