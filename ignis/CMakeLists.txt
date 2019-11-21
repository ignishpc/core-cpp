cmake_minimum_required(VERSION 3.6)
project(igniscore)

set(CMAKE_CXX_STANDARD 11)

FILE(GLOB rpc
        rpc/*.hpp
        rpc/*.cpp
        rpc/driver/*.hpp
        rpc/driver/*.cpp
        rpc/executor/*.hpp
        rpc/executor/*.cpp)

set(SOURCE_FILES
        #Driver
        driver/api/IgnisException.h
        driver/api/IgnisException.cpp

        #Executor api function
        executor/api/function/IFunction.h
        executor/api/function/IFunction0.h
        executor/api/function/IFunction2.h
        executor/api/function/IVoidFunction0.h
        executor/api/function/IVoidFunction.h
        executor/api/function/IVoidFunction2.h

        #Executor api
        executor/api/IContext.cpp
        executor/api/IContext.h
        executor/api/IContext.tcc
        executor/api/IReadIterator.h
        executor/api/IVector.h
        executor/api/IWriteIterator.h

        #Exception
        executor/core/exception/ICompatibilityException.h
        executor/core/exception/IException.h
        executor/core/exception/IException.cpp
        executor/core/exception/IInvalidArgument.h
        executor/core/exception/ILogicError.h

        #IO
        executor/core/io/IEnumTypes.h
        executor/core/io/IJsonPrinter.h
        executor/core/io/IJsonPrinter.tcc
        executor/core/io/IPrinter.h
        executor/core/io/IPrinter.tcc
        executor/core/io/IReader.h
        executor/core/io/IReader.tcc
        executor/core/io/IWriter.h
        executor/core/io/IWriter.tcc

        #Modules Impl
        executor/core/modules/impl/IBaseImpl.cpp
        executor/core/modules/impl/IBaseImpl.h
        executor/core/modules/impl/IBaseImpl.tcc
        executor/core/modules/impl/IGroupByImpl.cpp
        executor/core/modules/impl/IGroupByImpl.h
        executor/core/modules/impl/IGroupByImpl.tcc
        executor/core/modules/impl/IIOImpl.cpp
        executor/core/modules/impl/IIOImpl.h
        executor/core/modules/impl/IIOImpl.tcc
        executor/core/modules/impl/IPipeImpl.cpp
        executor/core/modules/impl/IPipeImpl.h
        executor/core/modules/impl/IPipeImpl.tcc
        executor/core/modules/impl/IReduceImpl.cpp
        executor/core/modules/impl/IReduceImpl.h
        executor/core/modules/impl/IReduceImpl.tcc
        executor/core/modules/impl/ISortImpl.cpp
        executor/core/modules/impl/ISortImpl.h
        executor/core/modules/impl/ISortImpl.tcc

        #Modules
        executor/core/modules/ICommModule.cpp
        executor/core/modules/ICommModule.h
        executor/core/modules/IExecutorServerModule.cpp
        executor/core/modules/IExecutorServerModule.h
        executor/core/modules/IGeneralActionModule.cpp
        executor/core/modules/IGeneralActionModule.h
        executor/core/modules/IGeneralModule.cpp
        executor/core/modules/IGeneralModule.h
        executor/core/modules/IIOModule.cpp
        executor/core/modules/IIOModule.h
        executor/core/modules/IModule.cpp
        executor/core/modules/IModule.h

        #Protocol
        executor/core/protocol/IObjectProtocol.h
        executor/core/protocol/IProtocol.h

        #Selector
        executor/core/selector/IGeneralActionSelector.h
        executor/core/selector/IGeneralSelector.h
        executor/core/selector/ISelector.h
        executor/core/selector/ITypeSelector.h

        #Storage
        executor/core/storage/IDiskPartition.h
        executor/core/storage/IDiskPartition.tcc
        executor/core/storage/IMemoryPartition.h
        executor/core/storage/IMemoryPartition.tcc
        executor/core/storage/IPartition.h
        executor/core/storage/IRawMemoryPartition.h
        executor/core/storage/IRawMemoryPartition.tcc
        executor/core/storage/IRawPartition.h
        executor/core/storage/IRawPartition.tcc

        #Transport
        executor/core/transport/IHeaderTransport.cpp
        executor/core/transport/IHeaderTransport.h
        executor/core/transport/IMemoryBuffer.cpp
        executor/core/transport/IMemoryBuffer.h
        executor/core/transport/ITransport.h
        executor/core/transport/IZlibTransport.cpp
        executor/core/transport/IZlibTransport.h

        #Executor core
        executor/core/IExecutorData.cpp
        executor/core/IExecutorData.h
        executor/core/ILibraryLoader.cpp
        executor/core/ILibraryLoader.h
        executor/core/ILog.cpp
        executor/core/ILog.h
        executor/core/IMpi.cpp
        executor/core/IMpi.tcc
        executor/core/IMpi.h
        executor/core/IPropertyParser.cpp
        executor/core/IPropertyParser.h
        executor/core/RTTInfo.cpp
        executor/core/RTTInfo.h

        #RPC
        ${rpc} )

add_definitions(-DBOOST_ALL_DYN_LINK)

set(CMAKE_CXX_FLAGS "-fPIC -rdynamic -pthread -fopenmp -g")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(igniscore SHARED ${SOURCE_FILES})
add_executable(ignis-cpp main.cpp ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(igniscore thrift dl backtrace thriftz z boost_log boost_thread boost_system boost_filesystem)
TARGET_LINK_LIBRARIES(ignis-cpp igniscore)

install(TARGETS igniscore LIBRARY DESTINATION lib)
install(TARGETS ignis-cpp RUNTIME DESTINATION bin)
install(DIRECTORY ../ignis
        DESTINATION include/
        FILES_MATCHING PATTERN "*.h" PATTERN "*.tcc")