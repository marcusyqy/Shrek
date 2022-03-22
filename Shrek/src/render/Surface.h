#pragma once
#include "defs.h"
#include "vulkan.h"

#include <GLFW/glfw3.h>
#include "helper/QueueFamilyIndices.h"

namespace shrek::render {

class Surface
{
public:
    Surface(VkInstance instance, VkPhysicalDevice gpu, GLFWwindow* window, const helper::QueueFamilyIndices& indices) SRK_NOEXCEPT;
    Surface();

    ~Surface() SRK_NOEXCEPT;

    operator bool() const SRK_NOEXCEPT;
    bool IsValid() const SRK_NOEXCEPT;

private:
    VkInstance m_Instance;
    VkSurfaceKHR m_Surface;
    VkSwapchainKHR m_SwapChain;
};

} // namespace shrek::render
