
#include "IFilterIterator.h"

using namespace std;
using namespace ignis::executor::core::storage;
using namespace ignis::executor::core;


IReadFilterIterator::IReadFilterIterator(const std::shared_ptr<ICoreReadIterator<IObject::Any>> &it,
                                         const shared_ptr<data::IOperator<IObject::Any>> &op)
        : it(it), op(op), next_elem(NULL) {}

IObject::Any &IReadFilterIterator::next() {
    return *next_elem;
}

bool IReadFilterIterator::hashNext() {
    bool check = false;
    while(it->hashNext()){
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



