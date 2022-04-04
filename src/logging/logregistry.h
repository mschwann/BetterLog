#pragma once
#include <map>
#include <memory>
#include <experimental/source_location>
#include <thread>
#include "spdlog/spdlog.h"

namespace Logging
{
    class LogRegistry
    {
        public:
            static LogRegistry& inst()
            {
                static LogRegistry instance;
                return instance;
            }
            void attach(std::shared_ptr<spdlog::logger> logger, const std::experimental::source_location loc = std::experimental::source_location::current())
            {
                //Map::insert doesnt insert, if there is already one with this key - so no logic to prevent that
                reg_func.insert(std::make_pair(std::string(loc.function_name()), logger));
                reg_fname.insert(std::make_pair(std::string(loc.file_name()), logger));
                reg_tid.insert(std::make_pair(std::this_thread::get_id(), logger));
            }
            std::shared_ptr<spdlog::logger> get(const std::experimental::source_location loc = std::experimental::source_location::current())
            {
                //std::cout << loc.function_name() << " " << loc.file_name() << " " << std::this_thread::get_id() << std::endl;
                
                auto func_iter = reg_func.find(std::string(loc.function_name()));
                if(func_iter != reg_func.end()) return func_iter->second;

                auto fname_iter = reg_fname.find(std::string(loc.file_name()));
                if(fname_iter != reg_fname.end()) return fname_iter->second;

                auto tid_iter = reg_tid.find(std::this_thread::get_id());
                if(tid_iter != reg_tid.end()) return tid_iter->second;

                return std::make_shared<spdlog::logger>("dummy");
            }
        private:
            std::map<std::string , std::shared_ptr<spdlog::logger> > reg_func; //funcname log registry
            std::map<std::string , std::shared_ptr<spdlog::logger> > reg_fname; //source-fname log registry
            std::map<std::thread::id , std::shared_ptr<spdlog::logger> > reg_tid; // thread-id log registry
    };
}