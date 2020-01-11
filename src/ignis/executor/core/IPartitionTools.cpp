
#include "IPartitionTools.h"
#include "ILog.h"

using namespace ignis::executor::core;

IPartitionTools::IPartitionTools(IPropertyParser &properties, api::IContext &context) : properties(properties),
                                                                                        context(context) {}

std::shared_ptr<storage::IVoidPartition> IPartitionTools::newVoidPartition(int64_t sz) {
    IGNIS_LOG(warning) << "Creating a void partition";
    return std::make_shared<storage::IVoidPartition>(sz);
}