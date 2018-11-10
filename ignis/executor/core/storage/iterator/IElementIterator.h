
#ifndef IGNIS_IELEMENTITERATOR_H
#define IGNIS_IELEMENTITERATOR_H
#include "../IObject.h"
#include "../../../api/IManager.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace storage {
                namespace iterator {

                    class IElementIterator : public ICoreReadIterator<IObject::Any>,  public ICoreWriteIterator<IObject::Any>{
                    public:

                        IElementIterator(const std::shared_ptr<api::IManager<IObject::Any>> &manager);

                        IElementIterator(const std::shared_ptr<api::IManager<IObject::Any>> &manager,
                                const std::shared_ptr<IObject::Any> &data);

                        virtual IObject::Any &next() override;

                        virtual std::shared_ptr<IObject::Any> nextShared() override;

                        virtual bool hasNext() override;

                        virtual  bool isMoved() override;

                        virtual void skip(size_t elems) override;

                        virtual void write(IObject::Any &obj) override;

                        virtual void write(IObject::Any &&obj) override;

                        virtual ~IElementIterator() override;

                    private:
                        std::shared_ptr<api::IManager<IObject::Any>> manager;
                        std::shared_ptr<IObject::Any> data;
                    };

                }
            }
        }
    }
}
#endif //IGNIS_IELEMENTITERATOR_H
