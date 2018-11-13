
#include "ILog.h"
#include <boost/log/expressions.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/attributes/mutable_constant.hpp>
#include <boost/filesystem.hpp>
#include <mutex>
#include <signal.h>
#include "../../exceptions/IException.h"


using namespace ignis::executor::core;

namespace logging  = boost::log;
namespace attrs    = boost::log::attributes;

int parse[] = {
        logging::trivial::trace,
        logging::trivial::debug,
        logging::trivial::info,
        logging::trivial::warning,
        logging::trivial::error,
        logging::trivial::fatal
};

std::mutex log_mutex;

void ILog::initLog() {
    safeDumpRegister();
    logging::core::get()->add_global_attribute("File", attrs::mutable_constant<std::string>(""));
    logging::core::get()->add_global_attribute("Line", attrs::mutable_constant<int>(0));

    logging::add_console_log(std::clog, logging::keywords::format = (boost::log::expressions::stream
                                     << logging::expressions::format_date_time<boost::posix_time::ptime>("TimeStamp", "%B %e, %Y %I:%M:%S %p")
                                     << ": <" << boost::log::trivial::severity << "> "
                                     << '[' << logging::expressions::attr<std::string>("File")
                                     << ':' << logging::expressions::attr<int>("Line") << "] "
                                     << logging::expressions::smessage
                             )
    );
    logging::add_common_attributes();
}


std::string filename(std::string path) {
    return path.substr(path.find_last_of("/\\") + 1);
}

template<typename ValueType>
ValueType newAttr(const char *name, ValueType value) {
    auto attr = boost::log::attribute_cast<boost::log::attributes::mutable_constant<ValueType>>(
            boost::log::core::get()->get_global_attributes()[name]);
    attr.set(value);
    return attr.get();
}

ILog::Streamlog::Streamlog(level l, bool thread, const std::string &file, const int &line)
        : l(l), file(file), line(line), flush(true), thread(thread) {}

ILog::Streamlog::Streamlog(const ILog::Streamlog &&other) :
        l(other.l), file(other.file), line(other.line), thread(other.thread), flush(true) {
    ((ILog::Streamlog &) other).flush = false;
}

void ILog::Streamlog::write() {
    BOOST_LOG_STREAM_WITH_PARAMS((boost::log::trivial::logger::get()),
                                 (newAttr<std::string>("File", filename(file)))(newAttr<int>("Line", line))
                                         (boost::log::keywords::severity = (logging::trivial::severity_level) l)
    ) << this->str();
}

ILog::Streamlog::~Streamlog() {
    if (flush) {
        if (thread) {
            std::lock_guard<std::mutex> lock(log_mutex);
            write();
        } else {
            write();
        }
    }
}


ILog::Streamlog ILog::threadLog(level l, const std::string &file, const int &line) {
    return Streamlog((level) parse[l], true, file, line);
}

ILog::Streamlog ILog::log(level l, const std::string &file, const int &line) {
    return Streamlog((level) parse[l], false, file, line);
}


bool ILog::logEnable(bool enable) {
    logging::core::get()->set_logging_enabled(enable);
}

#define DUMP_FILE "/tmp/ignis-cpp-core.dump"
void signalHandler(int signum) {
    ::signal(signum, SIG_DFL);
    boost::stacktrace::safe_dump_to(DUMP_FILE);
    ::raise(SIGABRT);
}

void ILog::safeDumpRegister() {
    if (boost::filesystem::exists(DUMP_FILE)) {
        std::ifstream ifs(DUMP_FILE);

        boost::stacktrace::stacktrace st = boost::stacktrace::stacktrace::from_dump(ifs);
        std::cerr << "Previous execution crashed log found:" << std::endl << st << std::endl;

        ifs.close();
        boost::filesystem::remove(DUMP_FILE);
    }
    ::signal(SIGSEGV, &signalHandler);
    ::signal(SIGABRT, &signalHandler);
}
