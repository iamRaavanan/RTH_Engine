#pragma once
#include <memory>
#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace RTH
{
#pragma warning(push)
#pragma warning(disable:4251)
	class RTH_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

// Core log marcos
#define RTH_CORE_TRACE(...)	::RTH::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define RTH_CORE_INFO(...)	::RTH::Log::GetCoreLogger()->info(__VA_ARGS__)
#define RTH_CORE_WARN(...)	::RTH::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define RTH_CORE_ERROR(...)	::RTH::Log::GetCoreLogger()->error(__VA_ARGS__)

// Client log marcos
#define RTH_TRACE(...)	::RTH::Log::GetClientLogger()->trace(__VA_ARGS__)
#define RTH_INFO(...)	::RTH::Log::GetClientLogger()->info(__VA_ARGS__)
#define RTH_WARN(...)	::RTH::Log::GetClientLogger()->warn(__VA_ARGS__)
#define RTH_ERROR(...)	::RTH::Log::GetClientLogger()->error(__VA_ARGS__)
