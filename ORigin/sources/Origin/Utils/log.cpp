#include "pch.h"
#include "log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace Origin
{
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		// Origin
		s_CoreLogger = spdlog::stdout_color_mt("Origin");
		s_CoreLogger->set_level(spdlog::level::trace);

		// Sandbox
		s_ClientLogger = spdlog::stdout_color_mt("Sandbox");
		s_ClientLogger->set_level(spdlog::level::trace);
	}
}