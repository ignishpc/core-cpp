
#ifndef UNTITLED_IREADER_H
#define UNTITLED_IREADER_H

#include <memory>
#include <unordered_map>
#include <vector>
#include <thrift/protocol/TProtocol.h>
#include "../ITypeInfo.h"
#include "IEnumTypes.h"
#include "IHandle.h"
#include "IContainer.h"
#include "IExtendedType.h"


namespace ignis {
    namespace data {
        namespace serialization {

            class IReader {

            public:

                IReader();

                virtual ~IReader(){};

            protected:
                typedef char Any;

                typedef void(*F_Reader)(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                static void readVoid(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                static void readBool(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                static void readI08(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                static void readI16(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                static void readI32(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                static void readI64(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                static void readDouble(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                static void readString(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                static void readList(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                static void readSet(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                static void readMap(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                static void readBinary(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                static void readTuple(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                static void readCustom(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                template<typename T>
                __uint8_t addType(F_Reader reader) {
                    IContainer<T> c;
                    readers.push_back(reader);
                    containers.push_back((IContainer<> &&) c);
                    return readers.size() - 1;//id
                }

                template<typename T>
                void addExtendedType() {
                    IExtendedType<T> et;
                    extended_types.push_back((IExtendedType<> &&) et);
                }

                void updateHandles();

                IHandle<> &getHandle(__uint8_t ti, __uint8_t tj);

                F_Reader &getReader(__uint8_t ti);

            private:

                std::vector<IContainer<>> containers;
                std::vector<IExtendedType<>> extended_types;
                std::vector<std::vector<std::shared_ptr<IHandle<>>>> handles;
                std::vector<F_Reader> readers;

            };
        }
    }
}

#endif
