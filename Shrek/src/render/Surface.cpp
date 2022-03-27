#include "pch.h"
#include "Surface.h"

#include "platform/Log.h"
#include "platform/WindowsWindow.h"

#include "helper/Debug.h"
#include "vulkan_core.h"
#include "vulkan_win32.h"
#include <GLFW/glfw3.h>

namespace shrek::render {

using helper::QueueFamilyIndices;

namespace {

// Vulkan tutorial says to use this but for pc we probably don't need to use this to check for device support
SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    SwapChainSupportDetails details{};

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.Capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

    if (formatCount != 0)
    {
        details.Formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.Formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

    if (presentModeCount != 0)
    {
        details.PresentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.PresentModes.data());
    }

    return details;
}

VkSurfaceFormatKHR chooseRightSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& surfaceFormats) SRK_NOEXCEPT
{
    for (const auto& format : surfaceFormats)
    {
        if (format.format == VK_FORMAT_B8G8R8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            return format;
    }

    return surfaceFormats.front();
}

VkExtent2D chooseSwapExtent(GLFWwindow* window, const VkSurfaceCapabilitiesKHR& capabilities)
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        return capabilities.currentExtent;
    }
    else
    {
        int width{};
        int height{};
        glfwGetFramebufferSize(window, &width, &height);

        VkExtent2D actualExtent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
        actualExtent.width      = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height     = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}


VkPresentModeKHR chooseSwapChainPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) SRK_NOEXCEPT
{
    for (const auto& presentMode : availablePresentModes)
    {
        if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
            return presentMode;
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkResult createSwapChain(VkSwapchainKHR& swapChain, VkSurfaceKHR surface, SwapChainSupportDetails swapChainSupportDetails, VkDevice gpu, GLFWwindow* window) SRK_NOEXCEPT
{
    VkSurfaceFormatKHR format      = chooseRightSurfaceFormat(swapChainSupportDetails.Formats);
    VkPresentModeKHR   presentMode = chooseSwapChainPresentMode(swapChainSupportDetails.PresentModes);
    VkExtent2D         extent      = chooseSwapExtent(window, swapChainSupportDetails.Capabilities);
    uint32_t           imageCount  = swapChainSupportDetails.Capabilities.minImageCount + 1;

    if (swapChainSupportDetails.Capabilities.maxImageCount > 0 && imageCount > swapChainSupportDetails.Capabilities.maxImageCount)
        imageCount = swapChainSupportDetails.Capabilities.maxImageCount;

    VkSwapchainCreateInfoKHR createInfo{};
    // TODO: completely populate swap chain create info struct
    {
        createInfo.sType                 = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface               = surface;
        createInfo.minImageCount         = imageCount;
        createInfo.imageFormat           = format.format;
        createInfo.imageColorSpace       = format.colorSpace;
        createInfo.imageExtent           = extent;
        createInfo.imageArrayLayers      = 1; // specifically for 3d type of rendering will only be using 1
        createInfo.imageUsage            = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        createInfo.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices   = nullptr;
        createInfo.preTransform          = swapChainSupportDetails.Capabilities.currentTransform;
        createInfo.compositeAlpha        = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode           = presentMode;
        createInfo.clipped               = VK_TRUE;
        createInfo.oldSwapchain          = swapChain; // doesn't need to check for VK_NULL_HANDLE because it probably is at the first run...
    }

    return vkCreateSwapchainKHR(gpu, &createInfo, nullptr, &swapChain);
}

VkResult acquireSwapChainImages(VkDevice device, VkSwapchainKHR swapChain, std::vector<VkImage>& images)
{
    images.clear();
    uint32_t imageCount{};

    VkResult result = vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);

    if (result != VK_SUCCESS)
        return result;

    images.resize(imageCount);
    return vkGetSwapchainImagesKHR(device, swapChain, &imageCount, images.data());
}

} // namespace

Surface::Surface(VkInstance instance, VkPhysicalDevice gpu, VkDevice lGpu, GLFWwindow* window, const QueueFamilyIndices& indices) SRK_NOEXCEPT :
    m_Instance(instance),
    m_Gpu(lGpu),
    m_Surface(VK_NULL_HANDLE),
    m_SwapChain(VK_NULL_HANDLE),
    m_Window(window)
{
    VkResult result = glfwCreateWindowSurface(instance, window, nullptr, &m_Surface);
    if (result != VK_SUCCESS)
    {
        SRK_CORE_CRITICAL("Surface was unable to be created with err : {}!", result);
        // just to make sure. maybe bug potential
        m_Surface   = VK_NULL_HANDLE;
        m_SwapChain = VK_NULL_HANDLE;
    }
    else
    {
        m_SwapChainSupportDetails = querySwapChainSupport(gpu, m_Surface);

        // TODO: assert this
        VkBool32 presentSupport{};
        result = vkGetPhysicalDeviceSurfaceSupportKHR(gpu, indices.Graphics, m_Surface, &presentSupport);
        if (result != VK_SUCCESS)
        {
            SRK_CORE_CRITICAL("vkGetPhysicalDeviceSurfaceSupportKHR failed with {}!", result);
            std::exit(-1);
        }
        else if (!presentSupport)
        {
            SRK_CORE_CRITICAL("Present support not available on this device on graphics queue family index");
            std::exit(-1);
        }

        // used to recreate and create initially
        RecreateSwapChain();
    }
}

// decided to put this here because this will likely be using all the resources from the render::Surface
void Surface::RecreateSwapChain()
{
    VkResult result = createSwapChain(m_SwapChain, m_Surface, m_SwapChainSupportDetails, m_Gpu, m_Window);
    if (result != VK_SUCCESS)
    {
        SRK_CORE_CRITICAL("SwapChain was unable to be created with err : {}!", result);
        // just to make sure. maybe bug potential, so that this will `invalidate` the SwapChain.
        m_SwapChain = VK_NULL_HANDLE;
    }
    else
    {
        result = acquireSwapChainImages(m_Gpu, m_SwapChain, m_Images);
    }
}

Surface::Surface() :
    m_Instance(VK_NULL_HANDLE), m_Gpu(VK_NULL_HANDLE), m_Surface(VK_NULL_HANDLE), m_SwapChain(VK_NULL_HANDLE), m_Window(nullptr)
{
}

Surface::~Surface() SRK_NOEXCEPT
{
    Exit();
}

void Surface::Exit() SRK_NOEXCEPT
{
    Invalidate(); // invalidate the swapchain 1st

    if (IsValid())
    {
        vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
        m_Instance = VK_NULL_HANDLE;
        m_Surface  = VK_NULL_HANDLE;
    }

    // Deletes Window as well.
    if (m_Window)
    {
        glfwDestroyWindow(m_Window);
        m_Window = nullptr;
    }
}

void Surface::Invalidate() SRK_NOEXCEPT
{
    if (m_SwapChain)
    {
        // to invalidate swapchain and start creating swapchain again
        vkDestroySwapchainKHR(m_Gpu, m_SwapChain, nullptr);
        // set to null (fn will probably not set it to null since it is not taking in a reference)
        m_SwapChain = VK_NULL_HANDLE;
    }
}

Surface::operator bool() const SRK_NOEXCEPT
{
    return IsValid();
}

bool Surface::IsValid() const SRK_NOEXCEPT
{
    return m_Window != nullptr && m_Instance != VK_NULL_HANDLE && m_SwapChain != VK_NULL_HANDLE && m_Surface != VK_NULL_HANDLE && m_Gpu != VK_NULL_HANDLE;
}


GLFWwindow* Surface::GetWindow() const SRK_NOEXCEPT
{
    return m_Window;
}

} // namespace shrek::render
