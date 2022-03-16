#include "pch.h"

#include "Log.h"
#include "Application.h"

namespace shrek {


namespace {
constexpr static size_t      loadingScreenWidth{640};
constexpr static size_t      loadingScreenHeight{480};
constexpr static WindowParam loadingScreenParams{
    []() { // initialize parasms SAFELY in case we change the order of some variables
        WindowParam params;
        {
            params.Width      = loadingScreenWidth;
            params.Height     = loadingScreenHeight;
            params.Maximize   = false;
            params.Resizable  = false;
            params.WindowName = "Shrek Loading Screen";
            params.TitleBar   = true; // TODO(Marcus): set this to false when Renderer is up.
        }
        return params;
    }()};
} // namespace

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

ApplicationCmdLineArgs::ApplicationCmdLineArgs(int argc, char** argv) SRK_NOEXCEPT :
    m_Size(static_cast<size_t>(argc)),
    m_Arguments(argv)
{
}

const char* ApplicationCmdLineArgs::operator[](size_t idx) const SRK_NOEXCEPT
{
    if (idx >= m_Size)
        return nullptr;

    return m_Arguments[idx];
}

size_t ApplicationCmdLineArgs::Size() const SRK_NOEXCEPT
{
    return m_Size;
}

Application::Application() SRK_NOEXCEPT :
    m_Context(std::make_unique<ApplicationEssentials>()),
    m_WindowManager(),
    m_Running(true)
{
}

// for linux based applications(?)
Application::Application([[maybe_unused]] ApplicationCmdLineArgs params) SRK_NOEXCEPT :
    Application()
{
}

// should load here
void Application::Load() SRK_NOEXCEPT
{
    WindowsWindow* loadingScreen{new WindowsWindow(loadingScreenParams)};

    while (!loadingScreen->ShouldClose())
    {
        loadingScreen->Update();
        m_WindowManager.PollEvents();
    }

    delete loadingScreen;
}

Application::~Application() SRK_NOEXCEPT = default;

void Application::Run() SRK_NOEXCEPT
{
    SRK_CORE_TRACE("Loading engine now");
    Load();

    SRK_CORE_INFO("Hello World! I am running from {}", "Shrek");

    WindowParam params;
    {
        params.Width      = loadingScreenWidth;
        params.Height     = loadingScreenHeight;
        params.Maximize   = false;
        params.Resizable  = false;
        params.WindowName = "Shrek Engine";
        params.TitleBar   = true;
    }
    m_WindowManager.AddWindow("Shrek Loading Screen", new WindowsWindow(params));

    while (m_Running)
    {
        // seems like an opengl cmd and will replace this with a vulkan equivalent soon
        m_WindowManager.Update();
        m_Running = !m_WindowManager.Empty();
    }

    SRK_CORE_INFO("Exitting from {} engine now...", "Shrek");
}

} // namespace shrek
