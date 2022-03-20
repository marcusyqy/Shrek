require "vendor/export-compile-commands"

workspace "Shrek"
	architecture "x86_64"
	startproject "Shrek"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

	flags {
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
--IncludeDir["ImGui"] = "vendor/ImGui"
--IncludeDir["ImGuizmo"] = "vendor/ImGui/ImGuizmo"
IncludeDir["vulkan"] = "%{wks.location}/Shrek/vendor/vulkan/include"
IncludeDir["GLFW"] = "%{wks.location}/Shrek/vendor/glfw/include"
IncludeDir["spdlog"] = "%{wks.location}/Shrek/vendor/spdlog/include"

--for grouping projects in the future
group "Dependencies"
	warnings "Off"
	--include "vendor/ImGui"
    include "Shrek/vendor/glfw"

group ""
--start of engine project
project "Shrek"
	location "Shrek"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	characterset "MBCS"
	pchheader "pch.h"
	pchsource "%{prj.name}/src/pch.cpp"
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	--recursively add every cpp/h/hpp file into the project
	files {
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.h"
	}

	includedirs {
        "%{IncludeDir.vulkan}",
		--"%{IncludeDir.ImGui}",
		--"%{IncludeDir.ImGuizmo}",
        "%{IncludeDir.GLFW}",
		"%{IncludeDir.spdlog}",
		"%{prj.name}/src"
	}

	syslibdirs {
		"%{wks.location}/Shrek/vendor/vulkan"
	}

	 links {
         "GLFW",
		--"ImGui",
		"vulkan-1.lib"
	 }

	warnings "Extra"

	defines {
        "VK_PROTOTYPES",
        "VK_USE_PLATFORM_WIN32_KHR",
		"NOMINMAX"
	}

	--defines for msvc compiler
	filter "system:windows"
		systemversion "latest"
		defines { "WIN32", "_CRT_SECURE_NO_WARNINGS" }

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines { "SRK_DIST" }
		runtime "Release"
		optimize "on"
