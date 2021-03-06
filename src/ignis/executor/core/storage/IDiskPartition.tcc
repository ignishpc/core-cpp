
#include "IDiskPartition.h"
#include <ghc/filesystem.hpp>
#include "ignis/executor/core/exception/IInvalidArgument.h"
#include "ignis/executor/core/transport/IHeaderTransport.h"
#include "ignis/executor/core/transport/IMemoryBuffer.h"

#define IDiskPartitionClass ignis::executor::core::storage::IDiskPartition

template<typename Tp>
const std::string IDiskPartitionClass<Tp>::TYPE = "Disk";

template<typename Tp>
IDiskPartitionClass<Tp>::IDiskPartition(std::string path, int8_t compression, bool persist, bool read)
    : IDiskPartition(std::make_shared<transport::IFileTransport>("/dev/null", false, true), path, compression, persist,
                     read) {}

template<typename Tp>
IDiskPartitionClass<Tp>::IDiskPartition(std::shared_ptr<transport::IFileTransport> &&trans, std::string &path,
                                        int8_t compression, bool persist, bool read)
    : IRawPartition<Tp>((std::shared_ptr<transport::ITransport> &) trans, compression), path(path), file(trans),
      destroy(!persist){
    /*Flush out zlib header*/
    transport::IFileTransport tmp(path, false, true);
    uint8_t byte = 0;
    this->zlib->write(&byte, 1);
    this->zlib->flush();
    int aux = tmp.getFD();
    tmp.setFD(file->getFD());
    file->setFD(aux);
    if (read) {
        auto file_header = std::make_shared<transport::IFileTransport>(path + ".header", true, false);
        auto zlib_header = std::make_shared<transport::IZlibTransport>(file_header);
        this->readHeader((std::shared_ptr<transport::ITransport> &) zlib_header);
    } else {
        clear();
    }
}

template<typename Tp>
std::shared_ptr<ignis::executor::core::storage::IPartition<Tp>> IDiskPartitionClass<Tp>::clone() {
    std::shared_ptr<ignis::executor::core::storage::IPartition<Tp>> newPartition;
    auto newPath = path;
    int64_t i = 0;
#pragma omp critical
    {
        while (ghc::filesystem::exists(newPath + "." + std::to_string(i))) { i++; }
        newPath += "." + std::to_string(i);
        newPartition = std::make_shared<IDiskPartition<Tp>>(newPath, this->compression);
    }
    this->copyTo(*newPartition);
    return newPartition;
}

template<typename Tp>
void IDiskPartitionClass<Tp>::clear() {
    this->zlib->flush();
    this->elems = 0;
    if (::ftruncate64(file->getFD(), 0) != 0) {
        throw exception::ILogicError("error: " + path + " truncate error");
    }
    sync();
}

template<typename Tp>
void IDiskPartitionClass<Tp>::fit() {}

template<typename Tp>
const std::string &IDiskPartitionClass<Tp>::type() {
    return TYPE;
}

template<typename Tp>
size_t IDiskPartitionClass<Tp>::bytes() {
    return ::lseek64(file->getFD(), 0, SEEK_CUR) + this->header_size;
}

template<typename Tp>
void IDiskPartitionClass<Tp>::sync() {
    writeHeader();
    transport::IFileTransport file_header(path + ".header", false, true);
    if (::ftruncate64(file_header.getFD(), 0) != 0) {
        throw exception::ILogicError("error: " + path + ".header truncate error");
    }
    file_header.write((uint8_t *) header.c_str(), this->header_size);
    IRawPartition<Tp>::sync();
}

template<typename Tp>
void IDiskPartitionClass<Tp>::persist(bool p) {
    destroy = !p;
}

template<typename Tp>
const std::string &IDiskPartitionClass<Tp>::getPath() {
    return path;
}

template<typename Tp>
void IDiskPartitionClass<Tp>::rename(const std::string &new_path) {
    file->close();
    int error = std::rename(path.c_str(), new_path.c_str());
    if (error == 0) {
        std::rename((path + ".header").c_str(), (new_path + ".header").c_str());
        path = new_path;
    }
    auto new_file = std::make_shared<transport::IFileTransport>(path, false, true);
    std::swap(*file, *new_file);
    this->sync();//flush and create new header
    if (error != 0) { throw exception::IInvalidArgument("error: " + new_path + " is not a valid storage name"); }
}

template<typename Tp>
IDiskPartitionClass<Tp>::~IDiskPartition() {
    if (destroy) {
        file->close();
        std::remove(path.c_str());
        std::remove((path + ".header").c_str());
    } else {
        sync();
    }
}

template<typename Tp>
void IDiskPartitionClass<Tp>::writeHeader() {
    auto buffer = std::make_shared<transport::IMemoryBuffer>(IDiskPartitionClass<Tp>::HEADER);
    auto zlib_buffer = std::make_shared<transport::IZlibTransport>(buffer, this->compression);
    protocol::IObjectProtocol proto(zlib_buffer);
    proto.writeSerialization();
    IHeader<Tp>().write(proto, this->elems);
    zlib_buffer->flush();
    header = buffer->getBufferAsString();
    this->header_size = header.size();
}

template<typename Tp>
std::shared_ptr<ignis::executor::core::transport::ITransport> IDiskPartitionClass<Tp>::readTransport() {
    auto file_trans = std::make_shared<transport::IFileTransport>(path, true, false);
    return std::make_shared<transport::IHeaderTransport>((std::shared_ptr<transport::ITransport> &) file_trans, header);
}

template<typename Tp>
std::shared_ptr<ignis::executor::core::transport::ITransport> &IDiskPartitionClass<Tp>::transport() {
    return (std::shared_ptr<transport::ITransport> &) file;
}

#undef IDiskPartitionClass
