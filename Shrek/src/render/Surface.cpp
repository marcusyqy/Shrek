#include "pch.h"
#include "Surface.h"

#include "platform/Log.h"
#include "platform/WindowsWindow.h"

#include "helper/Debug.h"
#include <GLFW/glfw3.h>

namespace shrek::render {

namespace {

} // namespace

Surface::Surface(VkInstance instance, GLFWwindow* window) SRK_NOEXCEPT
    :m_Instance(instance), m_Surface(VK_NULL_HANDLE), m_SwapChain(VK_NULL_HANDLE)
{
    VkResult result = glfwCreateWindowSurface(instance, window, nullptr, &m_Surface);
    if(result != VK_SUCCESS)
    {
        SRK_CORE_CRITICAL("Surface was unable to be created with err : {}!", result);
        // just to make sure. maybe bug potential
        m_Surface = VK_NULL_HANDLE;
    }
}

Surface::~Surface() SRK_NOEXCEPT
{
    vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
}


} // namespace shrek::render
