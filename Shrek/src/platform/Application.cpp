#include "pch.h"

#include "Log.h"
#include "Application.h"

namespace shrek {

struct Application::ApplicationEssentials
{
    ApplicationEssentials() SRK_NOEXCEPT
    {
        Log::Init();
    }

    ~ApplicationEssentials() SRK_NOEXCEPT
    {
        Log::Exit();
    }
};

Application::Application() SRK_NOEXCEPT :
    m_Context(std::make_unique<ApplicationEssentials>()),
    m_Window()
{
}

// for linux based applications(?)
Application::Application([[maybe_unused]] int argc, [[maybe_unused]] char** argv) SRK_NOEXCEPT :
    Application()
{
}

Application::~Application() SRK_NOEXCEPT = default;

void Application::Run() SRK_NOEXCEPT
{
    SRK_CORE_INFO("Hello World! I am running from {}", "Shrek");

    while (!m_Window.ShouldClose())
    {
        // seems like an opengl cmd and will replace this with a vulkan equivalent soon
        m_Window.Update();
    }

    SRK_CORE_INFO("Exitting from {} engine now...", "Shrek");
}

} // namespace shrek
