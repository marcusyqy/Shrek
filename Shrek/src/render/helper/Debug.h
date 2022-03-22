#pragma once
#include "defs.h"
#include "vulkan.h"

#include <string_view>
#include <iostream>


namespace shrek::render::helper {

std::string_view ToString(VkResult result) SRK_NOEXCEPT;


} // namespace shrek::render::helper

namespace shrek::render {

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) SRK_NOEXCEPT;
void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) SRK_NOEXCEPT;

// put this function in this namespace so that it can be used without scope within shrek::render
 std::ostream& operator<<(std::ostream& os, VkResult result);

} // namespace shrek::render
