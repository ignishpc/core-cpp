#include <cstdlib>
#include <iostream>
#include "data/serialization/IReader.h"
#include "data/serialization/IWriter.h"
#include "executor/api/IgnisExecutorException.h"
#include "driver/api/IgnisException.h"

using namespace std;
using namespace ignis::executor::api;
using namespace ignis::driver::api;

void f2() {
    throw IgnisExecutorException("hola");
}

void f() {
    try {
        f2();
    } catch (IgnisExecutorException &ex) {
        throw IgnisExecutorException("hola", ex);
    }
    try {
        f2();
    } catch (IgnisExecutorException &ex) {
        throw IgnisExecutorException("hola", ex);
    }
}

int main(int argc, char *argv[]) {

    ignis::data::serialization::IReader r;
    ignis::data::serialization::IWriter w;

    try {
        f();
    } catch (IgnisExecutorException &ex) {
        auto ex2 = IgnisException(ex.what(), ex.getStacktrace());
        cout << ex2;
    }

    return EXIT_SUCCESS;
}

