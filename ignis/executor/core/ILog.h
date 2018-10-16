
#ifndef IGNIS_ILOG_H
#define IGNIS_ILOG_H

#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes/mutable_constant.hpp>

#define IGNIS_LOG_INIT() ignis::executor::core::ILog::initLog()

#define IGNIS_LOG_ENABLE(enable) ignis::executor::core::ILog::logEnable(enable)

#define IGNIS_LOG(sev) \
   BOOST_LOG_STREAM_WITH_PARAMS( \
      (boost::log::trivial::logger::get()), \
         (ignis::executor::core::ILog::newAttr("File", ignis::executor::core::ILog::filename(__FILE__))) \
         (ignis::executor::core::ILog::newAttr("Line", __LINE__)) \
         (boost::log::keywords::severity = (boost::log::trivial::sev)) \
   )
namespace ignis {
    namespace executor {
        namespace core {
            class ILog {
            public:
                static void initLog();

                template<typename ValueType>
                static ValueType newAttr(const char *name, ValueType value) {
                    auto attr =  boost::log::attribute_cast<boost::log::attributes::mutable_constant<ValueType>>(
                            boost::log::core::get()->get_thread_attributes()[name]);
                    attr.set(value);
                    return attr.get();
                }

                static std::string filename(std::string path);

                static bool logEnable(bool enable);

            private:
                ILog(){}

            };
        }
    }
}


#endif
