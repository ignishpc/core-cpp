
#include "ILog.h"
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>

namespace logging  = boost::log;
namespace attrs    = boost::log::attributes;

void ignis::executor::core::ILog::initLog() {
    logging::core::get()->add_thread_attribute("File", attrs::mutable_constant<std::string>(""));
    logging::core::get()->add_thread_attribute("Line", attrs::mutable_constant<int>(0));

    logging::add_console_log(std::clog, boost::log::keywords::format = (boost::log::expressions::stream
                                     << boost::log::expressions::format_date_time<boost::posix_time::ptime>( "TimeStamp", "%B %e, %Y %I:%M:%S %p")
                                     << ": <" << boost::log::trivial::severity << "> "
                                     << '[' << boost::log::expressions::attr<std::string>("File")
                                     << ':' << boost::log::expressions::attr<int>("Line") << "] "
                                     << boost::log::expressions::smessage
                             )
    );
    logging::add_common_attributes();
}

std::string ignis::executor::core::ILog::filename(std::string path) {
    return path.substr(path.find_last_of("/\\") + 1);
}

