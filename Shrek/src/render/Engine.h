#pragma once
#include "defs.h"
#include <optional>

#include <vulkan.h>
#include "base/Singleton.h"

namespace shrek::render {

struct QueueFamilyIndices
{
    uint32_t Graphics;
    std::optional<uint32_t> Compute;
};

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

private:
    VkInstance               m_Instance;
    VkPhysicalDevice         m_Gpu;
    VkDevice                 m_LGpu; // L being logical
    VkDebugUtilsMessengerEXT m_DebugHandler;

    QueueFamilyIndices m_QueueFamily;
    VkQueue m_Queue;
};
} // namespace shrek::render
