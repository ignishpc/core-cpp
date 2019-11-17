
#include "IModuleTest.h"

using namespace ignis::executor::core;
using namespace ignis::executor::core::modules;

IModuleTest::IModuleTest() : executor_data(std::make_shared<IExecutorData>()), library("./libtestfunctions.so"),
                             base_impl(std::make_shared<modules::impl::IBaseImpl>(executor_data)) {
    if (executor_data->mpi().executors() > 1) {
        library = "." + library;
    }
    auto &props = executor_data->getContext().props();
    props["ignis.transport.compression"] = "6";
    props["ignis.partition.compression"] = "6";
    props["ignis.partition.serialization"] = "native";
}

ignis::rpc::ISource IModuleTest::newSource(const std::string &name) {
    rpc::ISource source;
    source.obj.name = library + ":" + name;
    return source;
}

