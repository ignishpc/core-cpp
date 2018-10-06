cmake_minimum_required(VERSION 3.6)
project(ignistest)


set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        main.cpp
        executor/core/storage/IMemoryObjectTest.h
        executor/core/storage/IMemoryObjectTest.cpp )

add_executable(ignistest ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(ignistest cppunit)
#add_test(NAME ignistest COMMAND ignistest)