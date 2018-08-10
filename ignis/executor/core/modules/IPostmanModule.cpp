
#include "IPostmanModule.h"
#include <thread>
#include <sstream>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TSocket.h>
#include "../../../data/IObjectProtocol.h"
#include "../../../data/IFileTransport.h"
#include "../ILog.h"

using namespace std;
using namespace apache::thrift::transport;
using namespace ignis::executor::core::storage;
using namespace ignis::executor::core::modules;
using namespace ignis::executor::core;
using ignis::rpc::IRemoteException;

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
            shared_ptr<IObject> object = getIObject();
            IGNIS_LOG(info) << "IPostmanModule id " << id << " receiving"
                            << " mode: " << (use_shared_memory ? "shared memory" : "socket");
            IMessage msg("local", object);
            executor_data->getPostBox().newMessage(id, msg);
            object->read(buffer);
            IGNIS_LOG(info) << "IPostmanModule id " << id << " received OK";
            object->fit();
        } catch (exceptions::IException &ex) {
            IGNIS_LOG(warning) << "IPostmanModule id " << id << " received FAILS " << ex.toString();
        } catch (std::exception &ex) {
            IGNIS_LOG(warning) << "IPostmanModule id " << id << " received FAILS " << ex.what();
        }
    } catch (exceptions::IException &ex) {
        IGNIS_LOG(warning) << "IPostmanModule connection exception " << ex.toString();
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
    IGNIS_LOG(info) << "IPostmanModule stopped, " << connections.size() << " connections accepted";
}

void IPostmanModule::start() {
    try {
        IGNIS_LOG(info) << "IPostmanModule starting";
        started = true;
        auto port = executor_data->getParser().getNumber("ignis.executor.transport.port");
        server = std::make_shared<TServerSocket>(port);
        std::thread(&IPostmanModule::threadServer, this).detach();
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

void IPostmanModule::stop() {
    IGNIS_LOG(info) << "IPostmanModule stopping";
    started = false;
    server->close();
}

int IPostmanModule::send(size_t id, IMessage &msg, int8_t compression) {
    try {
        stringstream saddr(msg.getAddr());
        vector<string> vaddr;
        while (saddr.good()) {
            string tmp;
            std::getline(saddr, tmp, '!');
            vaddr.push_back(tmp);
        }

        if (vaddr[0].find("local") == 0) {
            executor_data->getPostBox().newMessage(id, msg);
            return 1;
        }
        string addr_host = vaddr[1];
        int addr_port = stoi(vaddr[2]);


        IGNIS_LOG(warning) << "IPostmanModule id " << id << " connecting to " << addr_host << ":" << addr_port;
        shared_ptr<TTransport> transport = make_shared<TSocket>(addr_host, addr_port);
        for (int i = 0; i < 10; i++) {
            try {
                transport->open();
                break;
            } catch (std::exception &ex) {
                sleep(1);
                if (i == 9) {
                    throw ex;
                }
            }
        }
        auto protocol = make_shared<data::IObjectProtocol>(transport);
        bool use_shared_memory = vaddr[0].find("shared") == 0;
        protocol->writeBool(use_shared_memory);
        if (use_shared_memory) {
            string folder = vaddr[3];
            size_t block_size = stoul(vaddr[4]);
            protocol->writeString(folder);
            string buffer1 = folder + "buffer1";
            string buffer2 = folder + "buffer2";
            transport = make_shared<data::IFileTransport>(buffer1, buffer2, transport, block_size);
        }
        protocol->writeI64(id);
        auto buffer = make_shared<TBufferedTransport>(transport);
        IGNIS_LOG(info) << "IPostmanModule id " << id << " sending"
                        << " mode: " << (use_shared_memory ? "shared memory" : "socket");
        msg.getObj()->write(buffer, compression);
        transport->close();
        IGNIS_LOG(info) << "IPostmanModule id " << id << " sent OK";
    } catch (exceptions::IException &ex) {
        IGNIS_LOG(warning) << "IPostmanModule id " << id << " sent FAILS " << ex.toString();
        return 1;

    } catch (std::exception &ex) {
        IGNIS_LOG(warning) << "IPostmanModule id " << id << "sent FAILS " << ex.what();
        return 1;
    }
}

void IPostmanModule::sendAll() {
    try {
        auto msgs = executor_data->getPostBox().getOutMessages();
        size_t threads = executor_data->getParser().getNumber("ignis.executor.transport.threads");
        int8_t compression = executor_data->getParser().getNumber("ignis.executor.transport.compression");
        int errors = 0;
        if (threads > 1) {
            for (auto &entry: msgs) {
                errors += send(entry.first, entry.second, compression);
            }
        } else {
            pair<size_t, IMessage> msgs_array[msgs.size()];
            int i = 0;
            for (auto &entry: msgs) {
                msgs_array[i] = make_pair<size_t, IMessage>((size_t) entry.first, (IMessage) entry.second);
            }
#pragma omp parallel for num_threads(threads), reduction(+:errors)
            for (int i = 0; i < msgs.size(); i++) {
                errors += send(msgs_array[i].first, msgs_array[i].second, compression);
            }
        }
        if (errors > 0) {
            IGNIS_LOG(error) << "IPostmanModule fail to send " << errors << " messages";
            exceptions::IException ex("IPostmanModule fail to send " + to_string(errors) + " messages");
            IRemoteException iex;
            iex.__set_message(ex.what());
            iex.__set_stack(ex.toString());
            throw iex;
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

void IPostmanModule::clearAll() {
    try {
        executor_data->getPostBox().getOutMessages();
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

IPostmanModule::~IPostmanModule() {

}
