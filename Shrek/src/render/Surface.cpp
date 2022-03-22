#include "pch.h"
#include "Surface.h"

#include "platform/Log.h"
#include "platform/WindowsWindow.h"

#include "helper/Debug.h"
#include <GLFW/glfw3.h>

namespace shrek::render {

using helper::QueueFamilyIndices;

namespace {

} // namespace

Surface::Surface(VkInstance instance, VkPhysicalDevice gpu, GLFWwindow* window, const QueueFamilyIndices& indices) SRK_NOEXCEPT :
m_Instance(instance), m_Surface(VK_NULL_HANDLE), m_SwapChain(VK_NULL_HANDLE)
{
    VkResult result = glfwCreateWindowSurface(instance, window, nullptr, &m_Surface);
    if (result != VK_SUCCESS)
    {
        SRK_CORE_CRITICAL("Surface was unable to be created with err : {}!", result);
        // just to make sure. maybe bug potential
        m_Surface = VK_NULL_HANDLE;
    }

    // TODO: assert this
    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(gpu, indices.Graphics, m_Surface, &presentSupport);

    if (!presentSupport)
    {
        SRK_CORE_CRITICAL("Present support not available on this device on graphics queue family index");
        std::exit(-1);
    }
}

// serves as invalidated surface
Surface::Surface() :
    m_Instance(VK_NULL_HANDLE), m_Surface(VK_NULL_HANDLE), m_SwapChain(VK_NULL_HANDLE)
{
}

Surface::~Surface() SRK_NOEXCEPT
{
    if(IsValid())
        vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
}

Surface::operator bool() const SRK_NOEXCEPT
{
    return IsValid();
}

bool Surface::IsValid() const SRK_NOEXCEPT
{
    return m_Instance != VK_NULL_HANDLE;
}


} // namespace shrek::render
