#pragma once
#include "defs.h"
#include <optional>

#include <vulkan.h>
#include "base/Singleton.h"
#include "helper/QueueFamilyIndices.h"

namespace shrek::render {


class Engine : private base::Singleton<Engine>
{
public:
    Engine() SRK_NOEXCEPT;
    ~Engine() SRK_NOEXCEPT;

    // Honestly copy and move will be implicitly deleted from Singleton base class but should we explicitly do it?
    Engine& operator=(const Engine& other) SRK_NOEXCEPT = delete;
    Engine(const Engine& other) SRK_NOEXCEPT            = delete;
    Engine(Engine&& other) SRK_NOEXCEPT                 = delete;
    Engine& operator=(Engine&& other) SRK_NOEXCEPT = delete;

    // Vulkan API
    inline VkInstance                        GetInstance() const SRK_NOEXCEPT { return m_Instance; };
    inline VkPhysicalDevice                  GetGpu() const SRK_NOEXCEPT { return m_Gpu; };
    inline VkDevice                          GetLogicalGpu() const SRK_NOEXCEPT { return m_LGpu; };
    inline const helper::QueueFamilyIndices& GetQueueFamilyIndices() const SRK_NOEXCEPT { return m_QueueFamily; }

private:
    VkInstance               m_Instance;
    VkPhysicalDevice         m_Gpu;
    VkDevice                 m_LGpu; // L being logical
    VkDebugUtilsMessengerEXT m_DebugHandler;

    helper::QueueFamilyIndices m_QueueFamily;
    VkQueue                    m_Queue;
};
} // namespace shrek::render
