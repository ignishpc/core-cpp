
#include "IPostmanModule.h"
#include <thread>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TSocket.h>
#include "../../../data/IObjectProtocol.h"
#include "../../../data/IFileTransport.h"
#include "../storage/IRawMemoryObject.h"
#include "../storage/IMessage.h"
#include "../ILog.h"

using namespace std;
using namespace apache::thrift::transport;
using namespace ignis::executor::core::storage;
using namespace ignis::executor::core::modules;

IPostmanModule::IPostmanModule(shared_ptr<core::IExecutorData> &executor_data) : IgnisModule(executor_data) {}

void IPostmanModule::threadAccept(shared_ptr<TTransport> transport) {
    try {
        auto protocol = make_shared<data::IObjectProtocol>(transport);
        bool use_shared_memory;
        protocol->readBool(use_shared_memory);
        if (use_shared_memory) {
            string folder;
            protocol->readString(folder);
            string buffer1 = folder + "buffer1";
            string buffer2 = folder + "buffer2";
            transport = make_shared<data::IFileTransport>(buffer1, buffer2, transport);
        }
        int64_t id;
        protocol->readI64(id);
        try {
            auto buffer = make_shared<TBufferedTransport>(transport);
            shared_ptr<IObject> object;
            string storage = executor_data->getContext()["ignis.executor.storage"];
            int compression = executor_data->getParser().getNumber("ignis.executor.storage.compression");
            if(storage == "disk"){
                storage = "raw memory";//TODO create IDiskObject
                object = make_shared<storage::IRawMemoryObject>(compression);
            }else{
                storage = "raw memory";
                object = make_shared<storage::IRawMemoryObject>(compression);
            }
            IGNIS_LOG(info) << "IPostmanModule id " << id << " receiving"
                            << " mode: " << (use_shared_memory ? "shared memory" : "socket")
                            << " storage: " << storage
                            << " compression: " << compression;

            IMessage msg(object);
            executor_data->getPostBox().newMessage(id, msg);
            object->read(buffer);
            IGNIS_LOG(info) << "IPostmanModule id " << id << " received OK";
        } catch (std::exception &ex) {
            IGNIS_LOG(warning) << "IPostmanModule id " << id << " received FAILS";
        }
    } catch (std::exception &ex) {
        IGNIS_LOG(warning) << "IPostmanModule connection exception " << ex.what();
    }
    transport->close();
}

void IPostmanModule::threadServer() {
    IGNIS_LOG(info) << "IPostmanModule started";
    std::vector<std::shared_ptr<TTransport>> connections;
    server->listen();
    IGNIS_LOG(info) << "IPostmanModule listening on port " << server->getPort();
    while (started) {
        try {
            auto connection = server->accept();
            IGNIS_LOG(info) << "IPostmanModule connection accepted";
            connections.push_back(connection);
            std::thread(&IPostmanModule::threadAccept, this, connection).detach();
        } catch (std::exception &ex) {
            if (started) {
                IGNIS_LOG(warning) << "IPostmanModule accept exception" << ex.what();
            }
        }
    }
    for (auto trans:connections) {
        trans->close();
    }
    IGNIS_LOG(info) << "IPostmanModule stopped, " << connections.size() <<" connections accepted";
}

void IPostmanModule::start() {
    IGNIS_LOG(info) << "IPostmanModule starting";
    started = true;
    auto port = executor_data->getParser().getNumber("ignis.executor.rpc.port");
    server = std::make_shared<TServerSocket>(port);
    std::thread(&IPostmanModule::threadServer, this).detach();
}

void IPostmanModule::stop() {
    IGNIS_LOG(info) << "IPostmanModule stopping";
    started = false;
    server->close();
}

void IPostmanModule::sendAll() {


}