#include "pch.h"

#include "platform/Log.h"
#include "platform/Application.h"

int main(int argc, char** argv)
{
    shrek::Log::Init();
    shrek::Application app(
        shrek::ApplicationCmdLineArgs{argc, argv});
    app.Run();

    shrek::Log::Exit();
    return 0;
}
