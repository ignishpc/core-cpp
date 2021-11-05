
#include "IMathImpl.h"

using namespace ignis::executor::core::modules::impl;


IMathImpl::IMathImpl(std::shared_ptr<IExecutorData> &executorData) : IBaseImpl(executorData) {}

int64_t IMathImpl::count() {
    IGNIS_TRY()
    auto input = executor_data->getPartitions<char>(true);
    IGNIS_LOG(info) << "Math: count " << input->partitions() << " partitions";
    int64_t count = 0;
    for (auto &part : *input) { count += part->size(); }
    return count;
    IGNIS_CATCH()
}