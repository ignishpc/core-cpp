
#ifndef UNTITLED_IREADER_H
#define UNTITLED_IREADER_H

#include <memory>
#include <unordered_map>
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

                std::pair<std::shared_ptr<void>,ITypeInfo> readObject(apache::thrift::protocol::TProtocol &tProtocol);

                virtual ~IReader() {};

            protected:

                typedef char Any;

                typedef void(IReader::*F_Reader)(void *obj, ITypeInfo &type,
                                                 apache::thrift::protocol::TProtocol &tProtocol);
                typedef size_t(IReader::*F_Hash)(void* obj, ITypeInfo &type);

                typedef struct {
                    F_Reader function;
                    F_Hash hash;
                    size_t class_id;
                    std::string class_name;
                    std::shared_ptr<IContainer<>> container;
                    std::shared_ptr<IExtendedType<>> extended_type;
                    std::shared_ptr<IHandle<>> handle;
                    std::unordered_map<__uint8_t, std::shared_ptr<IHandle<>>> handles;
                } IReaderInfo;

                void readVoid(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                void readBool(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                void readI08(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                void readI16(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                void readI32(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                void readI64(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                void readDouble(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                void readString(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                void readList(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                size_t hashList(void* obj, ITypeInfo &type);

                void readSet(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                size_t hashSet(void* obj, ITypeInfo &type);

                void readMap(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                size_t hashMap(void* obj, ITypeInfo &type);

                void readBinary(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                void readTuple(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                size_t hashTuple(void* obj, ITypeInfo &type);

                __uint8_t readType(apache::thrift::protocol::TProtocol &tProtocol);

                size_t readSize(apache::thrift::protocol::TProtocol &tProtocol);

                template <typename T>
                size_t defaultHash(void *obj, ITypeInfo &type){
                    return std::hash<T>()(*(T*)obj);
                }

                template<typename C>
                F_Reader castReader(C reader) {
                    return static_cast<F_Reader>(reader);
                }

                template<typename C>
                F_Hash castHash(C f_hash) {
                    return static_cast<F_Hash>(f_hash);
                }

                template<typename T, typename I = ITypeInfo>
                void newType(__uint8_t id, F_Reader function) {
                    if (info_map.find(id) == info_map.end()) {
                        //Error Already exists
                    }
                    auto &entry = info_map[id];
                    entry.container = std::shared_ptr<IContainer<>>((IContainer<> *) new IContainer<T>());
                    auto type = ITypeInfo::getInfo<T>();
                    entry.class_id = type.getId();
                    entry.class_name = type.nameId();
                }

                template<typename T, typename I = ITypeInfo, typename E = IExtendedType<T>>
                void newExtendedType(__uint8_t id, F_Reader function, F_Hash hash = &IReader::defaultHash<T>) {
                    newType<T, I>(id, function);
                    info_map[id].hash = hash;
                    info_map[id].extended_type = std::shared_ptr<IExtendedType<>>((IExtendedType<> *) new E());
                }

                void updateInfo();

                const IReaderInfo &getInfo(__uint8_t id);

            private:
                ITypeInfo makeType(size_t class_id, std::string &&class_name);

                std::unordered_map<__uint8_t, IReaderInfo> info_map;
            };
        }
    }
}

#endif
