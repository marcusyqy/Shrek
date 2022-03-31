#include "pch.h"

#include "platform/Log.h"
#include "platform/Application.h"

int main(int argc, char** argv)
{
    shrek::Log::Init();

    // scope the creation of everything else
    {
        shrek::Application app(shrek::ApplicationCmdLineArgs{argc, argv});

        SRK_CORE_TRACE("Loading engine now");
        app.Load();

        while (!app.ShouldClose())
        {
            app.Tick();
        }

        SRK_CORE_INFO("Exitting from {} engine now...", "Shrek");
        app.Cleanup();
    }

    shrek::Log::Exit();
    return 0;
}
