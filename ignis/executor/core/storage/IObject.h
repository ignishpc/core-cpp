
#ifndef IGNIS_IOBJECT_H
#define IGNIS_IOBJECT_H

#include "../../../IHeaders.h"
#include "../../api/IManager.h"
#include "iterator/ICoreIterator.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace storage {
                class IObject {
                public:
                    typedef char Any;

                    virtual ~IObject() {};

                    virtual std::shared_ptr<iterator::ICoreReadIterator<Any>> readIterator() = 0;

                    virtual std::shared_ptr<iterator::ICoreWriteIterator<Any>> writeIterator() = 0;

                    virtual void read(std::shared_ptr<transport::TTransport> trans) = 0;

                    virtual void write(std::shared_ptr<transport::TTransport> trans, int8_t compression) = 0;

                    virtual void copyFrom(IObject &source) = 0;

                    virtual void copyTo(IObject &target) { target.copyFrom(*this); }

                    virtual void moveFrom(IObject &source) = 0;

                    virtual void moveTo(IObject &target) { target.moveFrom(*this); }

                    virtual size_t getSize() = 0;

                    virtual bool hasSize() { return true; }

                    virtual void clear() {}

                    virtual void fit() {}

                    virtual std::string getType() = 0;

                    virtual std::shared_ptr<api::IManager<Any>> getManager() {
                        return manager;
                    }

                    virtual IObject& setManager(std::shared_ptr<api::IManager<Any>> manager) {
                        this->manager = manager;
                        return *this;
                    }

                    template<typename T>
                    inline static Any &toAny(T &obj) {
                        return (Any &) obj;
                    }

                    template<typename T>
                    inline static std::shared_ptr<T> toShared(T* elem, const std::shared_ptr<api::IManager<T>>& manager){
                        return std::shared_ptr<T>(elem,[&manager](T* elem){
                            (*manager->deleter())(elem);
                        });
                    }

                protected:
                    std::shared_ptr<api::IManager<Any>> manager;

                };
            }
        }
    }
}

#endif
