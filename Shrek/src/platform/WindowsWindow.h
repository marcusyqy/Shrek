#pragma once

#include "defs.h"
#include <GLFW/glfw3.h>


namespace shrek {

    class WindowsWindow
    {
    public:
        WindowsWindow() SRK_NOEXCEPT;

    private:
        GLFWwindow* m_Context;
    };
}
