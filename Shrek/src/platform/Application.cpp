#include "pch.h"

#include "Log.h"
#include "Application.h"

namespace shrek {

namespace {

using Singleton = base::Singleton<Application>;

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
            params.TitleBar   = true; // TODO: set this to false when Renderer is up.
        }
        return params;
    }()};
} // namespace


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
    Singleton("Application"),
    m_WindowManager(),
    m_Running(true),
    m_RenderEngine()
{
}

// for linux based applications(?)
Application::Application([[maybe_unused]] ApplicationCmdLineArgs params) SRK_NOEXCEPT :
    Application()
{
}

// should load here
// display loading screen here
void Application::Load() SRK_NOEXCEPT
{
    std::string_view loadingScreenName{"Shrek Loading Screen"};
    m_WindowManager.AddWindow(loadingScreenName, new WindowsWindow(m_RenderEngine, loadingScreenParams));
    bool loading = true;

    while (loading)
    {
        m_WindowManager.Update();

        // have to change to loading params
        loading = !m_WindowManager.Empty();
    }

    // it's safe to delete nullptr
    delete m_WindowManager.ReleaseWindow(loadingScreenName);


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
    m_WindowManager.AddWindow("Shrek Engine", new WindowsWindow(m_RenderEngine, params));
}

Application::~Application() SRK_NOEXCEPT = default;

bool Application::ShouldClose() SRK_NOEXCEPT
{
    return !m_Running;
}

void Application::Cleanup() SRK_NOEXCEPT
{
    SRK_CORE_INFO("Exitting from {} engine now...", "Shrek");
}

void Application::Tick() SRK_NOEXCEPT
{
    m_WindowManager.Update();
    m_Running = !m_WindowManager.Empty();
}

} // namespace shrek
