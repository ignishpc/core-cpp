
#include "IFilesModule.h"
#include <fstream>
#include "../ILog.h"
#include "../storage/IRawMemoryObject.h"
#include "../storage/IMemoryObject.h"
#include "../../../exceptions/IInvalidArgument.h"

using namespace std;
using namespace ignis::executor::core::storage;
using namespace ignis::executor::core::modules;
using namespace ignis::executor::core;
using ignis::rpc::IRemoteException;

IFilesModule::IFilesModule(std::shared_ptr<IExecutorData> &executor_data) : IgnisModule(executor_data) {}

void IFilesModule::readFile(const std::string &path, const int64_t offset, const int64_t len, const int64_t lines) {
    try {
        string storage = executor_data->getContext()["ignis.executor.storage"];
        int compression = executor_data->getParser().getNumber("ignis.executor.storage.compression");
        shared_ptr<IObject> object;
        auto manager = make_shared<data::IManager<string>>();
        auto& manager_any=(shared_ptr<data::IManager<IObject::Any>>&)manager;
        if (storage == "disk") {
            storage = "raw memory";//TODO create IDiskObject
            object = make_shared<IRawMemoryObject>(compression, len);
            object->setManager(manager_any);
        } else if (storage == "raw memory") {
            storage = "raw memory";
            object = make_shared<IRawMemoryObject>(compression, len);
            object->setManager(manager_any);
        } else {
            storage = "memory";
            object = make_shared<IMemoryObject>(manager_any, lines);
        }
        IGNIS_LOG(info) << "IFileModule reading"
                        << " path: " << path
                        << " offset: " << offset
                        << " len: " << len
                        << " storage: " << storage
                        << " compression: " << compression;

        ifstream fs(path);
        if (!fs.is_open()) {
            throw exceptions::IInvalidArgument("IFileModule cannot open file " + path);
        }
        fs.seekg(offset);

        auto writer = object->writeIterator();
        string line;
        for (int i = 0; i < lines; i++) {
            getline(fs, line, '\n');
            writer->write(move(IObject::toAny(line)));
        }
        executor_data->loadObject(object);
    } catch (exceptions::IException &ex) {
        IRemoteException iex;
        iex.__set_cause(ex.what());
        iex.__set_stack(ex.toString());
        throw iex;
    } catch (std::exception &ex) {
        IRemoteException iex;
        iex.__set_cause(ex.what());
        iex.__set_stack("UNKNOWN");
        throw iex;
    }
}

void IFilesModule::saveFile(const std::string &path, const bool joined) {
    try {
        auto &object = executor_data->getLoadObject();
        if (!object.getManager()) {
            throw exceptions::IInvalidArgument("IFileModule c++ required use this data before save it " + path);
        }

        auto reader = object.readIterator();
        auto printer = object.getManager()->getClassManagerType()->getElemClassManager()->getTypeHandle()->printer();

        ofstream fs;
        if (joined) {
            fs.open(path, fstream::app);
        } else {
            fs.open(path + "_" + to_string(executor_data->getExecutorId()), fstream::trunc);
        }
        if (!fs.is_open()) {
            throw exceptions::IInvalidArgument("IFileModule cannot create/open file " + path);
        }

        IGNIS_LOG(info) << "IFileModule saving "
                        << " path: " << path
                        << " joined: " << joined ? "true" : "false";
        while (reader->hashNext()) {
            (*printer)(reader->next(), fs);
            fs << endl;
        }
    } catch (exceptions::IException &ex) {
        IRemoteException iex;
        iex.__set_cause(ex.what());
        iex.__set_stack(ex.toString());
        throw iex;
    } catch (std::exception &ex) {
        IRemoteException iex;
        iex.__set_cause(ex.what());
        iex.__set_stack("UNKNOWN");
        throw iex;
    }
}

void IFilesModule::saveJson(const std::string &path, const bool joined) {
    try {
        auto &object = executor_data->getLoadObject();
        if (!object.getManager()) {
            throw exceptions::IInvalidArgument("IFileModule c++ required use this data before save it " + path);
        }

        auto reader = object.readIterator();
        auto printer = object.getManager()->getClassManagerType()->getElemClassManager()->getTypeHandle()->printer();

        ofstream fs;
        if (joined) {
            fs.open(path, fstream::app);
            fs.seekp(-1, fs.end);
        } else {
            fs.open(path + "_" + to_string(executor_data->getExecutorId()), fstream::trunc);
        }
        if (!fs.is_open()) {
            throw exceptions::IInvalidArgument("IFileModule cannot create/open file " + path);
            fs << "[" << endl;
        }

        IGNIS_LOG(info) << "IFileModule saving as json"
                        << " path: " << path
                        << " joined: " << joined ? "true" : "false";
        if (reader->hashNext()) {
            (*printer)(reader->next(), fs);
        }

        while (reader->hashNext()) {
            fs << endl;
            (*printer)(reader->next(), fs);
        }
        fs << "]";

    } catch (exceptions::IException &ex) {
        IRemoteException iex;
        iex.__set_cause(ex.what());
        iex.__set_stack(ex.toString());
        throw iex;
    } catch (std::exception &ex) {
        IRemoteException iex;
        iex.__set_cause(ex.what());
        iex.__set_stack("UNKNOWN");
        throw iex;
    }
}

IFilesModule::~IFilesModule() {

}
