cmake_minimum_required(VERSION 3.6)
project(executorcpp)

ADD_SUBDIRECTORY(ignis)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        ignis/main.cpp
        )

#set(CMAKE_CXX_FLAGS "-O3 -fopenmp")
add_executable(executorcpp ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(executorcpp igniscore)
#TARGET_LINK_LIBRARIES(executorcpp thrift boost_system igniscore dl)

