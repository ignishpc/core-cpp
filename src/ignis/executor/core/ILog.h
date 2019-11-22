
#ifndef IGNIS_ILOG_H
#define IGNIS_ILOG_H

#include <string>
#include <sstream>

#define IGNIS_LOG_INIT() ignis::executor::core::ILog::initLog()

#define IGNIS_LOG_ENABLE(enable) ignis::executor::core::ILog::logEnable(enable)

#define IGNIS_LOG(sev) ignis::executor::core::ILog::log(ignis::executor::core::ILog::sev, __FILE__, __LINE__)

#define IGNIS_THREAD_LOG(sev) ignis::executor::core::ILog::threadLog(ignis::executor::core::ILog::sev, __FILE__, __LINE__)

namespace ignis {
    namespace executor {
        namespace core {
            class ILog {
            public:
                enum level {
                    trace,
                    debug,
                    info,
                    warning,
                    error,
                    fatal
                };

                class Streamlog : public std::ostringstream {
                    friend ILog;
                    const std::string &file;
                    const int &line;
                    const bool thread;
                    const level l;
                    bool flush;

                    Streamlog(const Streamlog &&other);

                    Streamlog(level l, bool thread, const std::string &file, const int &line);

                    void write();

                public:
                    virtual ~Streamlog();
                };

                static void initLog();

                static Streamlog threadLog(level l, const std::string &file, const int &line);

                static Streamlog log(level l, const std::string &file, const int &line);

                static bool logEnable(bool enable);

            private:
                ILog() {}

            };
        }
    }
}


#endif
