#pragma once

#include "defs.h"

namespace shrek {

class Application
{
public:
    Application() SRK_NOEXCEPT;

    // for linux based applications(?)
    Application([[maybe_unused]] int argc, [[maybe_unused]] char** argv) SRK_NOEXCEPT;
    ~Application() SRK_NOEXCEPT;

    void Run() SRK_NOEXCEPT;

private:
};

} // namespace shrek
