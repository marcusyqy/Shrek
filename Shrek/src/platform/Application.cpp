#include "pch.h"
#include "Application.h"

#include "Log.h"

namespace shrek {

Application::Application()
{
    Log::Init();
}

// for linux based applications(?)
Application::Application([[maybe_unused]] int argc, [[maybe_unused]] char** argv) :
    Application()
{
}

Application::~Application()
{
    Log::Exit();
}

void Application::Run()
{
   SRK_CORE_INFO("Hello World! I am running from {}", "Shrek");
}

} // namespace shrek
