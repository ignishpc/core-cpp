#include <cstdlib>
#include <iostream>
#include "data/serialization/IReader.h"
#include "data/serialization/IWriter.h"
#include "executor/api/IgnisExecutorException.h"
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

ignis::data::serialization::IReader r;
ignis::data::serialization::IWriter w;

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

class Test {
public:
    std::shared_ptr<Muerte> a = make_shared<Muerte>();

    /*~Test(){
        cout << "mori" << endl;
    }*/
};

template<typename T>
void destructor(T *obj) {
    delete obj;
}


int main(int argc, char *argv[]) {
    //write();
    read();

    void *t = new Test();
    void (*des)(void *obj) = (void (*)(void *)) &destructor<Test>;

    des(t);

    return EXIT_SUCCESS;
}

