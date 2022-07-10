#pragma once

#include "base/Singleton.h"
#include "defs.h"
#include "WindowManager.h"
#include <memory>

#include "render/Engine.h"

namespace shrek {

/*
 *  Currently considering if i should write this class as a singleton because there can only be one
 *  application but doing that may introduce a race condition where we may need to have some sort of
 *  mutex to control the access of this class
 */

class ApplicationCmdLineArgs
{
public:
    ApplicationCmdLineArgs(int argc, char** argv) SRK_NOEXCEPT;
    const char* operator[](size_t idx) const SRK_NOEXCEPT;
    size_t      Size() const SRK_NOEXCEPT;

private:
    size_t m_Size;
    char** m_Arguments;
};

class Application : private base::Singleton<Application>
{
public:
    Application() SRK_NOEXCEPT;

    // for linux based applications(?)
    Application(ApplicationCmdLineArgs params) SRK_NOEXCEPT;
    ~Application() SRK_NOEXCEPT;

    void Tick() SRK_NOEXCEPT;

    // no move no copy
    Application(const Application& other) = delete;
    Application& operator=(const Application& other) = delete;

    Application(Application&& other) = delete;
    Application operator=(Application&& other) = delete;

    bool ShouldClose() SRK_NOEXCEPT;
    void Load() SRK_NOEXCEPT;
    void Cleanup() SRK_NOEXCEPT;

private:
    WindowManager  m_WindowManager;
    bool           m_Running;
    render::Engine m_RenderEngine;
};

} // namespace shrek
