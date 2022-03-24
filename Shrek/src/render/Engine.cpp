#include "pch.h"
#include "Engine.h"
#include "vulkan_core.h"
#include "helper/Debug.h"
#include "platform/Log.h"

#include <GLFW/glfw3native.h>
#include <GLFW/glfw3.h>

namespace shrek::render {

using helper::QueueFamilyIndices;

namespace {

struct QueueFamilyIndicesHelper
{
    std::optional<uint32_t> Graphics{std::nullopt};
    std::optional<uint32_t> Compute{std::nullopt};

    operator QueueFamilyIndices() const SRK_NOEXCEPT
    {
        QueueFamilyIndices indices{};

        // TODO: change this to assert
        if (!Graphics.has_value())
        {
            SRK_CORE_CRITICAL("Could not find graphics queue but continued moving to this point of the program!");
            std::exit(1);
        }

        indices.Graphics = Graphics.value();
        indices.Compute  = Compute;

        return indices;
    }
};

constexpr bool        shouldPrintExtensions  = false;
constexpr bool        enableValidationLayers = true;
constexpr bool        shouldPrintDebugLogs   = true;
constexpr const char* debugUtilsExtName      = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;

constexpr std::array<const char*, 1> validationLayers = {
    "VK_LAYER_KHRONOS_validation"};

const std::array<const char*, 1> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME};

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT           messageSeverity,
    [[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT*      pCallbackData,
    [[maybe_unused]] void*                           pUserData) SRK_NOEXCEPT
{
    static constexpr const char* message = "[[VK_RENDERER]]: {}";

    switch (messageSeverity)
    {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            SRK_CORE_TRACE(message, pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            SRK_CORE_INFO(message, pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            SRK_CORE_WARN(message, pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            SRK_CORE_ERROR(message, pCallbackData->pMessage);
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

bool validateSupportOnLayers(const char** glfwExtensions, uint32_t glfwExtensionCount) SRK_NOEXCEPT
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

        // not found means 1 or more glfw layer not supported
        if (!found)
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
            if (strcmp(layerName, layerProp.layerName) == 0) // check for at least one support
                return true;
        }
    }

    return false;
}


std::vector<const char*> getRequiredExtensions() SRK_NOEXCEPT
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
        if (!validateSupportOnLayers(extensions.data(), static_cast<uint32_t>(extensions.size()))) //  TODO: change this to support both the top and bottom
        {
            SRK_CORE_ERROR("validation layer requested but debug extension not present!");
            extensions.pop_back();
        }
    }

    return extensions;
}

// to reuse for debugging of instance
VkDebugUtilsMessengerCreateInfoEXT populateDebugUtilsMessengerInfo() SRK_NOEXCEPT
{
    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    createInfo.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
    createInfo.pUserData       = nullptr; // Optional

    return createInfo;
}

VkDebugUtilsMessengerEXT setUpDebugMessenger(VkInstance instance) SRK_NOEXCEPT
{
    VkDebugUtilsMessengerCreateInfoEXT createInfo{populateDebugUtilsMessengerInfo()};

    VkDebugUtilsMessengerEXT debugMessenger{VK_NULL_HANDLE};
    VkResult                 result = CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger);

    if (result != VK_SUCCESS)
        SRK_CORE_WARN("vkCreateDebugUtilsMessenger failed with {}", result);

    return debugMessenger;
}


// TODO: can change to check charging power and all
int32_t scorePhysicalDevice(VkPhysicalDevice device) SRK_NOEXCEPT
{
    uint32_t score{};

    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(device, &properties);

    VkPhysicalDeviceFeatures features;
    vkGetPhysicalDeviceFeatures(device, &features);

    if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        score += 1000;

    // textures affect how strong the device is.
    score += properties.limits.maxImageDimension2D;

    if (!features.geometryShader) // should be valued as high as a discrete gpu.
        score -= 1000;

    if (shouldPrintDebugLogs)
        SRK_CORE_TRACE("Debugging score: {} and device name: {}", score, properties.deviceName);

    return score;
}

QueueFamilyIndicesHelper findQueueFamilies(VkPhysicalDevice device) SRK_NOEXCEPT
{
    uint32_t queueFamilyCount{};
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilyProps{queueFamilyCount};
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilyProps.data());

    QueueFamilyIndicesHelper indices{};
    uint32_t                 idx{};
    for (const auto& queueFamilyProp : queueFamilyProps)
    {
        if ((queueFamilyProp.queueFlags & VK_QUEUE_GRAPHICS_BIT) == VK_QUEUE_GRAPHICS_BIT)
        {
            indices.Graphics = idx;
        }
        ++idx;
    }
    return indices;
}

bool isDeviceSuitable(VkPhysicalDevice device) SRK_NOEXCEPT
{
    // check for extension support
    uint32_t extensionCount{};
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    for (const auto& deviceExtension : deviceExtensions)
    {
        bool found = false;
        for (const auto& availableExtension : availableExtensions)
        {
            if (strcmp(deviceExtension, availableExtension.extensionName) == 0)
            {
                found = true;
                break;
            }
        }

        if (!found)
        {
            SRK_CORE_WARN("Device extension of {} isn't found!", deviceExtension);
            return false;
        }
    }

    QueueFamilyIndicesHelper indices = findQueueFamilies(device);
    return indices.Graphics.has_value();
}


VkPhysicalDevice pickPhysicalDevice(VkInstance instance) SRK_NOEXCEPT
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    size_t  idx{}, highestScoreIdx{};
    int32_t highestScore = std::numeric_limits<int32_t>::lowest();

    if (shouldPrintDebugLogs)
        SRK_CORE_TRACE("Num of devices : {}", deviceCount, devices.size());

    bool foundSuitableDevice = false;
    for (const auto device : devices)
    {
        if (isDeviceSuitable(device))
        {
            int32_t score = scorePhysicalDevice(device);
            if (score > highestScore)
            {
                highestScoreIdx     = idx;
                highestScore        = score;
                foundSuitableDevice = true;
            }
        }

        ++idx;
    }

    if (foundSuitableDevice && highestScoreIdx < devices.size())
    {
        if (shouldPrintDebugLogs)
        {
            VkPhysicalDeviceProperties properties;
            vkGetPhysicalDeviceProperties(devices[highestScoreIdx], &properties);
            SRK_CORE_TRACE("Vulkan chose the gpu : {}", properties.deviceName);
        }
        return devices[highestScoreIdx];
    }

    // there should be no devices available(?)
    SRK_CORE_CRITICAL("Vulkan unable to find the right physical device");

    // this is if all devices are not suitable
    // for now just exit. maybe we can change the parameters of how to find a right physical device
    std::exit(-1);

    // this does not need to be returned
    return VK_NULL_HANDLE;
}

VkResult createDevice(VkPhysicalDevice physicalDevice, QueueFamilyIndices indices, VkDevice& device) SRK_NOEXCEPT
{
    VkDeviceQueueCreateInfo queueCreateInfo{};
    const float             queuePriorities{1.f};

    queueCreateInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = indices.Graphics;
    queueCreateInfo.queueCount       = 1;
    queueCreateInfo.pQueuePriorities = &queuePriorities;

    // HACK: only leaving it as it is for now because we haven't found what to do with it.
    VkPhysicalDeviceFeatures features{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType                = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos    = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pEnabledFeatures     = &features;
    createInfo.pNext                = nullptr;

    createInfo.enabledExtensionCount   = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    if (enableValidationLayers)
    {
        createInfo.enabledLayerCount   = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }
    else
    {
        createInfo.enabledLayerCount = 0;
    }

    return vkCreateDevice(physicalDevice, &createInfo, nullptr, &device);
}

VkResult createInstance(VkInstance& instance) SRK_NOEXCEPT
{
    VkApplicationInfo                  appInfo{createAppInfo()};
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{populateDebugUtilsMessengerInfo()};

    VkInstanceCreateInfo createInfo{};
    createInfo.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    const auto glfwExtensions          = getRequiredExtensions();
    createInfo.enabledExtensionCount   = static_cast<uint32_t>(glfwExtensions.size());
    createInfo.ppEnabledExtensionNames = glfwExtensions.data();
    createInfo.pNext                   = enableValidationLayers ? &debugCreateInfo : nullptr;

    if (enableValidationLayers)
    {
        if (checkValidationLayerSupport())
        {
            createInfo.enabledLayerCount   = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        }
        else
        {
            createInfo.enabledLayerCount   = 0;
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

    // if validate warning is inside the debug messenger
    if (enableValidationLayers)
        m_DebugHandler = setUpDebugMessenger(m_Instance);

    m_Gpu = pickPhysicalDevice(m_Instance);
    // only when physical device is found can we look for the queue families
    m_QueueFamily = findQueueFamilies(m_Gpu);

    result = createDevice(m_Gpu, m_QueueFamily, m_LGpu);
    if (result != VK_SUCCESS)
    {
        SRK_CORE_CRITICAL("Device cannot be created with error: {}", result);
        std::exit(result);
    }

    vkGetDeviceQueue(m_LGpu, m_QueueFamily.Graphics, 0, &m_Queue);
}

Engine::~Engine() SRK_NOEXCEPT
{
    vkDestroyDevice(m_LGpu, nullptr);
    // destroy in reverse order
    if (enableValidationLayers && m_DebugHandler != VK_NULL_HANDLE)
        DestroyDebugUtilsMessengerEXT(m_Instance, m_DebugHandler, nullptr);

    vkDestroyInstance(m_Instance, nullptr);
}

} // namespace shrek::render
