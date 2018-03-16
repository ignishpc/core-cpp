cmake_minimum_required(VERSION 3.6)
project(ignisdatacore)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        serialization/IHash.h
        serialization/IHandle.h
        serialization/IContainer.h
        serialization/IExtendedType.h
        serialization/IEnumTypes.h
        serialization/IReader.h
        serialization/IReader.cpp
        serialization/IWriter.h
        serialization/IWriter.cpp
        ITypeInfo.h
        RTTInfo.cpp
        RTTInfo.h
        IObjectProtocol.h
        IObjectProtocol.cpp)
set(CMAKE_CXX_FLAGS "-O0 -fPIC -rdynamic")
add_library(ignisdatacore SHARED ${SOURCE_FILES})
#TARGET_LINK_LIBRARIES(igniscore thrift boost_system boost_filesystem dl thriftz z)

#install(TARGETS igniscore LIBRARY DESTINATION lib)
#install(FILES Ignis.h DESTINATION include/ignis)
#install(DIRECTORY executor/api DESTINATION include/ignis/executor FILES_MATCHING PATTERN "*.h")
#install(DIRECTORY rpc data driver DESTINATION include/ignis FILES_MATCHING PATTERN "*.h" PATTERN "*.tcc")
