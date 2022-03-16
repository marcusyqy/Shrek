#pragma once

#include "defs.h"
#include <GLFW/glfw3.h>
#include <windows.h>
#include <cstdint>

namespace shrek {


struct WindowParam
{
    bool Resizable {true};
};

class WindowsWindow
{
public:
    WindowsWindow(const WindowParam& param = {}) SRK_NOEXCEPT;
    ~WindowsWindow() SRK_NOEXCEPT;

    // deleting both copy and move until we can find a way to make glfw initialize be called once
    WindowsWindow(const WindowsWindow& other) = delete;
    WindowsWindow& operator=(const WindowsWindow& other) = delete;

    WindowsWindow(WindowsWindow&& other) SRK_NOEXCEPT;
    WindowsWindow& operator=(WindowsWindow&& other) SRK_NOEXCEPT;

    void               Update() SRK_NOEXCEPT;
    bool               ShouldClose() const SRK_NOEXCEPT;
    [[nodiscard]] HWND GetRenderContext() const SRK_NOEXCEPT;
    void               Resize(size_t width, size_t height) SRK_NOEXCEPT;

private:
    GLFWwindow* m_Window;
};
} // namespace shrek
