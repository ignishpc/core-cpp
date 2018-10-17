
#include "IFilesModule.h"
#include <fstream>
#include "../ILog.h"
#include "../../../exceptions/IInvalidArgument.h"

using namespace ignis::executor::core::storage;
using namespace ignis::executor::core::modules;
using namespace ignis::executor::core;
using ignis::rpc::IRemoteException;

IFilesModule::IFilesModule(std::shared_ptr<IExecutorData> &executor_data) : IgnisModule(executor_data) {}

void IFilesModule::readFile(const std::string &path, const int64_t offset, const int64_t len, const int64_t lines) {
    try {
        auto manager = std::make_shared<api::IManager<std::string>>();
        auto &manager_any = (std::shared_ptr<api::IManager<IObject::Any>> &) manager;
        std::shared_ptr<IObject> object = getIObject(manager_any, lines, len);
        IGNIS_LOG(info) << "IFileModule reading"
                        << " path: " << path
                        << " offset: " << offset
                        << " len: " << len;
        std::ifstream fs(path);
        if (!fs.is_open()) {
            throw exceptions::IInvalidArgument("IFileModule cannot open file " + path);
        }
        fs.seekg(offset);

        auto writer = object->writeIterator();
        std::string line;
        for (int i = 0; i < lines; i++) {
            getline(fs, line, '\n');
            writer->write(std::move(IObject::toAny(line)));
        }
        object->fit();
        executor_data->loadObject(object);
    } catch (exceptions::IException &ex) {
        IRemoteException iex;
        iex.__set_message(ex.what());
        iex.__set_stack(ex.toString());
        throw iex;
    } catch (std::exception &ex) {
        IRemoteException iex;
        iex.__set_message(ex.what());
        iex.__set_stack("UNKNOWN");
        throw iex;
    }
}

void IFilesModule::saveFile(const std::string &path, const bool trunc, const bool new_line) {
    try {
        std::shared_ptr<IObject> object = executor_data->getSharedLoadObject();
        executor_data->deleteLoadObject();
        if (!object->getManager()) {
            throw exceptions::IInvalidArgument("IFileModule c++ required use this data before use it");
        }

        auto reader = object->readIterator();
        auto printer = object->getManager()->printer();

        std::ofstream fs;
        if (trunc) {
            fs.open(path, std::fstream::trunc);
        } else {
            fs.open(path, std::fstream::app);
        }
        if (!fs.is_open()) {
            throw exceptions::IInvalidArgument("IFileModule cannot create/open file " + path);
        }

        IGNIS_LOG(info) << "IFileModule saving "
                        << " path: " << path
                        << ", truncate: " << trunc ? "true" : "false";

        if (reader->hasNext()) {
            (*printer)(reader->next(), fs);
        }

        while (reader->hasNext()) {
            fs << std::endl;
            (*printer)(reader->next(), fs);
        }

        if (new_line) {
            fs << std::endl;
        }
    } catch (exceptions::IException &ex) {
        IRemoteException iex;
        iex.__set_message(ex.what());
        iex.__set_stack(ex.toString());
        throw iex;
    } catch (std::exception &ex) {
        IRemoteException iex;
        iex.__set_message(ex.what());
        iex.__set_stack("UNKNOWN");
        throw iex;
    }
}

void IFilesModule::saveJson(const std::string &path, const bool array_start, const bool array_end) {
    try {
        std::shared_ptr<IObject> object = executor_data->getSharedLoadObject();
        executor_data->deleteLoadObject();
        if (!object->getManager()) {
            throw exceptions::IInvalidArgument("IFileModule c++ required use this data before use it");
        }

        auto reader = object->readIterator();
        auto printer = object->getManager()->printer();

        std::ofstream fs;
        if (!array_start && !array_end) {
            fs.open(path, std::fstream::app);
            fs.seekp(-1, fs.end);
        } else {
            fs.open(path, std::fstream::trunc);
        }
        if (!fs.is_open()) {
            throw exceptions::IInvalidArgument("IFileModule cannot create/open file " + path);
        }

        IGNIS_LOG(info) << "IFileModule saving as json"
                        << " path: " << path
                        << ", array_start: " << (array_start ? "true" : "false")
                        << ", array_end: " << (array_end ? "true" : "false");

        if (array_start) {
            fs << "[" << std::endl;
        }

        if (reader->hasNext()) {
            printer->printJson(reader->next(), fs);
        }

        while (reader->hasNext()) {
            fs << std::endl << "," << std::endl;
            (*printer)(reader->next(), fs);
        }

        if (array_end) {
            fs << std::endl << "]";
        }
    } catch (exceptions::IException &ex) {
        IRemoteException iex;
        iex.__set_message(ex.what());
        iex.__set_stack(ex.toString());
        throw iex;
    } catch (std::exception &ex) {
        IRemoteException iex;
        iex.__set_message(ex.what());
        iex.__set_stack("UNKNOWN");
        throw iex;
    }
}

IFilesModule::~IFilesModule() {

}
