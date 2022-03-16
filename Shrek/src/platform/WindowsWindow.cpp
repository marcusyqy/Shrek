#include "pch.h"

#include "Log.h"
#include "WindowsWindow.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL

#include <GLFW/glfw3native.h>

namespace shrek {

namespace {

GLFWwindow* CreateGLFWwindow(const WindowParam& param) SRK_NOEXCEPT
{
    GLFWwindow* window;

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    //glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
    //glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

    // for now
    // hopefully this parameter doesn't provide undef behaviors
    glfwWindowHint(GLFW_RESIZABLE, param.Resizable);

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

WindowsWindow::WindowsWindow(const WindowParam& param) SRK_NOEXCEPT : m_Window(CreateGLFWwindow(param))
{

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwSetWindowUserPointer(m_Window, this);


    glfwSetFramebufferSizeCallback(
        m_Window, [](GLFWwindow* window, int width, int height) SRK_NOEXCEPT {
            WindowsWindow* parent = reinterpret_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));
            parent->Resize(width, height);
        });
}

void WindowsWindow::Resize(size_t width, size_t height) SRK_NOEXCEPT
{
    (void)width;
    (void)height;
}

WindowsWindow::~WindowsWindow() SRK_NOEXCEPT
{
    if (m_Window)
    {
        glfwDestroyWindow(m_Window);
        m_Window = nullptr;
    }
}

WindowsWindow::WindowsWindow(WindowsWindow&& other) SRK_NOEXCEPT : m_Window(nullptr)
{
    *this = std::move(other);
}

WindowsWindow& WindowsWindow::operator=(WindowsWindow&& other) SRK_NOEXCEPT
{
    std::swap(m_Window, other.m_Window);
    return *this;
}

HWND WindowsWindow::GetRenderContext() const SRK_NOEXCEPT
{
    return glfwGetWin32Window(m_Window);
}

void WindowsWindow::Update() SRK_NOEXCEPT
{
    glfwSwapBuffers(m_Window);
}

bool WindowsWindow::ShouldClose() const SRK_NOEXCEPT
{
    return glfwWindowShouldClose(m_Window);
}


} // namespace shrek
