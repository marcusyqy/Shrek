#pragma once

#pragma warning(push, 0)
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#pragma warning(pop)

#include <memory>

namespace shrek {

class Log
{
public:
    static void Init() noexcept;
    static void Exit() noexcept;

    inline static const std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
    inline static const std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }

private:
    static std::shared_ptr<spdlog::logger> s_ClientLogger;
    static std::shared_ptr<spdlog::logger> s_CoreLogger;
};

} // namespace shrek


// Core log macros
#define SRK_CORE_TRACE(...)    ::shrek::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define SRK_CORE_INFO(...)     ::shrek::Log::GetCoreLogger()->info(__VA_ARGS__)
#define SRK_CORE_WARN(...)     ::shrek::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define SRK_CORE_ERROR(...)    ::shrek::Log::GetCoreLogger()->error(__VA_ARGS__)
#define SRK_CORE_CRITICAL(...) ::shrek::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define SRK_TRACE(...)    ::shrek::Log::GetClientLogger()->trace(__VA_ARGS__)
#define SRK_INFO(...)     ::shrek::Log::GetClientLogger()->info(__VA_ARGS__)
#define SRK_WARN(...)     ::shrek::Log::GetClientLogger()->warn(__VA_ARGS__)
#define SRK_ERROR(...)    ::shrek::Log::GetClientLogger()->error(__VA_ARGS__)
#define SRK_CRITICAL(...) ::shrek::Log::GetClientLogger()->critical(__VA_ARGS__)
