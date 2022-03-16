#include "pch.h"

#include "platform/Application.h"

int main(int argc, char** argv)
{
    shrek::Application app(
        shrek::ApplicationCmdLineArgs{argc, argv});
    app.Run();
    return 0;
}
