#pragma once
#include <map>
#include <memory>
#include <string>
#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

namespace Logging
{
    class Exception : public std::runtime_error {
        public:
        template<typename T> Exception(T t) : std::runtime_error(t) {}
    };

    class LoggerFactory
    {
        public:
            LoggerFactory()
            {
                spdlog::init_thread_pool(8192, 1);
                sinks = {
                    std::make_pair("stdout", std::make_shared<spdlog::sinks::stdout_color_sink_mt >()),
                    std::make_pair("file", std::make_shared<spdlog::sinks::basic_file_sink_mt>("all.log"))
                };
            }
            void register_logger(const std::string& loggername, std::vector<std::string> sink_names, spdlog::level::level_enum logLevel)
            {
                std::vector<spdlog::sink_ptr> sinks_used;
                std::transform(sink_names.begin(), sink_names.end(), std::back_inserter(sinks_used), [&](const std::string& name){
                    auto it = sinks.find(name);
                    if(it != sinks.end())
                        return it->second;
                    throw Exception("LoggerFactory - sink not found(" + name+")");
                });
                auto logger = std::make_shared<spdlog::async_logger>(loggername, sinks_used.begin(), sinks_used.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
                logger->enable_backtrace(32);
                logger->set_level(logLevel);
                spdlog::register_logger(logger);
            }
            void register_file_sink(const std::string& sinkname, const std::string& fname)
            {
                sinks.insert(std::make_pair(sinkname, std::make_shared<spdlog::sinks::basic_file_sink_mt >(fname)));
            }
            std::map<std::string, std::shared_ptr<spdlog::sinks::sink> >& getSinks() {return sinks;}
        private:
            std::map<std::string, std::shared_ptr<spdlog::sinks::sink> > sinks;
    };
}