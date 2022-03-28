#pragma once
#include "defs.h"
#include "vulkan.h"

#include <GLFW/glfw3.h>
#include "helper/QueueFamilyIndices.h"

#include <vector>

namespace shrek::render {

struct SwapchainSupportDetails
{
    VkSurfaceCapabilitiesKHR        Capabilities;
    std::vector<VkSurfaceFormatKHR> Formats;
    std::vector<VkPresentModeKHR>   PresentModes;
};

class Surface
{
public:
    Surface(VkInstance instance, VkPhysicalDevice gpu, VkDevice lGpu, GLFWwindow* window, const helper::QueueFamilyIndices& indices) SRK_NOEXCEPT;
    Surface();

    ~Surface() SRK_NOEXCEPT;

         operator bool() const SRK_NOEXCEPT;
    bool IsValid() const SRK_NOEXCEPT;

    void Invalidate() SRK_NOEXCEPT;
    void Exit() SRK_NOEXCEPT;

    GLFWwindow* GetWindow() const SRK_NOEXCEPT;

private:
    void RecreateSwapchain();

    VkInstance m_Instance;
    VkDevice   m_Gpu;

    VkSurfaceKHR   m_Surface;
    VkSwapchainKHR m_Swapchain;

    SwapchainSupportDetails m_SwapchainSupportDetails;
    GLFWwindow*             m_Window;

    std::vector<VkImage> m_Images;
};

} // namespace shrek::render
