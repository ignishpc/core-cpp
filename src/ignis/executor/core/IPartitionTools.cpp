
#include "IPartitionTools.h"
#include "ILog.h"
#include <boost/filesystem.hpp>

using namespace ignis::executor::core;

IPartitionTools::IPartitionTools(IPropertyParser &properties, api::IContext &context)
    : properties(properties), context(context) {}

std::shared_ptr<storage::IVoidPartition> IPartitionTools::newVoidPartition(int64_t sz) {
    IGNIS_LOG(warning) << "Creating a void partition";
    if (properties.partitionType() == storage::IDiskPartition<storage::IVoidPartition::VOID_TYPE>::TYPE) {
        return std::make_shared<storage::IVoidPartition>(diskPath());
    } else {
        return std::make_shared<storage::IVoidPartition>(sz);
    }
}

void IPartitionTools::createDirectoryIfNotExists(const std::string &path) {
    if (!boost::filesystem::is_directory(path)) {
        boost::system::error_code error;
        boost::filesystem::create_directories(path, error);
        if (error && !boost::filesystem::is_directory(path)) {
            throw exception::IInvalidArgument("Unable to create directory " + path + " " + error.message());
        }
    }
}

bool IPartitionTools::createHardLink(const std::string &target, const std::string &link) {
    boost::filesystem::remove(link);
    boost::system::error_code error;
    boost::filesystem::create_hard_link(target, link, error);
    return !error.failed();
}

bool IPartitionTools::copyFile(const std::string &from, const std::string &to) {
    boost::filesystem::remove(to);
    boost::system::error_code error;
    boost::filesystem::copy(from, to, error);
    return !error.failed();
}