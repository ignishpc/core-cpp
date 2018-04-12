#include <cstdlib>
#include <iostream>
/*
#include "data/serialization_old/IReader.h"
#include "data/serialization_old/IWriter.h"
#include "executor/api/IException.h"
#include "driver/api/IgnisException.h"
#include "executor/core/IDinamicObject.h"
#include <thrift/transport/TFDTransport.h>
#include <thrift/protocol/TCompactProtocol.h>
#include <thrift/transport/TBufferTransports.h>

using namespace std;
using namespace ignis::executor::api;
using namespace ignis::driver::api;
using namespace ignis::data;
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;

class Any {
public:
    virtual ~Any() { cout << "no" << endl; }
};

ignis::data::serialization_old::IReader r;
ignis::data::serialization_old::IWriter w;

void read() {
    int file = open("/mnt/d/test.raw", O_CREAT | O_RDWR);
    auto fileTransport = make_shared<TFDTransport>(file);
    auto transport = make_shared<TBufferedTransport>(fileTransport);
    auto protocol = make_shared<TCompactProtocol>(transport);
    auto obj = r.readObject(*protocol);
    close(file);
}

void write() {
    int file = open("/mnt/d/test.raw", O_CREAT | O_RDWR);
    auto fileTransport = make_shared<TFDTransport>(file);
    auto transport = make_shared<TBufferedTransport>(fileTransport);
    auto protocol = make_shared<TCompactProtocol>(transport);

    vector<pair<unordered_set<int>, unordered_map<int, int>>> data;
    ////////////
    srand(0);
    for (int p = 0; p < 10000; p++) {
        unordered_set<int> us;
        for (int i = 0; i < 1000; i++) {
            us.insert({rand()});
        }
        unordered_map<int, int> um;
        for (int i = 0; i < 1000; i++) {
            um[rand()] = rand();
        }
        data.push_back(pair<decltype(us), decltype(um)>(us, um));
    }
    ///////////
    auto type = ITypeInfo::getInfo<>(&data);
    w.writeObject(&data, type, *protocol);
    transport->flush();
    close(file);
}

class Muerte {
public:
    ~Muerte() {
        cout << "mori" << endl;
    }
};

*/

#include "executor/core/modules/IPostman.h"
#include "executor/core/IExecutorData.h"
#include <iostream>
#include <thrift/transport/TSocket.h>
#include <thrift/protocol/TCompactProtocol.h>
#include <boost/filesystem.hpp>

using namespace ignis::executor::core;
using namespace ignis::executor::core::modules;
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;





int main(int argc, char *argv[]) {
    std::ofstream out("/mnt/d/text.t");

    out << "hola";
    out.flush();
    out.seekp(0);
    boost::filesystem::resize_file(boost::filesystem::path("/mnt/d/text.t"),0);
    out << "mundo";


/*
    auto data = std::make_shared<IExecutorData>();
    IPostman postman(data);
    postman.start();
    std::shared_ptr<TSocket> socket;
    for (int i = 0; i < 1; i++) {
        socket = std::make_shared<TSocket>("localhost", 1963);
        socket->open();
        auto proto = std::make_shared<TCompactProtocol>(socket);
        proto->writeI64(i);
        for (int b = 1; b < 100; b += 1) {
            uint8_t bb = b * b;
            socket->write(&bb, 1);
        }
        socket->close();
    }
    sleep(6);
    postman.stop();
    auto m0 = data->post_box.popMessage(0)->writeEnd();
    std::cout << m0 << std::endl;*/
    return EXIT_SUCCESS;
}
