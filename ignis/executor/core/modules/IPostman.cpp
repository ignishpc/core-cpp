
#include "IPostman.h"
#include <thread>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TZlibTransport.h>
#include "../../../data/IObjectProtocol.h"
#include "../../../data/IFileTransport.h"

using namespace std;
using namespace apache::thrift::transport;
using namespace ignis::executor::core::modules;

IPostman::IPostman(shared_ptr<core::IExecutorData> &executor_data) : IgnisModule(executor_data) {}

void IPostman::threadAccept(shared_ptr<TTransport> transport) {
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
    transport = make_shared<TBufferedTransport>(transport);
    transport = make_shared<TZlibTransport>(transport);
    protocol = make_shared<data::IObjectProtocol>(transport);

    int64_t id;
    protocol->readI64(id);
    auto memory = std::make_shared<TMemoryBuffer>();
    //executor_data->post_box.newMessage(id, memory);
    int bytes;
    uint8_t buffer[256];
    while ((bytes = transport->read(buffer, 256)) > -1) {
        memory->write(buffer, bytes);
    }
    transport->close();
}

void IPostman::threadServer() {
    std::vector<std::shared_ptr<TTransport>> connections;
    server->listen();
    while (started) {
        try {
            auto connection = server->accept();
            connections.push_back(connection);
            std::thread(&IPostman::threadAccept, this, connection).detach();
        } catch (std::exception &ex) {
            if (started) {
                //log error de conexion
            }
        }
    }
    for (auto trans:connections) {
        trans->close();
    }
}

void IPostman::start() {
    started = true;
    server = std::make_shared<TServerSocket>(1963);//TODO dinamic port
    std::thread(&IPostman::threadServer, this).detach();
}

void IPostman::stop() {
    started = false;
    server->close();
}

void IPostman::sendAll() {

}