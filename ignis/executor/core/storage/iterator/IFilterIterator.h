
#ifndef IGNIS_IFILTERITERATOR_H
#define IGNIS_IFILTERITERATOR_H

#include "../IObject.h"
#include "../../../../data/handle/IOperators.h"
#include "../../../api/IReadIterator.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace storage {
                namespace iterator {
                    class IReadFilterIterator : public ICoreReadIterator<IObject::Any> {
                    public:
                        IObject::Any &next() override;

                        bool hashNext() override;

                        bool isMoved() override;

                    protected:

                        virtual bool filter() = 0;

                        IReadFilterIterator(const std::shared_ptr<ICoreReadIterator<IObject::Any>> &it,
                                            const std::shared_ptr<data::handle::IOperator<IObject::Any>> &op);

                        std::shared_ptr<ICoreReadIterator<IObject::Any>> it;
                        std::shared_ptr<data::handle::IOperator<IObject::Any>> op;
                        IObject::Any *next_elem;
                    };
                }
            }
        }
    }
}

#endif
