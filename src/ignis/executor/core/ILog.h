
#ifndef IGNIS_ILOG_H
#define IGNIS_ILOG_H

#include <sstream>
#include <string>

#define IGNIS_LOG_INIT() ignis::executor::core::ILog::initLog()

#define IGNIS_LOG_ENABLE(e) ignis::executor::core::ILog::enable(e)

#define IGNIS_LOG(sev) ignis::executor::core::ILog::log(ignis::executor::core::ILog::sev, __FILE__, __LINE__)

#define IGNIS_THREAD_LOG(sev)                                                                                          \
    ignis::executor::core::ILog::threadLog(ignis::executor::core::ILog::sev, __FILE__, __LINE__)

namespace ignis {
    namespace executor {
        namespace core {
            class ILog {
            public:
                enum level { trace, debug, info, warning, error, fatal };

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

                static void enable(bool enable);

            private:
                ILog() {}
            };
        }// namespace core
    }    // namespace executor
}// namespace ignis


#endif
