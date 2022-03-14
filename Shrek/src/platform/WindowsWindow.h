#pragma once

#include "defs.h"
#include <GLFW/glfw3.h>
#include <windows.h>



namespace shrek {

class WindowsWindow
{
public:
    WindowsWindow() SRK_NOEXCEPT;
    ~WindowsWindow() SRK_NOEXCEPT;

    // deleting both copy and move until we can find a way to make glfw initialize be called once
    WindowsWindow(const WindowsWindow& other) = delete;
    WindowsWindow& operator=(const WindowsWindow& other) = delete;

    WindowsWindow(WindowsWindow&& other) SRK_NOEXCEPT = delete;
    WindowsWindow& operator=(WindowsWindow&& other) SRK_NOEXCEPT = delete;


    // deprecated for now
    [[deprecated]] void Initialize() SRK_NOEXCEPT;

    void               Update() SRK_NOEXCEPT;
    bool               ShouldClose() const SRK_NOEXCEPT;
    [[nodiscard]] HWND GetRenderContext() const SRK_NOEXCEPT;

private:
    GLFWwindow* m_Window;
};
} // namespace shrek
