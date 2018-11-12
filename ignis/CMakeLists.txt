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
        IHeaders.h
        #Data
        data/handle/IDeleter.h
        data/handle/IEnumTypes.h
        data/handle/IOperators.h
        data/handle/IOperatorsImpl.h
        data/handle/IPrinter.h
        data/handle/IReader.h
        data/handle/IWriter.h
        data/IMemoryBuffer.h
        data/IMemoryBuffer.cpp
        data/IZlibTransport.h
        data/IZlibTransport.cpp
        data/IFileTransport.h
        data/IFileTransport.cpp
        data/IObjectProtocol.h
        data/RTTInfo.h
        data/RTTInfo.cpp

        #Driver
        driver/api/IgnisException.h
        driver/api/IgnisException.cpp

        #Exception
        exceptions/IException.h
        exceptions/IException.cpp
        exceptions/ILogicError.h
        exceptions/IInvalidArgument.h

        #Executor api
        executor/api/IManager.h
        executor/api/IReadIterator.h
        executor/api/IWriteIterator.h
        executor/api/IContext.h
        executor/api/IContext.cpp
        executor/api/IRegister.h
        executor/api/Iterable.h
        executor/api/IValueBase.h
        executor/api/IValue.h
        executor/api/IExecute.h
        executor/api/function/IFunctionBase.h
        executor/api/function/IFunction.h
        executor/api/function/IFunction2Base.h
        executor/api/function/IFunction2.h
        executor/api/function/IFlatFunctionBase.h
        executor/api/function/IFlatFunction.h

        #Executor core
        executor/core/ILog.h
        executor/core/ILog.cpp
        executor/core/IExecutorData.h
        executor/core/IExecutorData.cpp
        executor/core/IObjectLoader.h
        executor/core/IObjectLoader.cpp
        executor/core/IPropertiesParser.h
        executor/core/IPropertiesParser.cpp

        #Modules
        executor/core/modules/IModule.h
        executor/core/modules/IModule.cpp
        executor/core/modules/IServerModule.h
        executor/core/modules/IServerModule.cpp
        executor/core/modules/IMapperModule.h
        executor/core/modules/IMapperModule.cpp
        executor/core/modules/IReducerModule.h
        executor/core/modules/IReducerModule.cpp
        executor/core/modules/IPostmanModule.h
        executor/core/modules/IPostmanModule.cpp
        executor/core/modules/IFilesModule.h
        executor/core/modules/IFilesModule.cpp
        executor/core/modules/IStorageModule.h
        executor/core/modules/IStorageModule.cpp
        executor/core/modules/IKeysModule.h
        executor/core/modules/IKeysModule.cpp
        executor/core/modules/ISortModule.h
        executor/core/modules/ISortModule.cpp
        executor/core/modules/IShuffleModule.h
        executor/core/modules/IShuffleModule.cpp

        #Storage
        executor/core/IPostBox.h
        executor/core/IMessage.h
        executor/core/IPostBox.cpp
        executor/core/storage/IObject.h
        executor/core/storage/IRawObject.h
        executor/core/storage/IRawObject.cpp
        executor/core/storage/IMemoryObject.h
        executor/core/storage/IMemoryObject.cpp
        executor/core/storage/IRawMemoryObject.h
        executor/core/storage/IRawMemoryObject.cpp
        executor/core/storage/iterator/ICoreIterator.h
        executor/core/storage/iterator/IElementIterator.h
        executor/core/storage/iterator/IElementIterator.cpp
        executor/core/storage/iterator/ITransportIterator.h
        executor/core/storage/iterator/ITransportIterator.cpp
        executor/core/storage/iterator/IMemoryIterator.h
        executor/core/storage/iterator/IMemoryIterator.cpp

        #RPC
        ${rpc})

set(CMAKE_CXX_FLAGS "-fPIC -rdynamic -pthread -fopenmp -DBOOST_ALL_DYN_LINK")#-O3

add_library(igniscore SHARED ${SOURCE_FILES})
add_executable(ignis-cpp main.cpp)

TARGET_LINK_LIBRARIES(igniscore jsoncpp thrift boost_system boost_filesystem dl thriftz z boost_log boost_thread)
TARGET_LINK_LIBRARIES(ignis-cpp igniscore)

install(TARGETS igniscore LIBRARY DESTINATION lib)
install(DIRECTORY ../ignis
        DESTINATION include/
        FILES_MATCHING PATTERN "*.h"
        PATTERN executor/core EXCLUDE)