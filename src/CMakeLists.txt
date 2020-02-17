cmake_minimum_required(VERSION 3.6)
project(igniscore)

set(CMAKE_CXX_STANDARD 11)

FILE(GLOB rpc
        ignis/rpc/*.hpp
        ignis/rpc/*.cpp
        ignis/rpc/driver/*.hpp
        ignis/rpc/driver/*.cpp
        ignis/rpc/executor/*.hpp
        ignis/rpc/executor/*.cpp)

set(SOURCE_FILES
        #Driver api
        ignis/driver/api/ICluster.cpp
        ignis/driver/api/ICluster.h
        ignis/driver/api/IDataFrame.cpp
        ignis/driver/api/IDataFrame.h
        ignis/driver/api/Ignis.cpp
        ignis/driver/api/Ignis.h
        ignis/driver/api/IDriverException.cpp
        ignis/driver/api/IDriverException.h
        ignis/driver/api/IProperties.cpp
        ignis/driver/api/IProperties.h
        ignis/driver/api/ISource.h
        ignis/driver/api/IWorker.cpp
        ignis/driver/api/IWorker.h

        #Driver core
        ignis/driver/core/ICallBack.cpp
        ignis/driver/core/ICallBack.h
        ignis/driver/core/IClient.cpp
        ignis/driver/core/IClient.h
        ignis/driver/core/IClientPool.cpp
        ignis/driver/core/IClientPool.h
        ignis/driver/core/IDriverContext.cpp
        ignis/driver/core/IDriverContext.h
        ignis/driver/core/IDriverContext.tcc

        #Executor api function
        ignis/executor/api/function/IFunction.h
        ignis/executor/api/function/IFunction0.h
        ignis/executor/api/function/IFunction2.h
        ignis/executor/api/function/IVoidFunction0.h
        ignis/executor/api/function/IVoidFunction.h
        ignis/executor/api/function/IVoidFunction2.h

        #Executor api
        ignis/executor/api/IContext.cpp
        ignis/executor/api/IContext.h
        ignis/executor/api/IContext.tcc
        ignis/executor/api/IJsonValue.cpp
        ignis/executor/api/IJsonValue.h
        ignis/executor/api/IReadIterator.h
        ignis/executor/api/IVector.h
        ignis/executor/api/IWriteIterator.h

        #Exception
        ignis/executor/core/exception/ICompatibilityException.h
        ignis/executor/core/exception/IException.h
        ignis/executor/core/exception/IException.cpp
        ignis/executor/core/exception/IInvalidArgument.h
        ignis/executor/core/exception/ILogicError.h

        #IO
        ignis/executor/core/io/IEnumTypes.h
        ignis/executor/core/io/IJsonReader.h
        ignis/executor/core/io/IJsonReader.tcc
        ignis/executor/core/io/IJsonWriter.h
        ignis/executor/core/io/IJsonWriter.tcc
        ignis/executor/core/io/IPrinter.h
        ignis/executor/core/io/IPrinter.tcc
        ignis/executor/core/io/IReader.h
        ignis/executor/core/io/IReader.tcc
        ignis/executor/core/io/IWriter.h
        ignis/executor/core/io/IWriter.tcc

        #Modules Impl
        ignis/executor/core/modules/impl/IBaseImpl.cpp
        ignis/executor/core/modules/impl/IBaseImpl.h
        ignis/executor/core/modules/impl/ICacheImpl.cpp
        ignis/executor/core/modules/impl/ICacheImpl.h
        ignis/executor/core/modules/impl/ICacheImpl.tcc
        ignis/executor/core/modules/impl/ICommImpl.cpp
        ignis/executor/core/modules/impl/ICommImpl.h
        ignis/executor/core/modules/impl/ICommImpl.tcc
        ignis/executor/core/modules/impl/IGroupByImpl.cpp
        ignis/executor/core/modules/impl/IGroupByImpl.h
        ignis/executor/core/modules/impl/IGroupByImpl.tcc
        ignis/executor/core/modules/impl/IIOImpl.cpp
        ignis/executor/core/modules/impl/IIOImpl.h
        ignis/executor/core/modules/impl/IIOImpl.tcc
        ignis/executor/core/modules/impl/IPipeImpl.cpp
        ignis/executor/core/modules/impl/IPipeImpl.h
        ignis/executor/core/modules/impl/IPipeImpl.tcc
        ignis/executor/core/modules/impl/IReduceImpl.cpp
        ignis/executor/core/modules/impl/IReduceImpl.h
        ignis/executor/core/modules/impl/IReduceImpl.tcc
        ignis/executor/core/modules/impl/ISortImpl.cpp
        ignis/executor/core/modules/impl/ISortImpl.h
        ignis/executor/core/modules/impl/ISortImpl.tcc

        #Modules
        ignis/executor/core/modules/ICacheContextModule.cpp
        ignis/executor/core/modules/ICacheContextModule.h
        ignis/executor/core/modules/ICommModule.cpp
        ignis/executor/core/modules/ICommModule.h
        ignis/executor/core/modules/IExecutorServerModule.cpp
        ignis/executor/core/modules/IExecutorServerModule.h
        ignis/executor/core/modules/IGeneralActionModule.cpp
        ignis/executor/core/modules/IGeneralActionModule.h
        ignis/executor/core/modules/IGeneralModule.cpp
        ignis/executor/core/modules/IGeneralModule.h
        ignis/executor/core/modules/IIOModule.cpp
        ignis/executor/core/modules/IIOModule.h
        ignis/executor/core/modules/IMathModule.cpp
        ignis/executor/core/modules/IMathModule.h
        ignis/executor/core/modules/IModule.cpp
        ignis/executor/core/modules/IModule.h

        #Protocol
        ignis/executor/core/protocol/IObjectProtocol.h
        ignis/executor/core/protocol/IProtocol.h

        #Selector
        ignis/executor/core/selector/IGeneralActionSelector.h
        ignis/executor/core/selector/IGeneralSelector.h
        ignis/executor/core/selector/ISelector.h
        ignis/executor/core/selector/ITypeSelector.h

        #Storage
        ignis/executor/core/storage/IDiskPartition.h
        ignis/executor/core/storage/IDiskPartition.tcc
        ignis/executor/core/storage/IMemoryPartition.h
        ignis/executor/core/storage/IMemoryPartition.tcc
        ignis/executor/core/storage/IPartition.h
        ignis/executor/core/storage/IRawMemoryPartition.h
        ignis/executor/core/storage/IRawMemoryPartition.tcc
        ignis/executor/core/storage/IRawPartition.h
        ignis/executor/core/storage/IRawPartition.tcc
        ignis/executor/core/storage/IVoidPartition.h
        ignis/executor/core/storage/IVoidPartition.cpp

        #Transport
        ignis/executor/core/transport/IHeaderTransport.cpp
        ignis/executor/core/transport/IHeaderTransport.h
        ignis/executor/core/transport/IMemoryBuffer.cpp
        ignis/executor/core/transport/IMemoryBuffer.h
        ignis/executor/core/transport/ITransport.h
        ignis/executor/core/transport/IZlibTransport.cpp
        ignis/executor/core/transport/IZlibTransport.h

        #ignis/executor core
        ignis/executor/core/IExecutorData.cpp
        ignis/executor/core/IExecutorData.h
        ignis/executor/core/IExecutorData.tcc
        ignis/executor/core/ILibraryLoader.cpp
        ignis/executor/core/ILibraryLoader.h
        ignis/executor/core/ILog.cpp
        ignis/executor/core/ILog.h
        ignis/executor/core/IMpi.cpp
        ignis/executor/core/IMpi.tcc
        ignis/executor/core/IMpi.h
        ignis/executor/core/IPartitionTools.cpp
        ignis/executor/core/IPartitionTools.tcc
        ignis/executor/core/IPartitionTools.h
        ignis/executor/core/IPropertyParser.cpp
        ignis/executor/core/IPropertyParser.h
        ignis/executor/core/RTTInfo.cpp
        ignis/executor/core/RTTInfo.h

        #RPC
        ${rpc} )

add_definitions(-DBOOST_ALL_DYN_LINK)

set(CMAKE_CXX_FLAGS "-fPIC -rdynamic -pthread -fopenmp -g")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(igniscore SHARED ${SOURCE_FILES})
add_library(igniscore_types SHARED ignis/executor/core/ICoreTypes.cpp)
add_executable(ignis-cpp main.cpp)

TARGET_LINK_LIBRARIES(igniscore thrift dl backtrace thriftz z boost_log boost_thread boost_system boost_filesystem)
TARGET_LINK_LIBRARIES(igniscore_types igniscore)
TARGET_LINK_LIBRARIES(ignis-cpp igniscore igniscore_types)

install(TARGETS igniscore LIBRARY DESTINATION lib)
install(TARGETS ignis-cpp RUNTIME DESTINATION bin)
install(DIRECTORY ignis
        DESTINATION include/
        FILES_MATCHING PATTERN "*.h" PATTERN "*.tcc")