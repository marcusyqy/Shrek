#pragma once

#include "defs.h"
#include "WindowsWindow.h"
#include <memory>

namespace shrek {

/*
 *  Currently considering if i should write this class as a singleton because there can only be one
 *  application but doing that may introduce a race condition where we may need to have some sort of
 *  mutex to control the access of this class
 */
class Application
{
public:
    struct ApplicationEssentials;

    Application() SRK_NOEXCEPT;

    // for linux based applications(?)
    Application([[maybe_unused]] int argc, [[maybe_unused]] char** argv) SRK_NOEXCEPT;
    ~Application() SRK_NOEXCEPT;

    void Run() SRK_NOEXCEPT;

    // no move no copy
    Application(const Application& other) = delete;
    Application& operator=(const Application& other) = delete;

    Application(Application&& other) = delete;
    Application operator=(Application&& other) = delete;

private:
    std::unique_ptr<ApplicationEssentials> m_Context;
    WindowsWindow                          m_Window;
};

} // namespace shrek
