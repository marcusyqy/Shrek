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

    (void)glfwVulkanSupported();

    // using Vulkan.
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    //glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

    /*
     * for now,
     * hopefully this parameter doesn't provide undef behaviors
    */
    glfwWindowHint(GLFW_MAXIMIZED, param.Maximize);
    glfwWindowHint(GLFW_RESIZABLE, param.Resizable);
    glfwWindowHint(GLFW_DECORATED, param.TitleBar);

    window = glfwCreateWindow(
        static_cast<int>(param.Width),
        static_cast<int>(param.Height),
        param.WindowName.data(), NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        SRK_CORE_ERROR("Unable to create window from GLFW!");
        return nullptr;
    }

    return window;
}

} // namespace

WindowsWindow::WindowsWindow(const render::Engine& engine, const WindowParam& param) SRK_NOEXCEPT :
    m_Window(CreateGLFWwindow(param)),
    m_Surface(engine.GetInstance(), engine.GetGpu(), m_Window, engine.GetQueueFamilyIndices())
{
    // so that user pointer won't throw from null exception
    if (m_Window != nullptr)
    {
        SetCallbacks();
    }
}

void WindowsWindow::Resize(size_t width, size_t height) SRK_NOEXCEPT
{
    (void)width;
    (void)height;
}

bool WindowsWindow::IsValid() const SRK_NOEXCEPT
{
    return m_Window != nullptr;
}

WindowsWindow::~WindowsWindow() SRK_NOEXCEPT
{
    if (m_Window)
    {
        glfwDestroyWindow(m_Window);
        m_Window = nullptr;
    }
}

WindowsWindow::WindowsWindow(WindowsWindow&& other) SRK_NOEXCEPT : m_Window(nullptr), m_Surface()
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

void WindowsWindow::SetCallbacks() SRK_NOEXCEPT
{
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwSetWindowUserPointer(m_Window, this);

    glfwSetFramebufferSizeCallback(
        m_Window, [](GLFWwindow* window, int width, int height) SRK_NOEXCEPT {
            WindowsWindow* parent = reinterpret_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));
            parent->Resize(width, height);
        });
}

} // namespace shrek
