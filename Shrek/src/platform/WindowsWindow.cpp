#include "defs.h"
#include "pch.h"

#include "Log.h"
#include "WindowsWindow.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL

#include <GLFW/glfw3native.h>


namespace shrek {

namespace {

GLFWwindow* CreateGLFWwindow() SRK_NOEXCEPT
{
    GLFWwindow* window;

    if (!glfwInit())
    {
        SRK_CORE_ERROR("Unable to initialize GLFW!");
        std::exit(-1);
    }

    window = glfwCreateWindow(640, 480, "Shrek Engine", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        SRK_CORE_ERROR("Unable to create window from GLFW!");
        std::exit(1);
    }

    return window;
}

} // namespace

WindowsWindow::WindowsWindow() SRK_NOEXCEPT : m_Window(CreateGLFWwindow())
{
}

WindowsWindow::~WindowsWindow() SRK_NOEXCEPT
{
    if (m_Window)
    {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }
}

// WindowsWindow::WindowsWindow(WindowsWindow&& other) SRK_NOEXCEPT : WindowsWindow()
// {
//     *this = std::move(other);
// }
//
// WindowsWindow& WindowsWindow::operator=(WindowsWindow&& other) SRK_NOEXCEPT
// {
//     std::swap(m_Window, other.m_Window);
//     return *this;
// }

HWND WindowsWindow::GetRenderContext() const SRK_NOEXCEPT
{
    return glfwGetWin32Window(m_Window);
}

void WindowsWindow::Update() SRK_NOEXCEPT
{
    glfwSwapBuffers(m_Window);
    glfwPollEvents();
}

bool WindowsWindow::ShouldClose() const SRK_NOEXCEPT
{
    return glfwWindowShouldClose(m_Window);
}

void WindowsWindow::Initialize() SRK_NOEXCEPT
{
    m_Window = CreateGLFWwindow();
}


} // namespace shrek
