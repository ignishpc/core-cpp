
#include "IExecutorData.h"

using namespace ignis::executor::core;

IExecutorData::IExecutorData() : properties_parser(core::IPropertiesParser(context.getProperties())) {}


std::shared_ptr<storage::IObject> IExecutorData::loadObject(std::shared_ptr<storage::IObject> object) {
    auto aux = loaded_object;
    loaded_object = object;
    return aux;
}

void IExecutorData::deleteLoadObject() {
    loaded_object.reset();
}

std::shared_ptr<storage::IObject> IExecutorData::loadObject() {
    return loaded_object;
}

ignis::executor::api::IContext &IExecutorData::getContext() {
    return context;
}

IPropertiesParser &IExecutorData::getParser() {
    return properties_parser;
}

IPostBox &IExecutorData::getPostBox() {
    return post_box;
}

int64_t IExecutorData::getThreads() {
    try {
        return properties_parser.getNumber("ignis.executor.cores");
    } catch (...) {
        return 1;
    }
}

IExecutorData::~IExecutorData() {

}


