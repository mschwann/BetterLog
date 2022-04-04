#include <iostream>
#include <exception>
#include "logging/logfactory.h"
#include "logging/log.h"

void terminate_handler()
{
	//If we are here, something went very wrong - so defaulting to std::cerr instead of using a log-system.
	std::cerr << "Terminate handler - something went wrong..." << std::endl;
	try
	{
		std::rethrow_exception(std::current_exception());
	}
	catch(const std::exception& e)
	{
		std::cerr << typeid(e).name() << std::endl;
		std::cerr << " what():" << e.what() << std::endl;
	}
	catch(...)
	{
		std::cerr << typeid(std::current_exception()).name() << std::endl;
		std::cerr << "... something, not an exception, dunno what." << std::endl;
	}
	//Lets try to recover some more info...
	std::cerr << "errno: " << errno << ": " << std::strerror(errno) << std::endl;
	std::cerr << "Generating backtraces of all loggers:" << std::endl;
	//This is the last step, so if something goes wrong - so be it.
	spdlog::details::registry::instance().apply_all( [] (auto x) {
		std::cerr << x->name() << ":" << std::endl;
		x->dump_backtrace();
		std::cerr << "==================================================" << std::endl;
	});
	std::abort();
}

void testingOtherLogger()
{
	LOG_ATTACH("tmp");
	LOG_INFO("TestTestTest");
}

int main()
{
	//MSMS::TODO - overriding logs from ENV/argv
	std::set_terminate(terminate_handler);
	Logging::LoggerFactory lf;
	//try {
		lf.register_file_sink("temp", "temp.txt");
		lf.register_logger("tmp", {"stdout"}, spdlog::level::info);
		lf.register_logger("loggername", {"stdout", "temp"}, spdlog::level::info);
	//}
	/*catch (Logging::Exception& e)
	{
		std::cout << e.what() << std::endl << "Current sink list:" << std::endl;
		for(auto sink :lf.getSinks())
			std::cout << "\t" << sink.first << std::endl;
	}*/

	{
		LOG_ATTACH("loggername");
		LOG_DEBUG("Debug MSG...");
		LOG_INFO("Will {} work?", "this");
		testingOtherLogger();
		{
			LOG_SCOPE_FORCELEVEL(spdlog::level::debug);
			LOG_DEBUG("Scope-force-debug!");
		}
		LOG_DEBUG("Backtrace-only display");
		LOG_BACKTRACE();
	}
	return 0;
}