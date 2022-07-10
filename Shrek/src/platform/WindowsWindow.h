#pragma once

#include "defs.h"
#include <GLFW/glfw3.h>
#include <windows.h>
#include <cstdint>
#include <string_view>

#include "render/Surface.h"
#include "render/Engine.h"

namespace shrek {


struct WindowParam
{
    size_t           Width{1600};
    size_t           Height{900};
    bool             Resizable{true};
    bool             Maximize{false};
    bool             TitleBar{true};
    std::string_view WindowName{"Shrek Engine"};
};

// TODO(Marcus): Should we even have this class if the render/Surface is going to have ownership of the GLFWwindow ptr?
class WindowsWindow
{
public:
    WindowsWindow(const render::Engine& engine, const WindowParam& param = {}) SRK_NOEXCEPT;
    ~WindowsWindow() SRK_NOEXCEPT;

    // deleting both copy and move until we can find a way to make glfw initialize be called once
    WindowsWindow(const WindowsWindow& other) = delete;
    WindowsWindow& operator=(const WindowsWindow& other) = delete;

    WindowsWindow(WindowsWindow&& other) SRK_NOEXCEPT;
    WindowsWindow& operator=(WindowsWindow&& other) SRK_NOEXCEPT;

    void               Update() SRK_NOEXCEPT;
    bool               ShouldClose() const SRK_NOEXCEPT;
    // [[nodiscard]] HWND GetRenderContext() const SRK_NOEXCEPT;
    void               Resize(size_t width, size_t height) SRK_NOEXCEPT;
    bool               IsValid() const SRK_NOEXCEPT;
    void               SetCallbacks() SRK_NOEXCEPT;

    GLFWwindow* Raw() const SRK_NOEXCEPT { return m_Surface.GetWindow(); };

private:
    render::Surface m_Surface;
};
} // namespace shrek
