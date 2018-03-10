cmake_minimum_required(VERSION 3.6)
project(igniscore)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        data/serialization/IHash.h
        data/serialization/IHandle.h
        data/serialization/IContainer.h
        data/serialization/IType.h
        data/serialization/IReader.h
        data/serialization/IReader.cpp
        data/serialization/IWriter.h
        data/serialization/IWriter.cpp
        data/IObjectProtocol.cpp
        data/IObjectProtocol.h
        )
set(CMAKE_CXX_FLAGS "-O0 -fPIC -rdynamic -Wa,-mbig-obj")
add_library(igniscore SHARED ${SOURCE_FILES})
#TARGET_LINK_LIBRARIES(igniscore thrift boost_system boost_filesystem dl thriftz z)

#install(TARGETS igniscore LIBRARY DESTINATION lib)
#install(FILES Ignis.h DESTINATION include/ignis)
#install(DIRECTORY executor/api DESTINATION include/ignis/executor FILES_MATCHING PATTERN "*.h")
#install(DIRECTORY rpc data driver DESTINATION include/ignis FILES_MATCHING PATTERN "*.h" PATTERN "*.tcc")
