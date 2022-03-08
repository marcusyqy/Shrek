#pragma once

#pragma warning(push, 0)
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#pragma warning(pop)

#include <memory>

namespace shrek
{

class Log
{
public:
    static void Init() noexcept;
    static void Exit() noexcept;

    static const std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
    static const std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }

private:
    static std::shared_ptr<spdlog::logger> s_ClientLogger;
    static std::shared_ptr<spdlog::logger> s_CoreLogger;
};

} // namespace shrek


// Core log macros
#define SRK_CORE_TRACE(...)    ::Shrek::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define SRK_CORE_INFO(...)     ::Shrek::Log::GetCoreLogger()->info(__VA_ARGS__)
#define SRK_CORE_WARN(...)     ::Shrek::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define SRK_CORE_ERROR(...)    ::Shrek::Log::GetCoreLogger()->error(__VA_ARGS__)
#define SRK_CORE_CRITICAL(...) ::Shrek::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define SRK_TRACE(...)    ::Shrek::Log::GetClientLogger()->trace(__VA_ARGS__)
#define SRK_INFO(...)     ::Shrek::Log::GetClientLogger()->info(__VA_ARGS__)
#define SRK_WARN(...)     ::Shrek::Log::GetClientLogger()->warn(__VA_ARGS__)
#define SRK_ERROR(...)    ::Shrek::Log::GetClientLogger()->error(__VA_ARGS__)
#define SRK_CRITICAL(...) ::Shrek::Log::GetClientLogger()->critical(__VA_ARGS__)
