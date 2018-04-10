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

#include "data/serialization/ITypeHandle.h"
#include <thrift/protocol/TProtocol.h>
#include <jsoncpp/json/json.h>
#include <iostream>

class Test {

};

int main(int argc, char *argv[]) {
    //write();
    //read();
    std::vector<bool> v;
    v.push_back(true);
    v.push_back(false);
    v.push_back(true);
    v.push_back(false);
    ignis::data::serialization::ITypeHandle<std::vector<Test>> a;
    ignis::data::serialization::ITypeHandle<std::vector<uint8_t >> test;

    Json::Value root;
    std::vector<Test> t;
    t.push_back(Test());
    a.printer().printJson(t, root);
/*
    std::cout << root << std::endl;

    a.printer()(v,std::cout);
*/

    //



    //void *t = new Test();
    //void (*des)(void *obj) = (void (*)(void *)) &destructor<Test>;

    //des(t);

    return EXIT_SUCCESS;
}

namespace ignis {
    namespace data {
        namespace serialization {


            template<>
            struct IPrinter<Test> {
                void operator()(const Test &b, std::ostream &out, size_t level = 0){

                }

                void printJson(const Test &b, Json::Value &json) {}
            };
        }
    }
}