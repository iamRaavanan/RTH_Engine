workspace "RTH"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}
	
outputdir = "%{cfg.buildcfg}%{cfg.system}%{cfg.architecture}"

-- Including the directories relative to the root folder
IncludeDir = {}
IncludeDir["GLFW"] = "RTH/Plugins/GLFW/include"

include "RTH/Plugins/GLFW"

project "RTH"
	location "RTH"
	kind "SharedLib"
	language "C++"

	targetdir ("Binaries/" .. outputdir .. "/%{prj.name}")
	objdir ("Intermediate/" .. outputdir .. "/%{prj.name}")

	pchheader "Rthpch.h"
	pchsource "%{prj.name}/Source/Rthpch.cpp"
	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/Source",
		"%{prj.name}/Plugins/spdlog/include",
		"%{IncludeDir.GLFW}"
	}
	links
	{
		"GLFW",
		"opengl32.lib"
	}
	
	filter "system:windows"
		cppdialect "C++17"
		--staticruntime "On"
		staticruntime "Off"
		systemversion "latest"

		defines
		{
			"RTH_PLATFORM_WINDOWS",
			"RTH_BUILD_DLL",
			"RTH_ENABLE_ASSERTS"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../Binaries/" .. outputdir .. "/Playground")
		}
	
	filter "configurations:Debug"
		defines "RTH_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "RTH_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "RTH_DIST"
		optimize "On"
	
	filter "action:vs*"
		buildoptions { "/utf-8" }

		
project "Playground"
	location "Playground"
	kind "ConsoleApp"
	language "C++"

	targetdir ("Binaries/" .. outputdir .. "/%{prj.name}")
	objdir ("Intermediate/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp"
	}

	includedirs
	{
		"RTH/Source",
		"RTH/Plugins/spdlog/include"
	}

	links
	{
		"RTH"
	}

	filter "system:windows"
		cppdialect "C++17"
		--staticruntime "On"
		staticruntime "Off"
		systemversion "latest"

		defines
		{
			"RTH_PLATFORM_WINDOWS",
			"RTH_ENABLE_ASSERTS"
		}
	
	filter "configurations:Debug"
		defines "RTH_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "RTH_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "RTH_DIST"
		optimize "On"

	filter "action:vs*"
		buildoptions { "/utf-8" }