
#ifndef IGNIS_IWRITER_H
#define IGNIS_IWRITER_H

#include <memory>
#include <unordered_map>
#include <thrift/protocol/TProtocol.h>
#include "ITypeInfo.h"
#include "../serialization2/IEnumTypes.h"
#include "IContainer.h"
#include <list>
#include <forward_list>
#include <set>
#include <map>

namespace ignis {
    namespace data {
        namespace serialization {
            class IWriter {
            public:

                IWriter();

                void writeObject(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                virtual ~IWriter() {};

            protected:

                typedef void(IWriter::*F_Writer)(void *obj, ITypeInfo &type,
                                                 apache::thrift::protocol::TProtocol &tProtocol);

                typedef void(IWriter::*F_Type)(__uint8_t id, ITypeInfo &type,
                                               apache::thrift::protocol::TProtocol &tProtocol);

                typedef size_t(IWriter::*F_Size)(ITypeInfo &type);

                typedef struct {
                    F_Writer function;
                    F_Type type;
                    F_Size size;
                    int8_t id;
                } IWriterInfo;

                //
                void writeBool(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                void writeI08(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                void writeI16(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                void writeI32(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                void writeI64(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                void writeDouble(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                void writeString(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                void writeList(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                void writeTypeList(__uint8_t id, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                void writeSet(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                void writeMap(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                void writeTuple(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                size_t getSizeTuple(ITypeInfo &type);

                void writeType(__uint8_t id, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                void writeSize(size_t size, apache::thrift::protocol::TProtocol &tProtocol);

                template <typename T>
                size_t getSize(ITypeInfo &type){
                    return sizeof(T);
                }

                //Other C++ types
                void writeUI08(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                void writeUI16(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                void writeUI32(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                void writeUI64(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                void writeFloat(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                void writeCppList(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                void writeForwardList(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                void writeTreeSet(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                void writeTreeMap(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                void writePointer(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                void writeSharedPointer(void *obj, ITypeInfo &type, apache::thrift::protocol::TProtocol &tProtocol);

                template<typename T, typename I = ITypeInfo>
                void newType(int8_t id, F_Writer function, F_Type type = &IWriter::writeType, F_Size size = &IWriter::getSize<T>) {
                    auto type_info = ITypeInfo::getInfo<T>();
                    if (info_map.find(type_info.getId()) == info_map.end()) {
                        //Error Already exists
                    }
                    auto &entry = info_map[type_info.getId()];
                    entry.type = type;
                    entry.function = function;
                    entry.size = size;
                    entry.id = id;
                }

                const IWriterInfo &getInfo(size_t id);

            private:

                std::unordered_map<size_t, IWriterInfo> info_map;
            };

        }
    }
}

#endif
