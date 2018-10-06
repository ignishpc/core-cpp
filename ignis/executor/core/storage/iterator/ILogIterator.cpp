
#include "ILogIterator.h"
#include "../../ILog.h"

using namespace ignis::executor::core::storage;
using namespace ignis::executor::core::storage::iterator;

IReadLogIterator::IReadLogIterator(const std::shared_ptr<ICoreReadIterator<IObject::Any>> &reader,
                                   const size_t trigger_size,
                                   const std::string &msg_before, const std::string &msg_after,
                                   size_t total_size) : reader(reader),
                                                        msg_before(msg_before),
                                                        msg_after(msg_after),
                                                        trigger_size(trigger_size),
                                                        total_size(total_size) {}

void IReadLogIterator::printLog() {
    if (total_size == 0) {
        IGNIS_LOG(info) << msg_before << read_size << msg_after;
    } else {
        IGNIS_LOG(info) << msg_before << ((read_size / total_size) * 100) << "%" << msg_after;
    }
}

IObject::Any &IReadLogIterator::next() {
    if(read_size % trigger_size == 0){
        printLog();
    }
    read_size++;
}

