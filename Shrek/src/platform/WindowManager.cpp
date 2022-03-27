#include "pch.h"

#include "Log.h"
#include "WindowManager.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL

#include <GLFW/glfw3native.h>


namespace shrek {

using Singleton = base::Singleton<WindowManager>;

WindowManager::WindowManager() SRK_NOEXCEPT : Singleton("WindowManager")
{
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

void WindowManager::Update() SRK_NOEXCEPT
{
    for (auto [name, window] : m_Windows)
    {
        window->Update();
    }

    PollEvents();
    ValidateAndPurge();
}

void WindowManager::ValidateAndPurge() SRK_NOEXCEPT
{
    auto begin = m_Windows.begin();
    auto end   = m_Windows.end();
    while (begin != end)
    {
        if (begin->second->ShouldClose())
        {
            // TODO: need to optimize this part as delete will cause too loop time.
            SRK_CORE_TRACE("Deleting window now {}", begin->first);

            // deleting because it's still the manager's ownership
            delete begin->second;
            begin = m_Windows.erase(begin);
            end   = m_Windows.end();
        }
        else
        {
            ++begin;
        }
    }
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
        // not deleting because it's for the user to delete(releasing ownership to the user)
        m_Windows.erase(iter);
    }

    return window;
}

bool WindowManager::Empty() const SRK_NOEXCEPT
{
    return m_Windows.empty();
}

} // namespace shrek
