
#ifndef IGNIS_ILOG_H
#define IGNIS_ILOG_H

#include <sstream>
#include <string>

#define IGNIS_LOG_INIT() ignis::executor::core::ILog::initLog()

#define IGNIS_LOG_ENABLE(e) ignis::executor::core::ILog::enable(e)

#define IGNIS_LOG(sev) {/*Prevent =*/}ignis::executor::core::ILog(ignis::executor::core::ILog::sev, __FILE__, __LINE__)

namespace ignis {
    namespace executor {
        namespace core {
            class ILog : public std::ostringstream {
            public:
                enum level {
                    trace, debug, info, warning, error, fatal
                };

                static void initLog();

                static void enable(bool enable);

                ILog(level l, const std::string &file, int line);

                ~ILog();

            private:
                const std::string file;
                const int line;
                const level l;
            };
        }// namespace core
    }    // namespace executor
}// namespace ignis


#endif
