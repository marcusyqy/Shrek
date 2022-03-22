#pragma once
#include "defs.h"
#include "vulkan.h"

#include <GLFW/glfw3.h>

namespace shrek::render {

class Surface
{
public:
    Surface(VkInstance instance, GLFWwindow* window) SRK_NOEXCEPT;
    ~Surface() SRK_NOEXCEPT;

private:
    VkInstance m_Instance;
    VkSurfaceKHR m_Surface;
    VkSwapchainKHR m_SwapChain;
};

} // namespace shrek::render
