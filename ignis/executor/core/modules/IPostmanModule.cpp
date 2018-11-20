
#include "IPostmanModule.h"

#include <sstream>
#include <vector>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TSocket.h>
#include "../../../data/IObjectProtocol.h"
#include "../../../data/IFileTransport.h"
#include "../ILog.h"

using namespace ignis::executor::core::storage;
using namespace ignis::executor::core::modules;
using namespace ignis::executor::core;
using ignis::rpc::IRemoteException;

IPostmanModule::IPostmanModule(std::shared_ptr<core::IExecutorData> &executor_data) : IgnisModule(executor_data),
                                                                                      started(false) {}

void IPostmanModule::threadAccept(std::shared_ptr<transport::TTransport> transport) {
    try {
        auto protocol = std::make_shared<data::IObjectProtocol>(transport);
        int64_t id;
        std::string addr_mode;
        protocol->readI64(id);
        protocol->readString(addr_mode);
        if (addr_mode == "socket") {
            //Do nothing else
        } else if (addr_mode == "unixSocket") {
            throw ignis::exceptions::ILogicError(
                    "IPostmanModule id " + std::to_string(id) + " mode " + addr_mode + " not implemented yet");
        } else if (addr_mode == "memoryBuffer") {
            std::string addr_path;
            protocol->readString(addr_path);
            std::string buffer1 = addr_path + "/buffer1";
            std::string buffer2 = addr_path + "/buffer2";
            transport = std::make_shared<data::IFileTransport>(buffer1, buffer2, transport);
        } else {
            throw ignis::exceptions::ILogicError(
                    "IPostmanModule id " + std::to_string(id) + " mode " + addr_mode + " unknown");
        }
        try {
            auto buffer = std::make_shared<transport::TBufferedTransport>(transport);
            std::shared_ptr<IObject> object = getIObject();
            IGNIS_THREAD_LOG(info) << "IPostmanModule id " << id << " receiving" << " mode: " << addr_mode;
            object->read(buffer);
            IMessage msg("local", object);
            executor_data->getPostBox().newInMessage(id, msg);
            IGNIS_THREAD_LOG(info) << "IPostmanModule id " << id << " received OK";
            object->fit();
        } catch (exceptions::IException &ex) {
            IGNIS_THREAD_LOG(warning) << "IPostmanModule id " << id << " received FAILS " << ex.toString();
        } catch (std::exception &ex) {
            IGNIS_THREAD_LOG(warning) << "IPostmanModule id " << id << " received FAILS " << ex.what();
        }
    } catch (exceptions::IException &ex) {
        IGNIS_THREAD_LOG(warning) << "IPostmanModule connection exception " << ex.toString();
    } catch (std::exception &ex) {
        IGNIS_THREAD_LOG(warning) << "IPostmanModule connection exception " << ex.what();
    }
    transport->close();
}

void IPostmanModule::threadServer() {
    IGNIS_THREAD_LOG(info) << "IPostmanModule started";
    std::vector<std::shared_ptr<std::thread>> connections;
    server->listen();
    IGNIS_THREAD_LOG(info) << "IPostmanModule listening on port " << server->getPort();
    while (started) {
        try {
            auto connection = server->accept();
            IGNIS_THREAD_LOG(info) << "IPostmanModule connection accepted";
            connections.push_back(std::make_shared<std::thread>(&IPostmanModule::threadAccept, this, connection));
        } catch (std::exception &ex) {
            if (started) {
                IGNIS_THREAD_LOG(warning) << "IPostmanModule accept exception" << ex.what();
            }
        }
    }
    for (auto &th:connections) {
        th->join();
    }
    server->close();
    IGNIS_THREAD_LOG(info) << "IPostmanModule stopped, " << connections.size() << " connections accepted";
}

void IPostmanModule::start() {
    try {
        if (!started && !thread_server) {
            IGNIS_THREAD_LOG(info) << "IPostmanModule starting";
            started = true;
            auto port = executor_data->getParser().getNumber("ignis.executor.transport.port");
            server = std::make_shared<transport::TServerSocket>(port);
            thread_server = std::make_shared<std::thread>(&IPostmanModule::threadServer, this);
        }
    } catch (exceptions::IException &ex) {
        IRemoteException iex;
        iex.__set_message(ex.what());
        iex.__set_stack(ex.getStacktrace());
        throw iex;
    } catch (std::exception &ex) {
        IRemoteException iex;
        iex.__set_message(ex.what());
        iex.__set_stack("UNKNOWN");
        throw iex;
    }
}

void IPostmanModule::stop() {
    try {
        IGNIS_THREAD_LOG(info) << "IPostmanModule stopping";
        if (started) {
            started = false;
            server->interrupt();
            thread_server->join();
            server.reset();
            thread_server.reset();
        }
    } catch (exceptions::IException &ex) {
        IRemoteException iex;
        iex.__set_message(ex.what());
        iex.__set_stack(ex.getStacktrace());
        throw ex;
    } catch (std::exception &ex) {
        IRemoteException iex;
        iex.__set_message(ex.what());
        iex.__set_stack("UNKNOWN");
        throw iex;
    }
}

int IPostmanModule::send(size_t id, IMessage &msg, int8_t compression) {
    try {
        std::stringstream saddr(msg.getAddr());
        std::vector<std::string> vaddr;
        while (saddr.good()) {
            std::string tmp;
            std::getline(saddr, tmp, '!');
            vaddr.push_back(tmp);
        }
        std::string addr_mode = vaddr[0];

        if (addr_mode == "local") {
            executor_data->getPostBox().newInMessage(id, msg);
            return 0;
        }
        std::string addr_host = vaddr[1];
        int addr_port = stoi(vaddr[2]);

        IGNIS_THREAD_LOG(info) << "IPostmanModule id " << id << " connecting to " << addr_host << ":" << addr_port;
        std::shared_ptr<transport::TTransport> transport = std::make_shared<transport::TSocket>(addr_host, addr_port);
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
        auto protocol = std::make_shared<data::IObjectProtocol>(transport);
        protocol->writeI64(id);
        protocol->writeString(addr_mode);
        if (addr_mode == "socket") {
            //Do nothing else
        } else if (addr_mode == "unixSocket") {
            throw ignis::exceptions::ILogicError(
                    "IPostmanModule id " + std::to_string(id) + " mode " + addr_mode + " not implemented yet");
        } else if (addr_mode == "memoryBuffer") {
            std::string addr_path = vaddr[3];
            size_t addr_block_size = std::stoul(vaddr[4]);
            protocol->writeString(addr_path);
            std::string buffer1 = addr_path + "/buffer1";
            std::string buffer2 = addr_path + "/buffer2";
            transport = std::make_shared<data::IFileTransport>(buffer1, buffer2, transport, addr_block_size);
        } else {
            throw ignis::exceptions::ILogicError(
                    "IPostmanModule id " + std::to_string(id) + " mode " + addr_mode + " unknown");
        }
        auto buffer = std::make_shared<transport::TBufferedTransport>(transport);
        IGNIS_THREAD_LOG(info) << "IPostmanModule id " << id << " sending" << " mode: " << addr_mode;
        try {
            msg.getObj()->write(buffer, compression);
        } catch (std::exception &ex) {
            transport->close();
            throw ex;
        }
        IGNIS_THREAD_LOG(info) << "IPostmanModule id " << id << " sent OK";
        return 0;
    } catch (exceptions::IException &ex) {
        IGNIS_THREAD_LOG(warning) << "IPostmanModule id " << id << " sent FAILS " << ex.toString();
        return 1;

    } catch (std::exception &ex) {
        IGNIS_THREAD_LOG(warning) << "IPostmanModule id " << id << "sent FAILS " << ex.what();
        return 1;
    }
}

void IPostmanModule::sendAll() {
    try {
        auto msgs = executor_data->getPostBox().popOutBox();
        size_t threads;
        if (executor_data->getParser().getString("ignis.executor.transport.threads") == "cores") {
            threads = executor_data->getParser().getNumber("ignis.executor.cores");
        } else {
            threads = executor_data->getParser().getNumber("ignis.executor.transport.threads");
        }
        int8_t compression = executor_data->getParser().getNumber("ignis.executor.transport.compression");
        int8_t reconnections = executor_data->getParser().getNumber("ignis.executor.transport.reconnections");
        int errors = 0;
        if (threads == 1) {
            for (auto &entry: msgs) {
                int error;
                for (int cn = 0; cn < reconnections + 1; cn++) {
                    error = send(entry.first, entry.second, compression);
                    if (error == 0) {
                        break;
                    }
                }
                errors += error;
            }
        } else {
            std::vector<std::pair<size_t, IMessage>> i_msgs(msgs.begin(), msgs.end());
#pragma omp parallel for num_threads(threads), reduction(+:errors)
            for (int i = 0; i < msgs.size(); i++) {
                int error = 0;
                for (int cn = 0; cn < reconnections + 1; cn++) {
                    error = send(i_msgs[i].first, i_msgs[i].second, compression);
                    if (error == 0) {
                        break;
                    }
                }
                errors += error;
            }
        }
        if (errors > 0) {
            IGNIS_THREAD_LOG(error) << "IPostmanModule fail to send " << errors << " messages";
            throw exceptions::IException("IPostmanModule fail to send " + std::to_string(errors) + " messages");
        }
    } catch (exceptions::IException &ex) {
        IRemoteException iex;
        iex.__set_message(ex.what());
        iex.__set_stack(ex.getStacktrace());
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
        executor_data->getPostBox().popOutBox();
    } catch (exceptions::IException &ex) {
        IRemoteException iex;
        iex.__set_message(ex.what());
        iex.__set_stack(ex.getStacktrace());
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
