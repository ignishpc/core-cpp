cmake_minimum_required(VERSION 3.6)
project(igniscore)

ADD_SUBDIRECTORY(data)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        driver/api/IgnisException.cpp
        driver/api/IgnisException.h
        executor/api/IContext.cpp
        executor/api/IContext.h
        executor/api/IExecutorException.cpp
        executor/api/IgnisExecutorException.h
        executor/api/ILoadClass.h
        executor/api/IMapper.h
        executor/api/IReducer.h
        executor/api/IReadIterator.h
        executor/api/IWriteIterator.h)

set(CMAKE_CXX_FLAGS "-O0 -fPIC -rdynamic")
add_library(igniscore SHARED ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(igniscore ignisdatacore thrift boost_system boost_filesystem dl thriftz z)

#install(TARGETS igniscore LIBRARY DESTINATION lib)
#install(FILES Ignis.h DESTINATION include/ignis)
#install(DIRECTORY executor/api DESTINATION include/ignis/executor FILES_MATCHING PATTERN "*.h")
#install(DIRECTORY rpc data driver DESTINATION include/ignis FILES_MATCHING PATTERN "*.h" PATTERN "*.tcc")
