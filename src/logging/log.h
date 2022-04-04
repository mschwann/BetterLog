#pragma once
#include "logregistry.h"

#define LOGGER_FROM_NAME(loggername) (spdlog::get(loggername)?:std::make_shared<spdlog::logger>(loggername))
#define LOG_ATTACH(logname) Logging::LogRegistry::inst().attach(LOGGER_FROM_NAME(logname))
#define GET_LOGGER() Logging::LogRegistry::inst().get()
#define LOG_INFO(...) GET_LOGGER()->info(__VA_ARGS__);
#define LOG_DEBUG(...) GET_LOGGER()->debug(__VA_ARGS__);
#define LOG_SCOPE_FORCELEVEL(level, ...) Logging::ScopedLogElevation _(GET_LOGGER(), level)
#define LOG_BACKTRACE() GET_LOGGER()->dump_backtrace()

namespace Logging
{
    class ScopedLogElevation
    {
        public:
            ScopedLogElevation(std::shared_ptr<spdlog::logger> logger, spdlog::level::level_enum newLevel) :
                logger(logger), level(logger->level()) {logger->set_level(newLevel);}
            ~ScopedLogElevation() { logger->set_level(level);}
        private:
            std::shared_ptr<spdlog::logger> logger;
            spdlog::level::level_enum level;
    };
};