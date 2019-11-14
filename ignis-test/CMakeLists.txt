cmake_minimum_required(VERSION 3.6)
project(ignistest)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        executor/core/IElements.h
        executor/core/IElements.tcc

        #Modules Tests
        executor/core/modules/IGeneralActionModuleTest.h
        executor/core/modules/IGeneralActionModuleTest.tcc
        executor/core/modules/IGeneralModuleTest.h
        executor/core/modules/IGeneralModuleTest.tcc
        executor/core/modules/IModuleTest.cpp
        executor/core/modules/IModuleTest.h
        executor/core/modules/IModuleTest.tcc

        #Mpi Tests
        executor/core/IMpiTest.tcc
        executor/core/IMpiTest.h

        #Partition Tests
        executor/core/storage/IDiskPartitionTest.h
        executor/core/storage/IDiskPartitionTest.tcc
        executor/core/storage/IMemoryPartitionTest.h
        executor/core/storage/IPartitionTest.tcc
        executor/core/storage/IPartitionTest.h
        executor/core/storage/IRawMemoryPartitionTest.h

        main.cpp)

set(CMAKE_CXX_FLAGS "-g -O0")

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ../ignis)

add_executable(ignistest ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(ignistest cppunit igniscore)

set (CMAKE_CXX_FLAGS "-g -O0 -rdynamic -fPIC")
add_library(testfunctions SHARED executor/TestFunctions.cpp )
