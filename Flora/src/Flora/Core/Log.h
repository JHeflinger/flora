#pragma once
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/ringbuffer_sink.h"
#include "spdlog/fmt/ostr.h"

namespace Flora {
	class Log {
	public:
		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
		inline static std::shared_ptr<spdlog::sinks::ringbuffer_sink_mt>& GetCoreRingBuffer() { return s_CoreRingbuffer; }
		inline static std::shared_ptr<spdlog::sinks::ringbuffer_sink_mt>& GetClientRingBuffer() { return s_ClientRingbuffer; }
	private:
		static std::shared_ptr<spdlog::sinks::ringbuffer_sink_mt> s_CoreRingbuffer;
		static std::shared_ptr<spdlog::sinks::ringbuffer_sink_mt> s_ClientRingbuffer;
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

//Core log macros
#define FL_CORE_ERROR(...)   ::Flora::Log::GetCoreLogger()->error(__VA_ARGS__)
#define FL_CORE_WARN(...)    ::Flora::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define FL_CORE_INFO(...)    ::Flora::Log::GetCoreLogger()->info(__VA_ARGS__)
#define FL_CORE_TRACE(...)   ::Flora::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define FL_CORE_FATAL(...)   ::Flora::Log::GetCoreLogger()->fatal(__VA_ARGS__)

//Client log macros
#define FL_ERROR(...)        ::Flora::Log::GetClientLogger()->error(__VA_ARGS__)
#define FL_WARN(...)         ::Flora::Log::GetClientLogger()->warn(__VA_ARGS__)
#define FL_INFO(...)         ::Flora::Log::GetClientLogger()->info(__VA_ARGS__)
#define FL_TRACE(...)        ::Flora::Log::GetClientLogger()->trace(__VA_ARGS__)
#define FL_FATAL(...)        ::Flora::Log::GetClientLogger()->fatal(__VA_ARGS__)