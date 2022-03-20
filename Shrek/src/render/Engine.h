#pragma once
#include "defs.h"
#include "vulkan.h"
#include "base/Singleton.h"


namespace shrek::render {

class Engine : private base::Singleton<Engine>
{
public:
    Engine() SRK_NOEXCEPT;
    ~Engine() SRK_NOEXCEPT;

    Engine& operator=(const Engine& other) SRK_NOEXCEPT = delete;
    Engine(const Engine& other) SRK_NOEXCEPT            = delete;
    Engine(Engine&& other) SRK_NOEXCEPT                 = delete;
    Engine& operator=(Engine&& other) SRK_NOEXCEPT = delete;

private:
    VkInstance               m_Instance;
    VkPhysicalDevice         m_Gpu;
    VkDevice                 m_LGpu; // L being logical
    VkDebugUtilsMessengerEXT m_DebugHandler;
};
} // namespace shrek::render
