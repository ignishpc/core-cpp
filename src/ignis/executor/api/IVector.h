
#ifndef IGNIS_IVECTOR_H
#define IGNIS_IVECTOR_H

#include <vector>
#include "ignis/executor/core//io/IReader.h"
#include "ignis/executor/core//io/IWriter.h"
#include "ignis/executor/core/io/IJsonWriter.h"
#include "ignis/executor/core/io/IJsonReader.h"

namespace ignis {
    namespace executor {
        namespace api {

            template<typename Tp>
            struct IVectorType {
                using type=Tp;
                using rtype=Tp;
            };

            template<>
            struct IVectorType<bool> {
                using type=char;
                using rtype=bool;
            };

            /*avoid error of vector<bool> specialization*/
            template<typename Tp, typename Tp2 = typename IVectorType<Tp>::type, typename _Alloc = std::allocator<Tp2>>
            class IVector : public std::vector<Tp2, _Alloc> {
                using std::vector<Tp2, _Alloc>::vector;
            };

        }
        namespace core {
            namespace io {

                template<typename _Alloc>
                struct IWriterType<api::IVector<bool, char, _Alloc>> {
                    inline void writeType(protocol::IProtocol &protocol) {
                        return writer.writeType(protocol);
                    }

                    inline void
                    operator()(protocol::IProtocol &protocol, const api::IVector<bool, char, _Alloc> &obj) {
                        auto size = obj.size();
                        writeSizeAux(protocol, size);
                        IWriterType<bool>().writeType(protocol);
                        auto data = obj.data();
                        for (decltype(size) i = 0; i < size; i++) {
                            protocol.writeBool((bool &) data[i]);
                        }
                    }

                private:
                    IWriterType<std::vector<bool, _Alloc>> writer;

                };

                template<typename Tp, typename Tp2, typename _Alloc>
                struct IWriterType<api::IVector<Tp, Tp2, _Alloc>> {
                    inline void writeType(protocol::IProtocol &protocol) {
                        return writer.writeType(protocol);
                    }

                    inline void
                    operator()(protocol::IProtocol &protocol, const api::IVector<Tp, Tp2, _Alloc> &obj) {
                        writer(protocol, obj);
                    }

                private:
                    IWriterType<std::vector<Tp, _Alloc>> writer;

                };

                template<typename Tp, typename Tp2, typename _Alloc>
                struct IReaderType<api::IVector<Tp, Tp2, _Alloc>> {
                    inline bool readType(protocol::IProtocol &protocol) {
                        return reader.readType(protocol);
                    }

                    inline void operator()(protocol::IProtocol &protocol, api::IVector<Tp, Tp2, _Alloc> &obj) {
                        reader(protocol, obj);
                    }

                    inline api::IVector<Tp, Tp2, _Alloc> operator()(protocol::IProtocol &protocol) {
                        api::IVector<Tp, Tp2, _Alloc> obj;
                        (*this)(protocol, obj);
                        return std::move(obj);
                    }

                private:
                    IReaderType<std::vector<Tp, _Alloc>> reader;
                };

                template<typename _Alloc>
                struct IReaderType<api::IVector<bool, char, _Alloc>> {
                    inline bool readType(protocol::IProtocol &protocol) {
                        return reader.readType(protocol);
                    }

                    inline void operator()(protocol::IProtocol &protocol, api::IVector<bool, char, _Alloc> &obj) {
                        auto size = readSizeAux(protocol);
                        auto init = obj.size();
                        IReaderType<bool>().readType(protocol);
                        obj.resize(init + size);
                        auto data = obj.data() + init;
                        for (decltype(size) i = 0; i < size; i++) {
                            protocol.readBool((bool &) data[i]);
                        }
                    }

                    inline api::IVector<bool, char, _Alloc> operator()(protocol::IProtocol &protocol) {
                        api::IVector<bool, char, _Alloc> obj;
                        (*this)(protocol, obj);
                        return std::move(obj);
                    }

                private:
                    IReaderType<std::vector<bool, _Alloc>> reader;
                };


                template<typename _Alloc>
                struct IJsonReaderType<api::IVector<bool, char, _Alloc>> {
                    inline void operator()(JsonNode &in, api::IVector<bool, char, _Alloc> &obj) {
                        checkJsonTypeAux<api::IVector<bool, _Alloc>>(in.IsArray());
                        IJsonReaderType<bool> reader;
                        auto array = in.GetArray();
                        for (auto &value: array) {
                            obj.push_back(reader(value));
                        }
                    }

                    inline api::IVector<bool, char, _Alloc> operator()(JsonNode &in) {
                        api::IVector<bool, char, _Alloc> obj;
                        (*this)(in, obj);
                        return obj;
                    }
                };

                template<typename Tp, typename Tp2, typename _Alloc>
                struct IJsonReaderType<api::IVector<Tp, Tp2, _Alloc>> {
                    inline void operator()(JsonNode &in, api::IVector<Tp, Tp2, _Alloc> &obj) {
                        reader(in, obj);
                    }

                    inline api::IVector<Tp, Tp2, _Alloc> operator()(JsonNode &in) {
                        api::IVector<Tp, Tp2, _Alloc> obj;
                        (*this)(in, obj);
                        return obj;
                    }

                private:
                    IJsonReaderType<std::vector<Tp, _Alloc>> reader;
                };

                template<typename _Alloc>
                struct IJsonWriterType<api::IVector<bool, char, _Alloc>> {
                    inline void operator()(JsonWriter &out, const api::IVector<bool, char, _Alloc> &obj) {
                        out.StartArray();
                        IJsonWriterType<bool> writer;
                        for (const auto &elem:obj) {
                            writer(out, elem);
                        }
                        out.EndArray();
                    }
                };

                template<typename Tp, typename Tp2, typename _Alloc>
                struct IJsonWriterType<api::IVector<Tp, Tp2, _Alloc>> {
                    inline void operator()(JsonWriter &out, const api::IVector<Tp, Tp2, _Alloc> &obj) {
                        writer(out, obj);
                    }

                private:
                    IJsonWriterType<std::vector<Tp, _Alloc>> writer;
                };

            }
        }
    }
}


#endif
