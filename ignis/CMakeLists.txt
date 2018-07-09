cmake_minimum_required(VERSION 3.6)
project(igniscore)

set(CMAKE_CXX_STANDARD 11)

FILE(GLOB rpc
        rpc/*.hpp
        rpc/*.cpp
        rpc/driver/*.hpp
        rpc/driver/*.cpp )

set(SOURCE_FILES
        #Data
        data/RTTInfo.h
        data/RTTInfo.cpp
        data/IManager.h
        data/IObjectProtocol.h
        data/IOperators.h
        data/IFileTransport.h
        data/IFileTransport.cpp
        data/serialization/IEnumTypes.h
        data/serialization/IPrinter.h
        data/serialization/IDeleter.h
        data/serialization/IWriter.h
        data/serialization/IReader.h
        data/serialization/ITypeHandle.h
        data/serialization/IHash.h
        data/serialization/IClassManager.h

        #Executor api
        executor/api/IReadIterator.h
        executor/api/IWriteIterator.h
        executor/api/IReducer.h
        executor/api/IContext.h
        executor/api/IContext.cpp
        executor/api/IRegister.h
        executor/api/Iterable.h
        executor/api/function/IFunction.h
        executor/api/function/IFlatFunction.h
        executor/api/function/IFunction2.h

        #Driver
        driver/api/IgnisException.h
        driver/api/IgnisException.cpp

        #Exception
        exceptions/IException.h
        exceptions/IException.cpp
        exceptions/ILogicError.h
        exceptions/IInvalidArgument.h
        #RPC
        ${rpc} )

set(CMAKE_CXX_FLAGS "-fPIC -rdynamic")#-O3
add_library(igniscore SHARED ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(igniscore jsoncpp thrift boost_system boost_filesystem dl thriftz z)

install(TARGETS igniscore LIBRARY DESTINATION lib)
install(DIRECTORY data driver exceptions DESTINATION include/ignis FILES_MATCHING PATTERN "*.h" PATTERN "*.tcc")
install(DIRECTORY executor/api DESTINATION include/ignis/executor FILES_MATCHING PATTERN "*.h" PATTERN "*.tcc")
