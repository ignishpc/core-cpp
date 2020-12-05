
#include "ILog.h"
#include "spdlog/spdlog.h"


using namespace ignis::executor::core;

spdlog::level::level_enum parse[] = {spdlog::level::trace, spdlog::level::debug, spdlog::level::info,
                                     spdlog::level::warn,  spdlog::level::err,   spdlog::level::critical};


void ILog::initLog() { spdlog::default_logger()->set_pattern("%B %d, %Y %I:%M:%S %p <%l> [%s:%#] %v"); }

ILog::ILog(level l, const std::string &file, int line) : l(l), file(file), line(line) {}

ILog::~ILog() { spdlog::default_logger()->log(spdlog::source_loc(file.c_str(), line, nullptr), parse[l], this->str()); }

void ILog::enable(bool enable) {
    if (enable) {
        spdlog::set_level(spdlog::level::trace);
    } else {
        spdlog::set_level(spdlog::level::off);
    }
}
