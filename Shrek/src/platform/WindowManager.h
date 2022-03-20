#pragma once
#include "pch.h"

#include "defs.h"
#include <GLFW/glfw3.h>
#include <windows.h>
#include <memory>
#include "WindowsWindow.h"
#include "base/Singleton.h"

namespace shrek {

class WindowManager : private base::Singleton<WindowManager>
{
public:
    WindowManager() SRK_NOEXCEPT;
    ~WindowManager() SRK_NOEXCEPT;

    // Honestly copy and move will be implicitly deleted from Singleton base class but should we explicitly do it?
    WindowManager(const WindowManager& other) = delete;
    WindowManager& operator=(const WindowManager other) = delete;

    WindowManager(WindowManager&& other) SRK_NOEXCEPT = delete;
    WindowManager& operator=(WindowManager&& other) SRK_NOEXCEPT = delete;


    // note that since this takes in a string_view, this will guarantee that the name must still be preserved
    // also, the window now belongs to the window manager and is the right of the window manager to free the memory
    void AddWindow(std::string_view name, WindowsWindow* window) SRK_NOEXCEPT;

    WindowsWindow* GetWindow(std::string_view name) const SRK_NOEXCEPT;
    WindowsWindow* ReleaseWindow(std::string_view name) SRK_NOEXCEPT;

    // just to check if there are any windows
    bool Empty() const SRK_NOEXCEPT;

    void Update() SRK_NOEXCEPT;

private:
    // need to think about whether each of this functions can be moved to public(?) is there a use for them being in public(?)
    void ValidateAndPurge() SRK_NOEXCEPT;
    void PollEvents() SRK_NOEXCEPT;

private:
    std::unordered_map<std::string_view, WindowsWindow*> m_Windows;
};
} // namespace shrek
