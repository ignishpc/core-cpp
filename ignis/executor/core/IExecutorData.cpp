
#include "IExecutorData.h"

using namespace ignis::executor::core;

IExecutorData::IExecutorData() : properties_parser(core::IPropertiesParser(context.getProperties())) {}


void IExecutorData::loadObject(std::shared_ptr<storage::IObject> object) {
    loaded_object = object;
}

bool IExecutorData::hasLoadObject() {
    return bool(loaded_object);
}

void IExecutorData::deleteLoadObject() {
    loaded_object.reset();
}

storage::IObject &IExecutorData::getLoadObject() {
    return *loaded_object;
}

std::shared_ptr<storage::IObject> IExecutorData::getSharedLoadObject() {
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


