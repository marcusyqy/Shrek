#include "pch.h"
#include "Engine.h"
#include "vulkan_core.h"
#include "helper/Debug.h"
#include "platform/Log.h"

#include <GLFW/glfw3native.h>
#include <GLFW/glfw3.h>

namespace shrek::render {

namespace {

constexpr bool        shouldPrintExtensions  = false;
constexpr bool        enableValidationLayers = true;
constexpr const char* debugUtilsExtName      = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;

constexpr std::array<const char*, 1> validationLayers = {
    "VK_LAYER_KHRONOS_validation"};

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT           messageSeverity,
    [[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT*      pCallbackData,
    [[maybe_unused]] void*                           pUserData)
{

    switch (messageSeverity)
    {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            SRK_CORE_TRACE("[[RENDERER]] vkDebug: {}", pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            SRK_CORE_INFO("[[RENDERER]] vkDebug: {}", pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            SRK_CORE_WARN("[[RENDERER]] vkDebug: {}", pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            SRK_CORE_ERROR("[[RENDERER]] vkDebug: {}", pCallbackData->pMessage);
            break;
        default:
            break;
    }

    return VK_FALSE;
}

VkApplicationInfo createAppInfo() SRK_NOEXCEPT
{
    VkApplicationInfo appInfo;
    {
        appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName   = "Shrek";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName        = "Shrek Engine";
        appInfo.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion         = VK_API_VERSION_1_0; // following Vulkan tutorial
        appInfo.pNext              = NULL;               // caught this with validation layers
    }
    return appInfo;
}

bool validateSupportOnLayers(const char** glfwExtensions, uint32_t glfwExtensionCount)
{
    uint32_t extensionCount{};
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    // a vector here doesn't slow down the process as much because we don't need 60hz loading
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    if (shouldPrintExtensions)
    {
        SRK_CORE_INFO("Number of available extensions: {}", extensionCount);
        {
            uint32_t idx{};
            for (const auto& extension : extensions)
            {
                SRK_CORE_INFO("Extension {}) {}", idx++, extension.extensionName);
            }
        }

        SRK_CORE_INFO("Number of glfw extensions: {}", glfwExtensionCount);
        {
            for (uint32_t idx{}; idx < glfwExtensionCount; ++idx)
            {
                SRK_CORE_INFO("Glfw Extension {}) {}", idx, glfwExtensions[idx]);
            }
        }
    }


    for (uint32_t idx{}; idx < glfwExtensionCount; ++idx)
    {
        bool found = false;
        for (const auto& extension : extensions)
        {
            if (strcmp(glfwExtensions[idx], extension.extensionName) == 0)
            {
                found = true;
                break;
            }
        }

        if (!found) // not found means 1 or more glfw layer not supported
            return false;
    }

    return true;
}

bool checkValidationLayerSupport() SRK_NOEXCEPT
{
    uint32_t layerCount{};
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> layerProps(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, layerProps.data());

    for (const auto& layerName : validationLayers)
    {
        for (const auto& layerProp : layerProps)
        {
            if (strcmp(layerName, layerProp.layerName) == 0)
            {
                return true;
            }
        }
    }

    return false;
}


std::vector<const char*> getRequiredExtensions()
{
    uint32_t     glfwExtensionCount{};
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    if (!validateSupportOnLayers(glfwExtensions, glfwExtensionCount))
    {
        SRK_CORE_CRITICAL("Unable to find the right glfwExtensions!\n Enable printing of extensions to debug the non-existent layers");
        std::exit(-1);
    }

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
    if (enableValidationLayers)
    {
        extensions.emplace_back(debugUtilsExtName);
        if (!validateSupportOnLayers(extensions.data(), static_cast<uint32_t>(extensions.size()))) // lazy TODO(Marcus): change this to support both the top and bottom
        {
            SRK_CORE_ERROR("validation layer requested but debug extension not present!");
            extensions.pop_back();
        }
    }

    return extensions;
}

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}


VkDebugUtilsMessengerEXT setUpDebugMessenger(VkInstance instance) SRK_NOEXCEPT
{
    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
    createInfo.pUserData = nullptr; // Optional

    VkDebugUtilsMessengerEXT debugMessenger{nullptr};
    VkResult result = CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger);
    if(result != VK_SUCCESS)
        SRK_CORE_WARN("vkCreateDebugUtilsMessenger failed with {}", result);

    return debugMessenger;
}

VkResult createInstance(VkInstance& instance) SRK_NOEXCEPT
{
    VkApplicationInfo    appInfo = createAppInfo();
    VkInstanceCreateInfo createInfo{};
    createInfo.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    const auto glfwExtensions = getRequiredExtensions();

    createInfo.enabledExtensionCount   = static_cast<uint32_t>(glfwExtensions.size());
    createInfo.ppEnabledExtensionNames = glfwExtensions.data();

    if (enableValidationLayers)
    {
        if (checkValidationLayerSupport())
        {
            createInfo.enabledLayerCount   = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        }
        else
        {
            createInfo.enabledLayerCount = 0;
            createInfo.ppEnabledLayerNames = nullptr;
            SRK_CORE_ERROR("Validation layer support requested but not supported!");
        }
    }

    return vkCreateInstance(&createInfo, nullptr, &instance);
}



} // namespace

using Singleton = base::Singleton<Engine>;

Engine::Engine() SRK_NOEXCEPT :
    Singleton("render::Engine"),
    m_Instance(),
    m_Gpu(),
    m_LGpu(),
    m_DebugHandler()
{
    int isVulkanSupported = glfwVulkanSupported();
    if (isVulkanSupported != GLFW_TRUE)
    {
        // stop here
        SRK_CORE_CRITICAL("Vulkan not supported! {}", isVulkanSupported);
        std::exit(isVulkanSupported);
    }

    VkResult result = createInstance(m_Instance);
    if (result != VK_SUCCESS)
    {
        // stop here
        SRK_CORE_CRITICAL("Vulkan Instance unable to be created! {}", result);
        std::exit(result);
    }

    // if validate
    if(enableValidationLayers)
    {
        m_DebugHandler = setUpDebugMessenger(m_Instance);
        if(m_DebugHandler)
        {
        }
        else
            SRK_CORE_WARN("Unable to setUpDebugMessenger after request for it!");
    }
}

Engine::~Engine() SRK_NOEXCEPT
{
    // destroy in reverse order
    if (enableValidationLayers && m_DebugHandler)
        DestroyDebugUtilsMessengerEXT(m_Instance, m_DebugHandler, nullptr);

    vkDestroyInstance(m_Instance, nullptr);
}

} // namespace shrek::render
