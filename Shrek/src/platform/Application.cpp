#include "pch.h"
#include "Application.h"

#include <iostream>
#include "Log.h"

namespace shrek
{
Application::Application()
{
    Log::Init();
}

// for linux based applications(?)
Application::Application([[maybe_unused]] int argc, [[maybe_unused]] char** argv) :
    Application()
{
}

Application::~Application()
{
    Log::Exit();
}

void Application::Run()
{
    std::cout << "Hello World" << std::endl;
}

} // namespace shrek
