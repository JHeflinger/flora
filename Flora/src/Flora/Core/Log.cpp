#include "flpch.h"
#include "Log.h"

namespace Flora {
	std::shared_ptr<spdlog::sinks::ringbuffer_sink_mt> Log::s_CoreRingbuffer;
	std::shared_ptr<spdlog::sinks::ringbuffer_sink_mt> Log::s_ClientRingbuffer;
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init() {
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_CoreLogger = spdlog::stdout_color_mt("FLORA");
		s_CoreLogger->set_level(spdlog::level::trace);
		s_ClientLogger = spdlog::stdout_color_mt("FAUNA");
		s_ClientLogger->set_level(spdlog::level::trace);


		s_CoreRingbuffer = std::make_shared<spdlog::sinks::ringbuffer_sink_mt>(1000);
		s_ClientRingbuffer = std::make_shared<spdlog::sinks::ringbuffer_sink_mt>(1000);
		s_CoreLogger->sinks().push_back(s_CoreRingbuffer);
		s_ClientLogger->sinks().push_back(s_ClientRingbuffer);
	}
}