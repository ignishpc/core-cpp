
#include "IFilterIterator.h"

using namespace ignis::executor::core::storage;
using namespace ignis::executor::core::storage::iterator;


IReadFilterIterator::IReadFilterIterator(const std::shared_ptr<ICoreReadIterator<IObject::Any>> &it,
                                         const std::shared_ptr<data::handle::IOperator<IObject::Any>> &op)
        : it(it), op(op), next_elem(NULL) {}

IObject::Any &IReadFilterIterator::next() {
    return *next_elem;
}

bool IReadFilterIterator::hasNext() {
    bool check = false;
    while(it->hasNext()){
        next_elem = &it->next();
        if(check = filter()){
            break;
        }
    }
    if(!check){
        next_elem = NULL;
    }
    return next_elem != NULL;
}

bool IReadFilterIterator::isMoved() {
    return it->isMoved();
}



