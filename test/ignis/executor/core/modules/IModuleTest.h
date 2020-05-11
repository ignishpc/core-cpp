
#ifndef IGNIS_IMODULETEST_H
#define IGNIS_IMODULETEST_H

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include "ignis/executor/core/IExecutorData.h"
#include "ignis/executor/core/IElements.h"
#include "ignis/executor/core/modules/impl/IBaseImpl.h"

namespace ignis {
    namespace executor {
        namespace core {
            namespace modules {
                class IModuleTest : public CPPUNIT_NS::TestCase {
                public:
                    IModuleTest();

                    rpc::ISource newSource(const std::string& name);

                    template<typename Tp>
                    void registerType();

                    template <typename Tp>
                    api::IVector<Tp> rankVector(const api::IVector<Tp>& elems);

                    template <typename Tp>
                    void loadToPartitions(const api::IVector<Tp>& elems, int partitions);

                    template <typename Tp>
                    api::IVector<Tp> getFromPartitions();

                protected:
                    std::string library;
                    std::shared_ptr<IExecutorData> executor_data;
                    std::shared_ptr<modules::impl::IBaseImpl> base_impl;
                };
            }
        }
    }

}

#include "IModuleTest.tcc"
#endif

