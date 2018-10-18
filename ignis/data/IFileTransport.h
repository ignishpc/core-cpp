
#ifndef IGNIS_IFILETRANSPORT_H
#define IGNIS_IFILETRANSPORT_H

#include <thrift/transport/TTransport.h>
#include <thrift/transport/TFileTransport.h>
#include <thrift/protocol/TCompactProtocol.h>
#include <string>
#include <iostream>
#include <fstream>
#include <memory>
#include <boost/filesystem.hpp>

namespace ignis {
    namespace data {
        class IFileTransport : public apache::thrift::transport::TTransport {
        public:

            IFileTransport(std::string &read_file, std::string &write_file, std::shared_ptr<TTransport> &sync_trans,
                           size_t block_size = 50 * 1024 * 1024);

            bool isOpen() override;

            void open() override;

            void close() override;

            uint32_t read_virt(uint8_t *buf, uint32_t len) override;

            void write_virt(const uint8_t *buf, uint32_t len) override;

            void flush() override;

            virtual ~IFileTransport();

        private:

            std::shared_ptr<TTransport> sync_trans;
            boost::filesystem::path buffer_path[2];
            std::fstream buffer[2];
            size_t block_size;
            size_t write_bytes;
            int8_t read_flag;
            int8_t write_flag;
            bool init;

            inline void sync();

            inline void swapRead();

            inline void swapWrite();
        };

    }
}
#endif
