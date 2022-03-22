#include "pch.h"
#include "Debug.h"
#include "platform/Log.h"


namespace shrek::render::helper {


std::string_view ToString(VkResult result) SRK_NOEXCEPT
{
#define TO_STRING(X) \
    case X:          \
        return #X
    switch (result)
    {
        TO_STRING(VK_SUCCESS);
        TO_STRING(VK_NOT_READY);
        TO_STRING(VK_TIMEOUT);
        TO_STRING(VK_EVENT_SET);
        TO_STRING(VK_EVENT_RESET);
        TO_STRING(VK_INCOMPLETE);
        TO_STRING(VK_ERROR_OUT_OF_HOST_MEMORY);
        TO_STRING(VK_ERROR_OUT_OF_DEVICE_MEMORY);
        TO_STRING(VK_ERROR_INITIALIZATION_FAILED);
        TO_STRING(VK_ERROR_DEVICE_LOST);
        TO_STRING(VK_ERROR_MEMORY_MAP_FAILED);
        TO_STRING(VK_ERROR_LAYER_NOT_PRESENT);
        TO_STRING(VK_ERROR_EXTENSION_NOT_PRESENT);
        TO_STRING(VK_ERROR_FEATURE_NOT_PRESENT);
        TO_STRING(VK_ERROR_INCOMPATIBLE_DRIVER);
        TO_STRING(VK_ERROR_TOO_MANY_OBJECTS);
        TO_STRING(VK_ERROR_FORMAT_NOT_SUPPORTED);
        TO_STRING(VK_ERROR_FRAGMENTED_POOL);
        TO_STRING(VK_ERROR_UNKNOWN);
        TO_STRING(VK_ERROR_OUT_OF_POOL_MEMORY);
        TO_STRING(VK_ERROR_INVALID_EXTERNAL_HANDLE);
        TO_STRING(VK_ERROR_FRAGMENTATION);
        TO_STRING(VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS);
        TO_STRING(VK_PIPELINE_COMPILE_REQUIRED);
        TO_STRING(VK_ERROR_SURFACE_LOST_KHR);
        TO_STRING(VK_ERROR_NATIVE_WINDOW_IN_USE_KHR);
        TO_STRING(VK_SUBOPTIMAL_KHR);
        TO_STRING(VK_ERROR_OUT_OF_DATE_KHR);
        TO_STRING(VK_ERROR_INCOMPATIBLE_DISPLAY_KHR);
        TO_STRING(VK_ERROR_VALIDATION_FAILED_EXT);
        TO_STRING(VK_ERROR_INVALID_SHADER_NV);
        TO_STRING(VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT);
        TO_STRING(VK_ERROR_NOT_PERMITTED_KHR);
        TO_STRING(VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT);
        TO_STRING(VK_THREAD_IDLE_KHR);
        TO_STRING(VK_THREAD_DONE_KHR);
        TO_STRING(VK_OPERATION_DEFERRED_KHR);
        TO_STRING(VK_OPERATION_NOT_DEFERRED_KHR);
        TO_STRING(VK_RESULT_MAX_ENUM);
        default:
            return "UNKNOWN";
    }
#undef TO_STRING
}

} // namespace shrek::render::helper


namespace shrek::render {

// need to load these two functions due to it being external/extended
VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) SRK_NOEXCEPT
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr)
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    else
        return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) SRK_NOEXCEPT
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr)
        func(instance, debugMessenger, pAllocator);
    else
        SRK_CORE_WARN("unable to clean up debugMesssenger correctly");
}

std::ostream& operator<<(std::ostream& os, VkResult result)
{
    using namespace std;
    return os << shrek::render::helper::ToString(result);
}
}

