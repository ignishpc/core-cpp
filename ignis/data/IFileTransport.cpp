
#include "IFileTransport.h"

using namespace std;
using namespace apache::thrift::transport;
using namespace ignis::data;

IFileTransport::IFileTransport(std::string &read_file, std::string &write_file, shared_ptr<TTransport> &sync,
                               size_t blockSize) : sync_trans(sync_trans),
                                                   block_size(block_size),
                                                   write_bytes(blockSize),
                                                   read_flag(0),
                                                   write_flag(1) {
    buffer[read_flag].open(read_file, ios::binary | ios::trunc);
    buffer[write_flag].open(write_file, ios::binary | ios::trunc);
}

bool IFileTransport::isOpen() {
    return sync_trans->isOpen();
}

void IFileTransport::open() {
    if (!sync_trans->isOpen()) {
        sync_trans->open();
    }
}

void IFileTransport::close() {
    buffer[read_flag].close();
    buffer[write_flag].close();
    sync_trans->close();
}

uint32_t IFileTransport::read_virt(uint8_t *buf, uint32_t len) {
    auto read_sz = buffer[read_flag].readsome((char *) buf, len);
    if (read_sz == 0) {
        swapRead();
        read_sz = buffer[read_flag].readsome((char *) buf, len);
    }
    return read_sz;
}

void IFileTransport::write_virt(const uint8_t *buf, uint32_t len) {
    if (write_bytes + len > block_size) {
        do {
            auto send = block_size - write_bytes;
            buffer[write_flag].write((char *) buf, send);
            len -= send;
            buf += send;
            swapWrite();
        } while (len > block_size);
    }
    buffer[write_flag].write((char *) buf, len);
    write_bytes += len;
}

void IFileTransport::flush() {
    if (write_bytes > 0) {
        TTransport::flush();
    }
}


void IFileTransport::sync() {
    uint8_t buf[1];
    sync_trans->write(buf, 1);
    sync_trans->flush();
    sync_trans->read(buf, 1);
}

void IFileTransport::swapRead() {
    sync();
    read_flag %= read_flag + 1;
    write_flag %= write_flag + 1;
    buffer[read_flag].seekg(0);
}


void IFileTransport::swapWrite() {
    sync();
    buffer[write_flag].flush();
    read_flag %= read_flag + 1;
    write_flag %= write_flag + 1;
    buffer[write_flag].seekg(0);
    boost::filesystem::resize_file(buffer_path[write_flag], 0);
    write_bytes = 0;
}
