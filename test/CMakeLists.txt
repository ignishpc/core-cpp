cmake_minimum_required(VERSION 3.6)
project(ignistest)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        ignis/executor/core/IElements.h
        ignis/executor/core/IElements.tcc

        #Modules Tests
        ignis/executor/core/modules/IGeneralActionModuleTest.h
        ignis/executor/core/modules/IGeneralActionModuleTest.tcc
        ignis/executor/core/modules/IGeneralModuleTest.h
        ignis/executor/core/modules/IGeneralModuleTest.tcc
        ignis/executor/core/modules/IIOModuleTest.h
        ignis/executor/core/modules/IIOModuleTest.tcc
        ignis/executor/core/modules/IIOModuleTest.cpp
        ignis/executor/core/modules/IModuleTest.cpp
        ignis/executor/core/modules/IModuleTest.h
        ignis/executor/core/modules/IModuleTest.tcc

        #Mpi Tests
        ignis/executor/core/IMpiTest.tcc
        ignis/executor/core/IMpiTest.h

        #Partition Tests
        ignis/executor/core/storage/IDiskPartitionTest.h
        ignis/executor/core/storage/IDiskPartitionTest.tcc
        ignis/executor/core/storage/IMemoryPartitionTest.h
        ignis/executor/core/storage/IPartitionTest.tcc
        ignis/executor/core/storage/IPartitionTest.h
        ignis/executor/core/storage/IRawMemoryPartitionTest.h

        main.cpp)

set(CMAKE_CXX_FLAGS "-g -O0")

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ../src)

add_executable(ignistest ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(ignistest cppunit igniscore)

set(CMAKE_CXX_FLAGS "-g -O0 -rdynamic -fPIC")
add_library(testfunctions SHARED ignis/executor/TestFunctions.cpp)
