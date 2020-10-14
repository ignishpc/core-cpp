
#include "IHeaderTransport.h"
#include <algorithm>
#include <cstring>

using namespace ignis::executor::core::transport;


IHeaderTransport::IHeaderTransport(std::shared_ptr<transport::ITransport> &trans, std::string &header)
    : trans(trans), header(header), pos(0) {}

IHeaderTransport::~IHeaderTransport() {}

uint32_t IHeaderTransport::read(uint8_t *buf, uint32_t len) {
    if (header.empty()) { return trans->read_virt(buf, len); }
    int bytes = std::min((uint32_t)(header.size() - pos), len);
    std::memcpy(buf, header.c_str() + pos, bytes);
    if (bytes == header.size()) { header.clear(); }
    return bytes;
}

void IHeaderTransport::write(const uint8_t *buf, uint32_t len) { trans->write_virt(buf, len); }

const uint8_t *IHeaderTransport::borrow(uint8_t *buf, uint32_t *len) {
    if (header.empty()) { return nullptr; }
    return trans->borrow_virt(buf, len);
}

void IHeaderTransport::consume(uint32_t len) { trans->consume_virt(len); }
