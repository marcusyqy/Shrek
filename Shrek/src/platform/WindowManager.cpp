#include "pch.h"

#include "Log.h"
#include "WindowManager.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL

#include <GLFW/glfw3native.h>


namespace shrek {

bool WindowManager::s_IsSingleton = false;

WindowManager::WindowManager() SRK_NOEXCEPT
{
    if (s_IsSingleton)
    {
        SRK_CORE_ERROR("WindowContext has already been created once!!");
        std::exit(-1);
    }

    s_IsSingleton = true;

    if (!glfwInit())
    {
        SRK_CORE_ERROR("Unable to initialize WindowContext!");
        std::exit(-1);
    }
}

WindowManager::~WindowManager() SRK_NOEXCEPT
{
    for (auto [name, window] : m_Windows)
    {
        delete window;
    }

    glfwTerminate();
}

void WindowManager::PollEvents() SRK_NOEXCEPT
{
    glfwPollEvents();
}

// note that since this takes in a string_view, this will guarantee that the name must still be preserved
void WindowManager::AddWindow(std::string_view name, WindowsWindow* window) SRK_NOEXCEPT
{
    m_Windows.emplace(name, window);
}

WindowsWindow* WindowManager::GetWindow(std::string_view name) const SRK_NOEXCEPT
{
    // returns nullptr if there isn't such a window
    auto iter = m_Windows.find(name);
    return iter != m_Windows.end() ? iter->second : nullptr;
}

WindowsWindow* WindowManager::ReleaseWindow(std::string_view name) SRK_NOEXCEPT
{
    WindowsWindow* window = nullptr;

    auto iter = m_Windows.find(name);
    if (iter != m_Windows.end())
    {
        window = iter->second;
        m_Windows.erase(iter);
    }

    return window;
}


} // namespace shrek
