
#include "IMemoryPartition.h"
#include <utility>
#include "ignis/executor/core/protocol/IObjectProtocol.h"
#include "ignis/executor/core/transport/IZlibTransport.h"

#define IMemoryPartitionClass ignis::executor::core::storage::IMemoryPartition
#define IMemoryReadIteratorClass ignis::executor::core::storage::IMemoryReadIterator
#define IMemoryWriteIteratorClass ignis::executor::core::storage::IMemoryWriteIterator

template<typename Tp>
const std::string IMemoryPartitionClass<Tp>::TYPE = "Memory";

template<typename Tp>
IMemoryPartitionClass<Tp>::IMemoryPartition(size_t size) {
    elements.reserve(size);
}

template<typename Tp>
IMemoryPartitionClass<Tp>::IMemoryPartition(IVector<Tp>&& elements){
    std::swap(this->elements, elements);
}

template<typename Tp>
IMemoryPartitionClass<Tp>::~IMemoryPartition() {}

template<typename Tp>
std::shared_ptr<ignis::executor::api::IReadIterator<Tp>> IMemoryPartitionClass<Tp>::readIterator() {
    return std::make_shared<IMemoryReadIterator < Tp>>
    (elements);
};

template<typename Tp>
std::shared_ptr<ignis::executor::api::IWriteIterator<Tp>> IMemoryPartitionClass<Tp>::writeIterator() {
    return std::make_shared<IMemoryWriteIterator < Tp>>
    (elements);
}

template<typename Tp>
void IMemoryPartitionClass<Tp>::read(std::shared_ptr<transport::ITransport> &trans) {
    auto zlib_trans = std::make_shared<transport::IZlibTransport>(trans);
    protocol::IObjectProtocol proto(zlib_trans);
    proto.readObject < IVector < Tp >> (elements);
}

template<typename Tp>
void IMemoryPartitionClass<Tp>::write(std::shared_ptr<transport::ITransport> &trans, int8_t compression) {
    write(trans, compression, false);
}

template<typename Tp>
void IMemoryPartitionClass<Tp>::write(std::shared_ptr<transport::ITransport> &trans, int8_t compression, bool native) {
    auto zlib_trans = std::make_shared<transport::IZlibTransport>(trans, compression);
    protocol::IObjectProtocol proto(zlib_trans);
    proto.writeObject(elements, native);
    zlib_trans->flush();
}

template<typename Tp>
void IMemoryPartitionClass<Tp>::copyFrom(IPartition <Tp> &source) {
    if (source.type() == TYPE) {
        auto &men_source = reinterpret_cast<IMemoryPartition <Tp> &>(source);
        std::copy(men_source.begin(), men_source.end(), std::back_inserter(elements));
    } else {
        auto it = source.readIterator();
        for (int64_t i = 0; i < source.size(); i++) {
            elements.push_back(std::move(it->next()));
        }
    }
}

template<typename Tp>
void IMemoryPartitionClass<Tp>::moveFrom(IPartition <Tp> &source) {
    if (source.type() == TYPE) {
        auto &men_source = reinterpret_cast<IMemoryPartition <Tp> &>(source);
        std::move(men_source.begin(), men_source.end(), std::back_inserter(elements));
        source.clear();
    } else {
        auto it = source.readIterator();
        for (int64_t i = 0; i < source.size(); i++) {
            elements.push_back(std::move(it->next()));
        }
    }
}

template<typename Tp>
std::shared_ptr<ignis::executor::core::storage::IPartition<Tp>> IMemoryPartitionClass<Tp>::clone() {
    auto newPartition = std::make_shared<IMemoryPartition < Tp>>
    (size());
    this->copyTo(*newPartition);
    return newPartition;
}

template<typename Tp>
size_t IMemoryPartitionClass<Tp>::size() {
    return elements.size();
}

template<typename Tp>
size_t IMemoryPartitionClass<Tp>::bytes() {
    return elements.size() * sizeof(Tp);
}

template<typename Tp>
void IMemoryPartitionClass<Tp>::clear() {
    elements.clear();
}

template<typename Tp>
void IMemoryPartitionClass<Tp>::fit() {
    elements.resize(elements.size());
}

template<typename Tp>
const std::string &IMemoryPartitionClass<Tp>::type() {
    return TYPE;
}

template<typename Tp>
inline Tp &IMemoryPartitionClass<Tp>::operator[](int64_t index) {
    return reinterpret_cast<Tp &>(elements[index]);
}

template<typename Tp>
Tp *IMemoryPartitionClass<Tp>::begin() {
    return reinterpret_cast<Tp *>(&elements[0]);
}

template<typename Tp>
Tp *IMemoryPartitionClass<Tp>::end() {
    return reinterpret_cast<Tp *>(&elements[0] + elements.size());
}

template<typename Tp>
void IMemoryPartitionClass<Tp>::resize(int64_t size) {
    elements.resize(size);
}
template<typename Tp>
ignis::executor::api::IVector <Tp> & IMemoryPartitionClass<Tp>::inner(){
    return elements;
}

template<typename Tp>
IMemoryReadIteratorClass<Tp>::IMemoryReadIterator(IVector <Tp> &elements)
        :elements(elements), index(0) {}

template<typename Tp>
Tp &IMemoryReadIteratorClass<Tp>::next() {
    return reinterpret_cast<Tp &>(elements[index++]);
}

template<typename Tp>
std::shared_ptr<Tp> IMemoryReadIteratorClass<Tp>::nextShared() {
    return std::shared_ptr<Tp>(reinterpret_cast<Tp *>(&elements[index++]), [](Tp *) {});
}

template<typename Tp>
bool IMemoryReadIteratorClass<Tp>::hasNext() {
    return elements.size() > index;
}

template<typename Tp>
IMemoryReadIteratorClass<Tp>::~IMemoryReadIterator() {}

template<typename Tp>
IMemoryWriteIteratorClass<Tp>::IMemoryWriteIterator(IVector <Tp> &elements)
        :elements(elements) {}

template<typename Tp>
void IMemoryWriteIteratorClass<Tp>::write(Tp &obj) {
    elements.emplace_back(obj);
}

template<typename Tp>
void IMemoryWriteIteratorClass<Tp>::write(Tp &&obj) {
    elements.emplace_back(std::forward<Tp>(obj));
}

template<typename Tp>
IMemoryWriteIteratorClass<Tp>::~IMemoryWriteIterator() {}

#undef IMemoryPartitionClass
#undef IMemoryReadIteratorClass
#undef IMemoryWriteIteratorClass