
#ifndef IGNIS_IVECTOR_H
#define IGNIS_IVECTOR_H

#include "ignis/executor/core//io/IReader.h"
#include "ignis/executor/core//io/IWriter.h"
#include "ignis/executor/core/io/IJsonReader.h"
#include "ignis/executor/core/io/IJsonWriter.h"
#include "ignis/executor/core/io/INativeReader.h"
#include "ignis/executor/core/io/INativeWriter.h"
#include "ignis/executor/core/io/IPrinter.h"
#include <vector>

namespace ignis {
    namespace executor {
        namespace api {

            template<typename Tp>
            struct IVectorType {
                using type = Tp;
                using rtype = Tp;
            };

            template<>
            struct IVectorType<bool> {
                using type = char;
                using rtype = bool;
            };

            template<typename Tp>
            class IViewAllocator : public std::allocator<Tp> {
            private:
                using Base = std::allocator<Tp>;
                using Pointer = typename std::allocator_traits<Base>::pointer;
                using SizeType = typename std::allocator_traits<Base>::size_type;

            public:
                IViewAllocator() {}

                IViewAllocator(const std::allocator<typename IVectorType<Tp>::type> &) {}

                Pointer allocate(SizeType n, const void * = static_cast<const void *>(0)) {
                    throw executor::core::exception::ILogicError("A view cannot be resized");
                }

                void deallocate(Pointer p, SizeType n) {
                    throw executor::core::exception::ILogicError("A view cannot be resized");
                }

                template<class U>
                struct rebind {
                    using other = IViewAllocator<U>;
                };
            };

            template<typename Tp>
            struct is_view {
                const static bool value = false;
            };

            template<typename Tp>
            struct is_view<IViewAllocator<Tp>> {
                const static bool value = true;
            };

            /*avoid error of vector<bool> specialization*/
            template<typename Tp, typename _Alloc = std::allocator<typename IVectorType<Tp>::type>>
            class IVector : public std::vector<typename IVectorType<Tp>::type, _Alloc> {
            private:
                using Base = std::vector<typename IVectorType<Tp>::type, _Alloc>;
                using Pointer = typename Base::pointer;
                template<typename Tp2, typename _Alloc2>
                friend class IVector;
                IVector(Tp *init, size_t sz) {
                    this->_M_impl._M_start = (Pointer) init;
                    this->_M_impl._M_finish = (Pointer) init + sz;
                    this->_M_impl._M_end_of_storage = this->_M_impl._M_finish;
                }

            public:
                using std::vector<typename IVectorType<Tp>::type, _Alloc>::vector;

                IVector<Tp, IViewAllocator<typename IVectorType<Tp>::type>> view(size_t begin, size_t end) {
                    return IVector<Tp, IViewAllocator<typename IVectorType<Tp>::type>>(
                            (Tp *) (this->_M_impl._M_start + begin), (Tp *) (end - begin));
                }

                inline static IVector<Tp, IViewAllocator<typename IVectorType<Tp>::type>> view(Tp *init, size_t sz) {
                    return IVector<Tp, IViewAllocator<typename IVectorType<Tp>::type>>(init, sz);
                }

                ~IVector() {
                    if (is_view<_Alloc>::value) {
                        this->_M_impl._M_start = (Pointer) nullptr;
                        this->_M_impl._M_finish = (Pointer) nullptr;
                        this->_M_impl._M_end_of_storage = nullptr;
                    }
                }
            };
        }// namespace api
        namespace core {
            namespace io {

                template<typename _Alloc>
                struct IWriterType<api::IVector<bool, _Alloc>> {
                    inline void writeType(protocol::IProtocol &protocol) { return writer.writeType(protocol); }

                    inline void operator()(protocol::IProtocol &protocol, const api::IVector<bool, _Alloc> &obj) {
                        auto size = obj.size();
                        writeSizeAux(protocol, size);
                        IWriterType<bool>().writeType(protocol);
                        auto data = obj.data();
                        for (decltype(size) i = 0; i < size; i++) { protocol.writeBool((bool &) data[i]); }
                    }

                private:
                    IWriterType<std::vector<bool, _Alloc>> writer;
                };

                template<typename Tp, typename _Alloc>
                struct IWriterType<api::IVector<Tp, _Alloc>> {
                    inline void writeType(protocol::IProtocol &protocol) { return writer.writeType(protocol); }

                    inline void operator()(protocol::IProtocol &protocol, const api::IVector<Tp, _Alloc> &obj) {
                        writer(protocol, obj);
                    }

                private:
                    IWriterType<std::vector<Tp, _Alloc>> writer;
                };

                template<typename Tp, typename _Alloc>
                struct INativeWriterType<api::IVector<Tp, _Alloc>> {

                    inline void operator()(protocol::IProtocol &protocol, const api::IVector<Tp, _Alloc> &obj) {
                        writer(protocol, obj);
                    }

                private:
                    INativeWriterType<std::vector<Tp, _Alloc>> writer;
                };

                template<typename _Alloc>
                struct INativeWriterType<api::IVector<bool, _Alloc>> {

                    inline void operator()(protocol::IProtocol &protocol, const api::IVector<bool, _Alloc> &obj) {
                        protocol.writeBool(true);
                        protocol.writeI64(obj.size());
                        protocol.getTransport()->write((uint8_t *) &obj[0], (uint32_t) obj.size() * sizeof(char));
                    }
                };

                template<typename Tp, typename _Alloc>
                struct IReaderType<api::IVector<Tp, _Alloc>> {
                    inline bool readType(protocol::IProtocol &protocol) { return reader.readType(protocol); }

                    inline void operator()(protocol::IProtocol &protocol, api::IVector<Tp, _Alloc> &obj) {
                        reader(protocol, obj);
                    }

                    inline api::IVector<Tp, _Alloc> operator()(protocol::IProtocol &protocol) {
                        api::IVector<Tp, _Alloc> obj;
                        (*this)(protocol, obj);
                        return std::move(obj);
                    }

                private:
                    IReaderType<std::vector<Tp, _Alloc>> reader;
                };

                template<typename Tp, typename _Alloc>
                struct INativeReaderType<api::IVector<Tp, _Alloc>> {
                    inline void operator()(protocol::IProtocol &protocol, api::IVector<Tp, _Alloc> &obj) {
                        reader(protocol, obj);
                    }

                    inline api::IVector<Tp, _Alloc> operator()(protocol::IProtocol &protocol) {
                        api::IVector<Tp, _Alloc> obj;
                        (*this)(protocol, obj);
                        return std::move(obj);
                    }

                private:
                    INativeReaderType<std::vector<Tp, _Alloc>> reader;
                };

                template<typename _Alloc>
                struct INativeReaderType<api::IVector<bool, _Alloc>> {
                    inline void operator()(protocol::IProtocol &protocol, api::IVector<bool, _Alloc> &obj) {
                        int64_t len;
                        protocol.readI64(len);
                        auto first = obj.size();
                        obj.resize(len);
                        protocol.getTransport()->read((uint8_t *) &obj[first], (uint32_t) len * sizeof(char));
                    }

                    inline api::IVector<bool, _Alloc> operator()(protocol::IProtocol &protocol) {
                        api::IVector<bool, _Alloc> obj;
                        (*this)(protocol, obj);
                        return obj;
                    }
                };

                template<typename _Alloc>
                struct IReaderType<api::IVector<bool, _Alloc>> {
                    inline bool readType(protocol::IProtocol &protocol) { return reader.readType(protocol); }

                    inline void operator()(protocol::IProtocol &protocol, api::IVector<bool, _Alloc> &obj) {
                        auto size = readSizeAux(protocol);
                        auto init = obj.size();
                        IReaderType<bool>().readType(protocol);
                        obj.resize(init + size);
                        auto data = obj.data() + init;
                        for (decltype(size) i = 0; i < size; i++) { protocol.readBool((bool &) data[i]); }
                    }

                    inline api::IVector<bool, _Alloc> operator()(protocol::IProtocol &protocol) {
                        api::IVector<bool, _Alloc> obj;
                        (*this)(protocol, obj);
                        return std::move(obj);
                    }

                private:
                    IReaderType<std::vector<bool, _Alloc>> reader;
                };


                template<typename _Alloc>
                struct IJsonReaderType<api::IVector<bool, _Alloc>> {
                    inline void operator()(JsonNode &in, api::IVector<bool, _Alloc> &obj) {
                        checkJsonTypeAux<api::IVector<bool, _Alloc>>(in.IsArray());
                        IJsonReaderType<bool> reader;
                        auto array = in.GetArray();
                        for (auto &value : array) { obj.push_back(reader(value)); }
                    }

                    inline api::IVector<bool, _Alloc> operator()(JsonNode &in) {
                        api::IVector<bool, _Alloc> obj;
                        (*this)(in, obj);
                        return obj;
                    }
                };

                template<typename _Alloc>
                struct IPrinterType<api::IVector<bool, _Alloc>> {
                    inline void operator()(std::ostream &out, const api::IVector<bool, _Alloc> &obj, int64_t level) {
                        printer(out, (std::vector<char, _Alloc> &) obj, level);
                    }

                private:
                    IPrinterType<std::vector<char, _Alloc>> printer;
                };

                template<typename Tp, typename _Alloc>
                struct IPrinterType<api::IVector<Tp, _Alloc>> {
                    inline void operator()(std::ostream &out, const api::IVector<Tp, _Alloc> &obj, int64_t level) {
                        printer(out, obj, level);
                    }

                private:
                    IPrinterType<std::vector<Tp, _Alloc>> printer;
                };

                template<typename Tp, typename _Alloc>
                struct IJsonReaderType<api::IVector<Tp, _Alloc>> {
                    inline void operator()(JsonNode &in, api::IVector<Tp, _Alloc> &obj) { reader(in, obj); }

                    inline api::IVector<Tp, _Alloc> operator()(JsonNode &in) {
                        api::IVector<Tp, _Alloc> obj;
                        (*this)(in, obj);
                        return obj;
                    }

                private:
                    IJsonReaderType<std::vector<Tp, _Alloc>> reader;
                };

                template<typename _Alloc>
                struct IJsonWriterType<api::IVector<bool, _Alloc>> {
                    inline void operator()(JsonWriter &out, const api::IVector<bool, _Alloc> &obj) {
                        out.StartArray();
                        IJsonWriterType<bool> writer;
                        for (const auto &elem : obj) { writer(out, elem); }
                        out.EndArray();
                    }
                };

                template<typename Tp, typename _Alloc>
                struct IJsonWriterType<api::IVector<Tp, _Alloc>> {
                    inline void operator()(JsonWriter &out, const api::IVector<Tp, _Alloc> &obj) { writer(out, obj); }

                private:
                    IJsonWriterType<std::vector<Tp, _Alloc>> writer;
                };

            }// namespace io
        }// namespace core
    }// namespace executor
}// namespace ignis


#endif
