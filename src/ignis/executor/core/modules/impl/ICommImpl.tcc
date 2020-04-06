
#include "ICommImpl.h"
#include "ignis/executor/core/transport/IMemoryBuffer.h"

#define ICommImplClass ignis::executor::core::modules::impl::ICommImpl


template<typename Tp>
std::vector<std::string> ICommImplClass::getPartitions() {
    IGNIS_TRY()
        auto group = executor_data->getPartitions<Tp>();
        auto cmp = executor_data->getProperties().msgCompression();
        std::vector<std::string> partitions;
        for (auto &part: (*group)) {
            auto buffer = std::make_shared<transport::IMemoryBuffer>();
            part->write((std::shared_ptr<transport::ITransport> &) buffer, cmp);
            partitions.push_back(buffer->getBufferAsString());
        }
        return partitions;
    IGNIS_CATCH()
}

template<typename Tp>
void ICommImplClass::setPartitions(const std::vector<std::string> &partitions) {
    IGNIS_TRY()
        auto group = executor_data->getPartitionTools().newPartitionGroup<Tp>(partitions.size());
        for (int64_t i = 0; i < partitions.size(); i++) {
            auto &bytes = partitions[i];
            auto buffer = std::make_shared<transport::IMemoryBuffer>((uint8_t *) bytes.c_str(), bytes.size());
            (*group)[i]->read((std::shared_ptr<transport::ITransport> &)buffer);
        }
        executor_data->setPartitions<Tp>(group);
    IGNIS_CATCH()
}

template<typename Tp>
void ICommImplClass::driverGather(const std::string &id) {
    IGNIS_TRY()
        //TODO
    IGNIS_CATCH()
}

template<typename Tp>
void ICommImplClass::driverGather0(const std::string &id) {
    IGNIS_TRY()
        //TODO
    IGNIS_CATCH()
}

template<typename Tp>
void ICommImplClass::driverScatter(const std::string &id, const int64_t dataId) {
    IGNIS_TRY()
        //TODO
    IGNIS_CATCH()
}