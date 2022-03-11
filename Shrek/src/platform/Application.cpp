#include "pch.h"
#include "Application.h"

#include "Log.h"

namespace shrek {

Application::Application() SRK_NOEXCEPT
{
    Log::Init();
}

// for linux based applications(?)
Application::Application([[maybe_unused]] int argc, [[maybe_unused]] char** argv) SRK_NOEXCEPT :
    Application()
{
}


Application::~Application() SRK_NOEXCEPT
{
    Log::Exit();
}

void Application::Run() SRK_NOEXCEPT
{
    SRK_CORE_INFO("Hello World! I am running from {}", "Shrek");
}

} // namespace shrek
