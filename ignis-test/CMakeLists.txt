cmake_minimum_required(VERSION 3.6)
project(ignistest)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        main.cpp
        executor/core/storage/IObjectTest.h
        executor/core/storage/IObjectTest.cpp
        executor/core/storage/IMemoryObjectTest.h
        executor/core/storage/IRawMemoryObjectTest.h
        executor/core/modules/IModuleTest.h
        executor/core/modules/IModuleTest.cpp
        executor/core/modules/IMapperModuleTest.h
        executor/core/modules/IMapperModuleTest.cpp
        executor/core/modules/IShuffleModuleTest.h
        executor/core/modules/IShuffleModuleTest.cpp
        executor/core/modules/IPostmanModuleTest.h
        executor/core/modules/IPostmanModuleTest.cpp
        executor/core/modules/IKeysModuleTest.h
        executor/core/modules/IKeysModuleTest.cpp)

add_executable(ignistest ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(ignistest cppunit igniscore)

add_library(testfunctions SHARED executor/core/modules/TestFunctions.cpp)
SET_TARGET_PROPERTIES(testfunctions PROPERTIES LINKER_LANGUAGE CXX)
TARGET_LINK_LIBRARIES(testfunctions igniscore)


#add_test(NAME ignistest COMMAND ignistest)