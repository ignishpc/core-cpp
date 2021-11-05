
#ifndef IGNIS_IDISKPARTITION_H
#define IGNIS_IDISKPARTITION_H

#include "IRawPartition.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace storage {
                template<typename Tp>
                class IDiskPartition : public IRawPartition<Tp> {
                public:
                    typedef Tp value_type;
                    static const std::string TYPE;

                    IDiskPartition(std::string path, int8_t compression = 6, bool persist = false, bool read = false);

                    virtual std::shared_ptr<IPartition<Tp>> clone();

                    virtual void clear();

                    virtual void fit();

                    virtual const std::string &type();

                    virtual size_t bytes();

                    virtual void sync();

                    virtual void persist(bool p);

                    virtual const std::string &getPath();

                    virtual void rename(const std::string &new_path);

                    virtual ~IDiskPartition();

                private:
                    friend class core::IMpi;

                    IDiskPartition(std::shared_ptr<transport::IFileTransport> &&trans, std::string &path,
                                   int8_t compression, bool persist, bool read);

                    virtual void writeHeader();

                    virtual std::shared_ptr<transport::ITransport> readTransport();

                    virtual std::shared_ptr<transport::ITransport> &transport();

                    std::shared_ptr<transport::IFileTransport> file;
                    std::string path;
                    std::string header;
                    bool destroy;
                };
            }// namespace storage
        }    // namespace core
    }        // namespace executor
}// namespace ignis

#include "IDiskPartition.tcc"

#endif
