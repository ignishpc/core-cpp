cmake_minimum_required(VERSION 3.6)
project(executorcpp)

ADD_SUBDIRECTORY(ignis)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        ignis/main.cpp
        ignis/executor/core/IExecutorData.cpp
        ignis/executor/core/IExecutorData.h
        ignis/executor/core/modules/IModule.cpp
        ignis/executor/core/modules/IModule.h
        ignis/executor/core/modules/IServerModule.cpp
        ignis/executor/core/modules/IServerModule.h
        ignis/executor/core/modules/IMapperModule.cpp
        ignis/executor/core/modules/IMapperModule.h
        ignis/executor/core/modules/IReducerModule.cpp
        ignis/executor/core/modules/IReducerModule.h
        ignis/executor/core/modules/ISortModule.cpp
        ignis/executor/core/modules/ISortModule.h)

#set(CMAKE_CXX_FLAGS "-O3 -fopenmp")
add_executable(executorcpp ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(executorcpp igniscore dl)
#TARGET_LINK_LIBRARIES(executorcpp thrift boost_system igniscore dl)

