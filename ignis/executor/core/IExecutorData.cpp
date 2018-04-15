
#include "IExecutorData.h"

using namespace ignis::executor::core;

IExecutorData::IExecutorData() : properties_parser(IPropertiesParser(context.getProperties())) {}

void loadObject(std::shared_ptr<storage::IObject> &object);

bool IExecutorData::hasLoadObject(){
    return bool(loaded_object);
}

void IExecutorData::deleteLoadObject(){
    loaded_object.reset();
}

storage::IObject &IExecutorData::getLoadObject(){
    return *loaded_object;
}

std::unordered_map<size_t, std::shared_ptr<storage::IObject>>& IExecutorData::getStoredObjects(){
    return stored_objects;
};

ignis::executor::api::IContext &IExecutorData::getContext(){
    return context;
}

IPropertiesParser &IExecutorData::getParser(){
    return properties_parser;
}

storage::IPostBox &IExecutorData::getPostBox() {
    return post_box;
}
