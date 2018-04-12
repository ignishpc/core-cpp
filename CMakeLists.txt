cmake_minimum_required(VERSION 3.6)
project(executorcpp)

ADD_SUBDIRECTORY(ignis)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        ignis/main.cpp
        ignis/executor/core/IExecutorData.cpp
        ignis/executor/core/IExecutorData.h
        ignis/executor/core/IDinamicObject.cpp
        ignis/executor/core/IDinamicObject.h
        ignis/executor/core/modules/IModule.cpp
        ignis/executor/core/modules/IModule.h
        ignis/executor/core/modules/IServerModule.cpp
        ignis/executor/core/modules/IServerModule.h
        ignis/executor/core/modules/IMapperModule.cpp
        ignis/executor/core/modules/IMapperModule.h
        ignis/executor/core/modules/IReducerModule.cpp
        ignis/executor/core/modules/IReducerModule.h
        ignis/executor/core/modules/ISortModule.cpp
        ignis/executor/core/modules/ISortModule.h
        ignis/executor/core/modules/IPostman.h
        ignis/executor/core/modules/IPostman.cpp
        ignis/executor/core/modules/IFilesModule.h
        ignis/executor/core/modules/IFilesModule.cpp
        ignis/executor/core/storage/IObject.h
        ignis/executor/core/storage/IPostBox.h
        ignis/executor/core/storage/IMessage.h
        ignis/executor/core/storage/IPostBox.cpp
        )

#set(CMAKE_CXX_FLAGS "-O3 -fopenmp -pthread")
set(CMAKE_CXX_FLAGS "-pthread")
add_executable(executorcpp ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(executorcpp igniscore)

