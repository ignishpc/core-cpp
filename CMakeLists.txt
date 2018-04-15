cmake_minimum_required(VERSION 3.6)
project(ignis)

ADD_SUBDIRECTORY(ignis)

set(CMAKE_CXX_STANDARD 11)


FILE(GLOB rpc ignis/rpc/executor/*.hpp ignis/rpc/executor/*.cpp)

set(SOURCE_FILES
        ignis/main.cpp
        ignis/executor/core/ILog.h
        ignis/executor/core/ILog.cpp
        ignis/executor/core/IExecutorData.h
        ignis/executor/core/IExecutorData.cpp
        ignis/executor/core/IDinamicObject.h
        ignis/executor/core/IDinamicObject.cpp
        ignis/executor/core/IPropertiesParser.h
        ignis/executor/core/IPropertiesParser.cpp
        ignis/executor/core/modules/IModule.h
        ignis/executor/core/modules/IModule.cpp
        ignis/executor/core/modules/IServerModule.h
        ignis/executor/core/modules/IServerModule.cpp
        ignis/executor/core/modules/IMapperModule.h
        ignis/executor/core/modules/IMapperModule.cpp
        ignis/executor/core/modules/IReducerModule.h
        ignis/executor/core/modules/IReducerModule.cpp
        ignis/executor/core/modules/ISortModule.h
        ignis/executor/core/modules/ISortModule.cpp
        ignis/executor/core/modules/IPostmanModule.h
        ignis/executor/core/modules/IPostmanModule.cpp
        ignis/executor/core/modules/IFilesModule.h
        ignis/executor/core/modules/IFilesModule.cpp
        ignis/executor/core/modules/IStorageModule.h
        ignis/executor/core/modules/IStorageModule.cpp
        ignis/executor/core/storage/IPostBox.h
        ignis/executor/core/storage/IMessage.h
        ignis/executor/core/storage/IPostBox.cpp
        ignis/executor/core/storage/IObject.h
        ignis/executor/core/storage/IRawObject.h
        ignis/executor/core/storage/IRawObject.cpp
        ignis/executor/core/storage/IRawMemoryObject.h
        ignis/executor/core/storage/IRawMemoryObject.cpp
        ignis/executor/core/storage/ITransportIterator.h
        ignis/executor/core/storage/ITransportIterator.cpp
        ${rpc}
        )

set(CMAKE_CXX_FLAGS "-pthread -DBOOST_ALL_DYN_LINK")#-O3 -fopenmp
add_executable(igniscpp ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(igniscpp igniscore boost_log boost_thread)

